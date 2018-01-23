
var ADMIN_CALENDAR_PERM_LIST = null;
var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
var selDay = null;
try {
    if (calendardata == undefined || calendardata == "")
        calendardata = {
            "classrooms": []
        };
} catch (e) {
    calendardata = {
        "classrooms": []
    };
}

function getCurrentLanguage() {
	var lang = Drupal.settings.user.language;
	if(lang == "zh-hans")
		lang = "zh-cn";
	else if(lang == "ru")
		lang = "ru";	
	else if(lang == "de")
		lang = "de";	
	else if(lang == "es")
		lang = "es";	
	else if(lang == "ja")
		lang = "ja";	
	else if(lang == "ko")
		lang = "ko";	
	else if(lang == "pt-pt")
		lang = "pt";	
	else if(lang == "it")
		lang = "it";	
	else if(lang == "fr")
		lang = "fr";	
	return lang;
}
$(document).ready(function() {
/* Highlight root admin menu */
	if($('#block-system-main-menu a[href="/?q=admincalendar"]').hasClass('active') == false){
		$('#block-system-main-menu a[href="/?q=administration"]').addClass('active-trail active');
		$('#block-system-main-menu a[href="/?q=administration"]').parent().addClass('active-trail');
	}
// closePopupOnEscAndOutsideClick("#select-list-calendar-dropdown-list");

	$(document).mouseup(function(e) {
		if (e.target.id != 'admin-dropdown-arrow') {
			$("#select-list-calendar-dropdown-list").hide();
		}
	});

$.ajaxSetup({
	//dataType: 'json',
	cache: false,
	 xhr: function() {
         //return new window.XMLHttpRequest();
         try{
             if(window.ActiveXObject)
                 return new window.ActiveXObject("Microsoft.XMLHTTP");
         } catch(e) { }

         return new window.XMLHttpRequest();
     }
});


	lang = getCurrentLanguage();
	 // set user language
    moment.locale(lang);
    if(ADMIN_CALENDAR_PERM_LIST == null)
    	ADMIN_CALENDAR_PERM_LIST = Drupal.settings.cal.plist;
    if ($('#calendarprimaryview').size() == 0)
        return;
    var testview = ($.cookie("admincalendar_view") != undefined && $.cookie("admincalendar_view") != null && $.cookie("admincalendar_view") != '') ? $.cookie("admincalendar_view").toString().replace(/"/g, "") : 'agendaDay';
    $newJquery('#calendarprimaryview').fullCalendar({
    
     
        header: {
            right: '',
            left: "prev,today,next",
            center: 'agendaDay,basicWeek,month',//year,yearButton'
        },
        defaultView: testview,
        //         defaultDate: '2016-01-01',
        editable: true,
        droppable:true,
        lang: lang,
	  
        handleWindowResize: false,
        //events:"?q=administration/calendarviewapi",
       // eventLimit: 4, // allow "more" link when too many events
       // eventLimitText: "...",
        yearColumns: 3,
        eventOrder:"title",
        nextDayThreshold: '00:00',
        height: 665,
        views: {
       		 month: {
            		eventLimit: 4 // adjust to 6 only for agendaWeek/agendaDay
			        },
			 basicWeek:{
			 		eventLimit:30,
			 		columnFormat: 'ddd MM-DD-YYYY'
			 },
			 agendaDay:{
			 		eventLimit: false,
			 		slotDuration: "00:15:00",
			 		slotEventOverlap:false,
			 		//smallTimeFormat: 'HH:mm',
			 		
			 },
			 year:{
			 		eventLimit:1
			 }
			        
    	},
    	buttonText: {
    	    month: Drupal.t('LBL913'),
    	    week: Drupal.t('LBL1069'),
    	    day: Drupal.t('LBL910')
    	},
    	dayRightclick: function(date, jsEvent, view) {
	        //alert('a day has been rightclicked!');
	        addObjectThroughAdmCalendar(date,jsEvent);
        	// Prevent browser context menu:
    	    return false;
    	},
    	events:calendardata,
//        events: calendardata.vcrooms.concat(calendardata.classrooms).concat(calendardata.announcements).concat(calendardata.events).concat(calendardata.orders).concat(calendardata.reports), //.concat(calendardata.locations),

        dayClick: function(date, jsEvent, view) {
        /*	alert(date);
            if (view.name == "year") {
                $newJquery("#calendarprimaryview").fullCalendar("changeView", "month");
                $newJquery("#calendarprimaryview").fullCalendar("gotoDate", date);
                $newJquery("#calendarnarrowview").fullCalendar("gotoDate", date);

            }*/
        },

        eventLimitClick:function(cellInfo, jsEvent) {
        	var view = $newJquery('#calendarprimaryview').fullCalendar('getView');
            var events = '';
            var cellIdClass = "fc-more-" + cellInfo.cellEl.row + cellInfo.cellEl.col;
            var cellGridClass = "fc-grid-" + cellInfo.cellEl.row + cellInfo.cellEl.col;
            //var eventsBinded = $newJquery('#calendarprimaryview .' + cellGridClass).hasClass("events-attached");
        	var dayGrid = $newJquery('#calendarprimaryview').fullCalendar('getView').dayGrid;
        	var date = cellInfo.cellEl.start;
        	var dayEl = dayGrid.getCellDayEl(cellInfo.cellEl);
			var search_type = $("#search_all_calendar_type-hidden").val() ;
            /*if (eventsBinded == true) { // Events already attached, served from local cache.
	        	var allSegs = dayGrid.getCellSegs(cellInfo.cellEl);
	        	var reslicedAllSegs = dayGrid.resliceDaySegs(allSegs, date);
	        	dayGrid.showSegPopover(cellInfo.cellEl, cellInfo.moreEl, reslicedAllSegs);
            } else {*/
	        	var momentObj = moment(cellInfo.date).format("YYYY-MM-DD");
                var startDate = momentObj;
                var endDate = moment(cellInfo.date).add(1, "days").format("YYYY-MM-DD");
                	adminCalCreateLoader('calendarsummary');
            		$.ajax({
            	        url: "/?q=administration/calendarviewapi",
                    	METHOD: "post",
                	    data: "type=more&narrowcaldaywise=true&startDate=" + startDate + "&endDate=" + startDate +"&search_type="+ search_type,
            	        success: function(data) {
            	        	var events = JSON.parse(data);
            	        	remEvents = removeExistingEvents(events, cellInfo.segs);
            	        	$newJquery('#calendarprimaryview').fullCalendar('addEventSource', remEvents); 
            	        	$newJquery('#calendarprimaryview .' + cellGridClass).addClass("events-attached");
            	        	var dayEl = dayGrid.getCellDayEl(cellInfo.cellEl);
            	        	var allSegs = dayGrid.getCellSegs(cellInfo.cellEl);
            	        	var reslicedAllSegs = dayGrid.resliceDaySegs(allSegs, date);
            	        	dayGrid.showSegPopover(cellInfo.cellEl, $newJquery('#calendarprimaryview .' + cellIdClass), reslicedAllSegs);
            	        	adminCalDestroyLoader('calendarsummary');
                	},
                });
	        	
	       // }

        },
        eventAfterAllRender: function() {
           var view = $newJquery('#calendarprimaryview').fullCalendar('getView');
          
            //$("#calendarprimaryview .fc-content-skeleton .fc-more").html("...");
            if ($('#loader-container').length)
            	$('#loader-container').hide();
      	  	$(".fc-widget-header").css("margin-right", "0");
            if ($("#calendarprimaryview .fc-today").size() == 0)
                $("#calendarprimaryview .fc-today-button").addClass("notcurrent");
            else $("#calendarprimaryview .fc-today-button").removeClass("notcurrent");


            var addImg = getCalendarCellAddImg("primary","");//"<a class='add_img' href='javascript:void(0)' title='Click here to add entities' onclick='addObjectThroughAdmCalendar(this,event);'><img src='sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_admincalendar/calendar_object_add.png'></img></a>";
            var listImg = getCalendarCellListImg("primary","");//"<a class='list_img' href='javascript:void(0)' todaydate='2015-12-12' onclick='listObjectThroughAdmCalendar(this,event);' title='List Entities'   ><img src='sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_admincalendar/calendar_listicon.png'></img></a>";
            if (view.name == "month") {
                $('#calendarprimaryview .fc-day-number').each(function() {
                    var day = 0;
                    if (isNaN($(this).html())) {
                        day = parseInt($newJquery(this).find(".date").html());
                    } else
                        day = parseInt($(this).html());
                    //// // // console.log("Day:"+day);
                    $(this).html("<span class='date'>" + day + "</span><span class='more-dot'></span><span class='list-add-icons'>" + listImg + addImg + "</span>");
                });
               // mouseOverDelay("#calendarprimaryview .fc-more", admincalendarmouseover);
               

            } else if (view.name == "basicWeek") {
                //mouseOverDelay(document.getElementById('calendarprimaryview'), null);
                $('#calendarprimaryview .fc-day-header').each(function() {
                    var day = $(this).html();
                    var newdate = day.split(" ");
                    var newDateHtml = newdate[0] + ' ' + moment(newdate[1], "MM-DD-YYYY").format('DD/MM');
                    if(day.indexOf("div") == -1) {
                    	addImg = getCalendarCellAddImg("primary",newdate[1]);
                    	listImg = getCalendarCellListImg("primary",newdate[1]);
                    	$(this).html("<div class='date vtip' title='"+newDateHtml+"'>" + newDateHtml + "<span class='fade-out'></span></div><div class='list-add-icons'>" + listImg + addImg + "</div>");
                    }
                });
//                var tablHeight = $('#calendarprimaryview div.fc-content-skeleton table').height();
//                if(parseInt(tablHeight)>585){
//        			$('#calendarprimaryview div.fc-bg').css('margin-right','13px');
//        		}

            } else if (view.name == "agendaDay") {
            	//mouseOverDelay(document.getElementById('calendarprimaryview'), null);

                var date = $newJquery('#calendarprimaryview').fullCalendar('getDate').format("MM-DD-YYYY");

                $('#calendarprimaryview .fc-day-header').each(function() {
                    var day = $(this).html();
                    //// // // console.log("Day:"+day);
                    if(day.indexOf("div") == -1)
                    {
                    	var date = $newJquery('#calendarprimaryview').fullCalendar('getDate').format("MM-DD-YYYY");
                    	addImg = getCalendarCellAddImg("primary",date);
                    	listImg = getCalendarCellListImg("primary",date);
                    	var dateHtml =  $newJquery('#calendarprimaryview').fullCalendar('getDate').format("DD/MM");
	                    $(this).html("<div class='date' data-date='"+date+"'>" + day + " " + dateHtml + "</div><div class='list-add-icons'>" + listImg + addImg + "</div>");
	                }
                });
   				$('.fc-agendaDay-view .fc-day-grid .fc-week').css({'margin-right':'0px', 'border-right':'0px'});
            }
            attachEvents('#calendarprimaryview .fc-title a'); // add ajax events when view changed
            calendarallpopupclose();
            vtip();
            $.cookie("current_page", '1#0~0#0~0#0',{ expires : 1 });  
            if($newJquery("#calendarprimaryview").fullCalendar('getView').name == "basicWeek")
		    {
				var weekStartDay = $newJquery("#calendarprimaryview").fullCalendar('getView').intervalStart.format("YYYY-MM-DD");
				$(".narrowcalendarrowselect").removeClass(); // week class remove
		    	$newJquery("#calendarnarrowview  td").find('.narrowcalendardayselect').removeClass('narrowcalendardayselect'); // day class remove
				$newJquery("#calendarnarrowview td").find("[data-date='"+weekStartDay+"']").parent().addClass("narrowcalendarrowselect");                	
		    } else if($newJquery("#calendarprimaryview").fullCalendar('getView').name == "agendaDay"){
		    	$(".narrowcalendarrowselect").removeClass(); // week class remove
		    	$newJquery("#calendarnarrowview  td").find('.narrowcalendardayselect').removeClass('narrowcalendardayselect'); // day class remove
		    	var selectedDay = $newJquery("#calendarprimaryview").fullCalendar('getView').intervalStart.format("YYYY-MM-DD");
		    	$newJquery("#calendarnarrowview  td").find("[data-date='"+selectedDay+"']").addClass("narrowcalendardayselect");
    		} else {
    			$(".narrowcalendarrowselect").removeClass(); // week class remove
		    	$newJquery("#calendarnarrowview  td").find('.narrowcalendardayselect').removeClass('narrowcalendardayselect'); // day class remove
		    }
        },

        dayRender: function(date, cell) {
            //("dayRender...");

            var view = $newJquery('#calendarprimaryview').fullCalendar('getView');
           // if (view.name == "year")
             //   return false;

            //$newJquery(cell).attr("onMouseover","alert(2);");
            // cell.find(".fc-day-number").append("<div class='list-add-icons' style='text-align:right;'>" + listImg + "&nbsp;" + addImg + "</div>");
            //cell.find(".fc-day-number").remove();
            //// // // console.log(cell.html());
            //}
        },
        drop: function(date, jsEvent, ui, resourceId) {
        	var clientData = $newJquery('#calendarprimaryview').fullCalendar('clientEvents');
    		var eventId = $(this).data('event');
    		//console.log(eventId);
    		var event = getEventbyId(clientData, eventId);
    		//console.log("Dropped on" + date.format("YYYY-MM-DD"));
        	delta = {'dropdate' : date.format("YYYY-MM-DD")};
        	var startDate = event[0].start;
        	var startDateStr = event[0].start.format("YYYY-MM-DD");
           	var endDate = event[0].end;
           	var endDateStr = event[0].end.format("YYYY-MM-DD");
           	var dayDiff = endDate.diff(startDate, 'days');
           	event[0].start = moment(date);
        	if(startDateStr != endDateStr)
        		event[0].end = moment(date).add("days", dayDiff);
        	displayDragConfirm(Drupal.t('ACLBL0040')+" &quot;" + event[0].title + "&quot;", event[0], delta, revertFuncDummy);
        	//eventDropUpdate(event[0], delta, revertFuncDummy);
    	},
        eventDrop: function(event, delta, revertFunc) {
            //alert(JSON.stringify(delta));
        	displayDragConfirm(Drupal.t('ACLBL0040')+" &quot;" + event.title + "&quot;", event, delta, revertFunc);
        	//eventDropUpdate(event, delta, revertFunc);
           // calendarUpdateData(event,revertFunc);

        },
        eventRender: function(event, element) {
           if(!eventCanBeDisplayed(event))
			{
				return false;
			}
            var view = $newJquery('#calendarprimaryview').fullCalendar('getView');
		
            /*if (view.name == "year")
            {

              if (element.find(".more-dot").size() == 0)
               		 element.find(".fc-title").html("<span class='more-dot'></span>");
               	 switch (event.type) {

                case "lrn_cls_dty_ilt":
                    element.find(".more-dot").append("<span style=' color:#5cca32'>.</span>");
                    break;
                case "lrn_cls_dty_vcl":
                    element.find(".more-dot").append("<span style=' color:#3999E1'>.</span>");
                    break;
                case "event_regend":
                    element.find(".more-dot").append("<span style=' color:#CA9ED5'>.</span>");
                    break;
                case "event_regcancel":
                    element.find(".more-dot").append("<span style=' color:#CA9ED5'>.</span>");
                    break;
                case "announcement":
                    element.find(".more-dot").append("<span style=' color:#FFD975'>.</span>");
                    break;
                case "order":
                    element.find(".more-dot").append("<span style=' color:#FFD975'>.</span>");
                    break;
                    
            	}
            } else*/ 
            if (view.name == "month") {
                var titleLink = displayCalendarEvent(event, "");
                element.find(".fc-title").html(titleLink);
            }else if (view.name == "agendaDay") {
            	var titleLink = displayCalendarEvent(event, "");
            	var elemClass = eventClass(event); // set event class to day view
                element.find(".fc-title").html(titleLink);
                if(event.type != "order")
                {
                	var startTime = (event.startTime == undefined) ? '' : event.startTime;
                	var endTime = (event.endTime == undefined) ? '' : event.endTime;
					eventStartTime = calTimeFormat(event.from_date,event.startTime);	
                	var eventTilte = event.title;
                	element.find(".fc-bg").attr('title', $("<div/>").html(eventTilte).text());
                	element.find(".fc-bg").addClass(event.dataType);
                	element.find(".fc-bg").addClass(elemClass +' vtip');
                }
                else
                {
                	element.find(".fc-bg").removeClass("fc-bg");
                }
	            
            }else {
                var titleLink = displayCalendarEvent(event, "");
                element.find(".fc-title").html(titleLink);
            }
        },eventClick: function(calEvent, jsEvent, view) {
           $("#calendarprimaryview .fc-agenda-view .fc-bg").live('click', function(){
        		$(this).prev('.fc-content').find('.titlelinkstyle').click();
        	});  
        },
        eventDragStart: function( event, jsEvent, ui, view ) { 
        	$(this).find('.titletimestyle').removeAttr('title'); // remove title when you drag an element
        	$(this).find('.link').removeAttr('title').css('cursor', 'move');
        	$(this).css('cursor', 'move');
        	$("p#vtip").remove(); // remove active vtip while draging an element
        },
        viewRender: function(view, element) {
    	   if($.cookie("uri") == null){
              var uri = "?q=administration";
           }else{
              var uri = $.cookie("uri");
           }
           var listViewImg = "<a class='cal_list_view' href='"+uri+"'><span class='cal-list-view-img vtip' title='"+ Drupal.t('ACLBL0027') +"'> </span></a>";
           var calendarExportImg = "<a class='cal_export' href='javascript:void(0)' onClick='exportAdminCalendar();'><span class='cal-export-img vtip' title='" + Drupal.t('Export to CSV') + "'> </span></a>";
           var calendarPrintImg = "<a class='cal_print' href='javascript:void(0)' onClick='printAdminCalendar();'><span class='cal-print-img vtip' title='"+ Drupal.t('Print as PDF') +"'> </span></a>";
           var calendarSettingsImg = "<a class='cal_setting' id='admincalendarsettings' href='javascript:void(0)' onClick='displayAdminCalendarSettingsHideShow();' ><span class='cal-setting-img vtip' title='"+ Drupal.t('LBL563') +"'> </span></a><div id='qtipdiv'></div>";
   			if($("#qtipdiv").size() == 0)
   	            $("#calendarprimaryview .fc-right").html(listViewImg + "&nbsp;" + calendarExportImg + "&nbsp;" + calendarPrintImg + "&nbsp;" + calendarSettingsImg);
			
        },
        eventDestroy: function() {
        	
        }

    });

    $newJquery('#calendarnarrowview').fullCalendar({
        header: {
            right: 'prev,next',
            left: 'title',
            center: ''
        },
        nextDayThreshold: '00:00:00',
        lang:lang,
        handleWindowResize: false,
        eventOrder:'title',
        prev: function() {
            //alert('prev');
            //$('#calendar').load("events/findbymonth/"+$('#calendar').fullCalendar('getDate').getMonth());
        },
        next: function() {
            //alert('next');
            //$('#calendar').load("events/findbymonth/"+$('#calendar').fullCalendar('getDate').getMonth());
        },
        eventLimitClick: function(cellInfo, jsEvent) {
            //// // // console.log("1");
        },

        eventRender: function(event, element) {
            if (element.find(".more-dot").size() == 0)
                element.find(".fc-title").html("<span class='more-dot'></span>");
           /* switch (event.type) {
                case "lrn_cls_dty_ilt":
                    element.find(".more-dot").append("<span style=' color:#5cca32'>.</span>");
                    break;
                case "lrn_cls_dty_vcl":
                    element.find(".more-dot").append("<span style=' color:#3999E1'>.</span>");
                    break;
                case "event_regend":
                    element.find(".more-dot").append("<span style=' color:#CA9ED5'>.</span>");
                    break;
                case "event_regcancel":
                    element.find(".more-dot").append("<span style=' color:#CA9ED5'>.</span>");
                    break;
                case "announcement":
                    element.find(".more-dot").append("<span style=' color:#FFD975'>.</span>");
                    break;
            }*/
        },
        eventAfterAllRender: function() {
         //$("#calendarprimaryview .fc-agendaDay-view .fc-content-skeleton .fc-event-container > a.fc-event").each(function() {
   		 //console.log("load event eventAfterAllRender");
   		 $("#calendarprimaryview .fc-agendaDay-view .fc-time-grid-container .fc-content-skeleton .fc-event-container > a.fc-event").addClass("set-day-event-width");
   		 var getWid=$(".set-day-event-width").width();
   		 getWid=getWid-2;
   		 $(".set-day-event-width").width(getWid);
   		 setTimeout(function(){
	   		  var getCount=$(".set-day-event-width").length;
	   	      var scrollWidth=getCount * 86;
	   	      var scrollConDefaultWidth=732;
	   	      //console.log("get width for panel new: "+scrollWidth);
		   	   if(scrollWidth > scrollConDefaultWidth ){
		   		//$("#calendarprimaryview .fc-agendaDay-view .fc-scroller .fc-time-grid").jScrollPane({}).data('jsp').destroy();
		   		$("#calendarprimaryview .fc-agendaDay-view .fc-scroller .fc-time-grid").css("width",scrollWidth+100);
		   	   }
   		 },300);
            $('#calendarnarrowview .fc-day-number').each(function() {
                // Get current day
                var day = 0;
                if (isNaN($(this).html())) {
                    day = parseInt($newJquery(this).find(".date").html());
                } else
                    day = parseInt($(this).html());
                //    			// // // console.log("Day:"+day);

                $(this).html("<span class='date'>" + day + "</span><span class='more-dot'></span>");
            });
            $("#calendarnarrowview  .fc-time").each(function() {
               $(this).html("");
            });
            /*$("#calendarnarrowview  .fc-more").each(function() {
                $(this).html("<span style=' color:#3999e1'>.</span><span style=' color:#3999e1'>.</span><span style=' color:#3999e1'>.</span>");
            });*/
            if ($("#calendarsummary").size() == 0)
                $("#calendarnarrowview .fc-view-container").append("<div id='calendarsummary'></div>");
		

        
  			//displayCalendarSummaryEventsList(); // commented due to event refresh moved to another functions
  			if($newJquery("#calendarprimaryview").fullCalendar('getView').name == "basicWeek")
		    {
				var weekStartDay = $newJquery("#calendarprimaryview").fullCalendar('getView').intervalStart.format("YYYY-MM-DD");
		    	$(".narrowcalendarrowselect").removeClass(); // week class remove
		    	$newJquery("#calendarnarrowview  td").find('.narrowcalendardayselect').removeClass('narrowcalendardayselect'); // day class remove
				$newJquery("#calendarnarrowview  td").find("[data-date='"+weekStartDay+"']").parent().addClass("narrowcalendarrowselect");                	
		    } else if($newJquery("#calendarprimaryview").fullCalendar('getView').name == "agendaDay") {
		    	$(".narrowcalendarrowselect").removeClass(); // week class remove
		    	$newJquery("#calendarnarrowview  td").find('.narrowcalendardayselect').removeClass('narrowcalendardayselect'); // day class remove
		    	var selectedDay = $newJquery("#calendarprimaryview").fullCalendar('getView').intervalStart.format("YYYY-MM-DD");
		    	$newJquery("#calendarnarrowview  td").find("[data-date='"+selectedDay+"']").addClass("narrowcalendardayselect");
		    } else {
		    	$(".narrowcalendarrowselect").removeClass(); // week class remove
		    	$newJquery("#calendarnarrowview  td").find('.narrowcalendardayselect').removeClass('narrowcalendardayselect'); // day class remove
		    }
	    	highlightNarrowCalendarActiveDates();



        },
        dayClick: function(date, jsEvent, view) {
        	var selectedDate = date.format();
        	selDay = selectedDate;
        	$("#calendarsummary .today-events, #calendarsummary .tmrw-events").fadeOut();
            //var curView = ($.cookie("admincalendar_view") != undefined && $.cookie("admincalendar_view") != null && $.cookie("admincalendar_view") != '') ? $.cookie("admincalendar_view").toString().replace(/"/g, "") : 'month';
            var curView = $newJquery("#calendarprimaryview").fullCalendar('getView');
            if(curView.type == 'agendaDay'){
            	$newJquery('#calendarprimaryview').fullCalendar( 'gotoDate', selectedDate );
            	 getCalendarData({'refreshMode':'both'}); // only refresh events in primary cal
            	formatCalendarDataSummary(date.format("YYYY-MM-DD"),'',selectedDate);
            } else if (curView.type == 'basicWeek') {
            	formatCalendarDataSummary(date.format("YYYY-MM-DD"),'',selectedDate);
            } else if (curView.type == 'month') {
            	formatCalendarDataSummary(date.format("YYYY-MM-DD"),'',selectedDate);
            }
        },

        // defaultDate: '2016-01-01',
        editable: true,
        eventLimit: 1, // allow "more" link when too many events
        //   events: calendardata.vcrooms.concat(calendardata.classrooms).concat(calendardata.announcements).concat(calendardata.events).concat(calendardata.orders).concat(calendardata.reports), //.concat(calendardata.locations),
		events:calendardata
    });
    $(".fc-year-button").click(function(){
        $(".fc-year-monthly-td .fc-more-cell").html("");
        $('.active-qtip-div').remove();	
    	getCalendarData();	

    });
    $('.fc-month-button, .fc-basicWeek-button, .fc-agendaDay-button').click(function () {
    	$('.active-qtip-div').remove();	
    	getCalendarData();	

	});
    $("#calendarprimaryview .fc-button-group .fc-button").click(function(){
    	var admin_view = $newJquery("#calendarprimaryview").fullCalendar('getView');
    	$.cookie("admincalendar_view", admin_view.type);    
   		selDay = null;
    });
    
    reArangeSearchFilter();
});

function highlightNarrowCalendarActiveDates() {
		$newJquery("#calendarnarrowview .fc-content-skeleton .fc-day-number").removeClass('dt-with-events');
		var rowStructs = $newJquery("#calendarnarrowview").fullCalendar('getView').dayGrid.rowStructs;
		var rowStructsLen = rowStructs.length;
	    var segMatrixLen, segMatrixRowLen = 0;
	    var segMatrix, segs, segMatrixRow, gridId = '';
	    for (i=0; i < rowStructsLen; i++ ) {
	    	segMatrix = rowStructs[i].segMatrix;
	    	segs = rowStructs[i].segs;
	    	if (segs.length > 0) { // row has any segments
	    		segMatrixLen = segMatrix.length
				for (j=0; j < segMatrixLen; j++) {
					segMatrixRow = segMatrix[j];
					segMatrixRowLen = segMatrixRow.length
					for (var key in segMatrixRow) {
					    gridId = 'fc-grid-' + i + key;
						$newJquery("#calendarnarrowview .fc-content-skeleton ." + gridId).addClass('dt-with-events');
					}
				}
	    	}
	    }
}

function acYearviewMonthCalCell(month,top,left)
{
	return "<div class='"+month+"-month-cal yearview' style='width:240px;height:350px;padding:5px;' month='"+month+"'></div>";
}
function buildACYearView()
{
	$("#calendarprimaryview .fc-view-container").hide();
	$("#calendarprimaryview").parent().append("<div id='yearview' style='position:absolute;top:16px;left:210px;height:610px;margin-top:20px;overflow-y:auto;'></div>");
	var html = "";
	html += "<table cellpadding=1 cellspacing=1>";
	html += "<tr><td style='vertical-align:top;'>";
	html	+=	acYearviewMonthCalCell("jan");
	html +="</td><td style='vertical-align:top;'>";
	html	+=	acYearviewMonthCalCell("feb");
	html += "</td><td style='vertical-align:top;'>";
	html	+=	acYearviewMonthCalCell("mar");
	html += "</td></tr>";

	html += "<tr><td style='vertical-align:top;'>";
	html	+=	acYearviewMonthCalCell("apr");
	html +="</td><td style='vertical-align:top;'>";
	html	+=	acYearviewMonthCalCell("may");
	html += "</td><td style='vertical-align:top;'>";
	html	+=	acYearviewMonthCalCell("jun");
	html += "</td></tr>";

	html += "<tr><td style='vertical-align:top;'>";
	html	+=	acYearviewMonthCalCell("jul");
	html +="</td><td style='vertical-align:top;'>";
	html	+=	acYearviewMonthCalCell("aug");
	html += "</td><td style='vertical-align:top;'>";
	html	+=	acYearviewMonthCalCell("sep");
	html += "</td></tr>";


	html += "<tr><td style='vertical-align:top;'>";
	html	+=	acYearviewMonthCalCell("oct");
	html +="</td><td style='vertical-align:top;'>";
	html	+=	acYearviewMonthCalCell("nov");
	html += "</td><td style='vertical-align:top;'>";
	html	+=	acYearviewMonthCalCell("dec");
	html += "</td></tr>";

	
	html += "</table>";
//	html	+=	acYearviewMonthCalCell("mar");
    var calendarDate = $newJquery('#calendarnarrowview').fullCalendar('getDate');	
	$("#yearview").html(html);
	$("#yearview .yearview").each(function(){
	var monthCal = $(this).attr("month");
	$newJquery("."+monthCal+"-month-cal").fullCalendar({
        header: {
            right: '',
            left: "",
            center: ''
        },
        editable: false,
        droppable:false,
        lang: lang,
        contentHeight:340,
        handleWindowResize: false,
	//	events:"?q=administration/calendarviewapi",
        eventLimitText: "...",
        eventLimit:1,
        nextDayThreshold: '00:00'
    });
    });
    $("#yearview").attr("loaded","true");
    $("#yearview").attr("displayingyear",calendarDate.format("YYYY"));
    obj.destroyLoader("calendar");


}


function displayCalendarSummaryEventsList(selectedDate,isMoreData)
{
	var date = "";
    if(selectedDate == undefined || selectedDate == null)
    	date = $newJquery('#calendarnarrowview').fullCalendar('getDate');
    else
    	date = moment(selectedDate);
    	
    	var selectedDate = date.format("YYYY-MM-DD");
    	formatCalendarDataSummary(selectedDate,isMoreData, selectedDate);
}

function attachEvents(elements) {
	// // console.log(elements+":"+$(elements).size());
    /*---- Jquery attach event with Title ---*/
    // Drupal.behaviors.exp_sp_admincalendar = {
    //'attach': function(context, settings) {
    $(elements).not(".calendar-processed").each(function() {

        $(this).addClass('calendar-processed');

        var element_settings = {};

        element_settings.url = $(this).attr('href');
        element_settings.setClick = true;
        element_settings.event = 'click';
        element_settings.progress = {
            'type': 'throbber'
        };

        element_settings.keypress = true;
        element_settings.effect = 'none';
        element_settings.method = 'replaceWith';
        element_settings.submit = {
            'js': true
        };


        var base = $(this).attr('id');
        if (element_settings.url != 'javascript:void(0)') // Cross origin requests are only supported Issue in Chrome FIx 
        	Drupal.ajax[base] = new Drupal.ajax(base, this, element_settings);

    });
    // }
    //}
}

function displayCalendarDataAfterRefresh(data,params)
{
	 if($newJquery("#calendarprimaryview").fullCalendar("getView").name != "year")
	 {
  	    var momentObj = $newJquery('#calendarprimaryview').fullCalendar('getDate');
	    adminCalDestroyLoader()
	    calendardata = JSON.parse(data);
	    $newJquery('#calendarprimaryview').fullCalendar('removeEvents');
	    $newJquery('#calendarnarrowview').fullCalendar('removeEvents');
		
	    updateAdminCalendarEvents(calendardata, params);
	  //  $newJquery('#calendarprimaryview').fullCalendar('rerenderEvents');
	  //  $newJquery('#calendarnarrowview').fullCalendar('rerenderEvents');
	
	    $newJquery("#calendarnarrowview").fullCalendar("gotoDate", momentObj);
	    attachEvents('#calendarprimaryview .fc-title a');
	    if(selDay == null )
	       displayCalendarSummaryEventsList();
	
	    if($newJquery("#calendarprimaryview").fullCalendar("getView").name == "month")
	    {
	    	//if(params != undefined && params.initialLoad != 'true')
				//mouseOverDelay(document.getElementById('calendarprimaryview'), admincalendarmouseover);
		} 
	}
	else
	{
		  adminCalDestroyLoader()
    	  calendardata = JSON.parse(data);
    	  $newJquery('#calendarprimaryview').fullCalendar('removeEvents');
    	  updateAdminCalendarEvents(calendardata);
	}
	  				// console.timeEnd("Calendar Time");
}

function getCalendarData(params)
{
	  checkSessionOut();
	  var momentObj = $newJquery('#calendarprimaryview').fullCalendar('getDate');
	  var view = $newJquery("#calendarprimaryview").fullCalendar('getView');
	  var startDate = "";
	  var endDate = "";
	  var params = (params instanceof Object) ? params : {};
	    /*  if(view.name == "basicWeek" || view.name == "agendaDay")
	      {
	      	return true;
	      }
	      else*/
	      {
	      	 startDate = $newJquery("#calendarprimaryview").fullCalendar('getView').start.format("YYYY-MM-DD");
			 endDate = $newJquery("#calendarprimaryview").fullCalendar('getView').end.format("YYYY-MM-DD");
	      }
	      

        //     var startDate = momentObj.startOf("month").format("YYYY-MM-DD");
    	//    var endDate = momentObj.endOf('month').format("YYYY-MM-DD");
           
	      	adminCalCreateLoader();
		    //console.time("Calendar Time");
		/*	if($.browser.msie && $.browser.version == 9)
			{
				callAjaxForIE('/?q=administration/calendarviewapi?startDate='+startDate+"&endDate="+endDate,function(data)
				{
					adminCalDestroyLoader()
					displayCalendarDataAfterRefresh(data,params);
				}
				);
    		}
    		else */
    		{
    		   var ac_searchparams = getACSearchParams();
	            $.ajax({
    	            url: "/?q=administration/calendarviewapi",
        	        METHOD: "post",
            	    data: "startDate=" + startDate + "&endDate=" + endDate+ac_searchparams,
                	success: function(data) {
                		adminCalDestroyLoader();
                		displayCalendarDataAfterRefresh(data,params);
	                }
    	        });
    	    }
    	    $('#calendarlegendview').hide();
}

function getACSearchParams()
{
	var search_title = $('#narrow-search-calendar-filter').val();
	var langtitle = Drupal.t("LBL304").toUpperCase();
	var search_type = $("#search_all_calendar_type-hidden").val() ;
	var ac_searchparams =  "";
	var viewname = $newJquery("#calendarprimaryview").fullCalendar('getView').name;
	if((search_title.toLowerCase()) == (langtitle.toLowerCase()))
	{
		ac_searchparams = "&view="+viewname;			  
	}
	else
		ac_searchparams = "&search_text="+search_title+"&search_type="+search_type+"&view="+viewname;			  

	return ac_searchparams; 			  

}
function prevNarrowCalendarNavigation()
{
	 if($newJquery("#calendarprimaryview").fullCalendar('getView').name == "month")
	 {
	    $newJquery("#calendarprimaryview  .fc-prev-button").click();
	}
	else //if($newJquery("#calendarprimaryview").fullCalendar('getView').name == "week")
	{
		var monthStartDate = $newJquery('#calendarnarrowview').fullCalendar('getDate');
		$newJquery("#calendarprimaryview").fullCalendar("gotoDate", monthStartDate.startOf('month'));
	}
	    var selectedDate = $newJquery('#calendarnarrowview').fullCalendar('getDate');
	    formatCalendarDataSummary(selectedDate.format("YYYY-MM-DD"),'',selectedDate);
}


function nextNarrowCalendarNavigation()
{
	 if($newJquery("#calendarprimaryview").fullCalendar('getView').name == "month")
	 {
	    $newJquery("#calendarprimaryview  .fc-next-button").click();
	}
	else //if($newJquery("#calendarprimaryview").fullCalendar('getView').name == "week")
	{
		var monthStartDate = $newJquery('#calendarnarrowview').fullCalendar('getDate');
		$newJquery("#calendarprimaryview").fullCalendar("gotoDate", monthStartDate.startOf('month'));
	}
	    var selectedDate = $newJquery('#calendarnarrowview').fullCalendar('getDate');
	    formatCalendarDataSummary(selectedDate.format("YYYY-MM-DD"),'',selectedDate); 
}


$(document).ready(function() {
    attachEvents("#calendarprimaryview .fc-title a");
    vtip();
    getCalendarData({'initialLoad':'true'});
    $newJquery('.fc-today-button').click(
        function() {
        	$('.active-qtip-div').remove();	
          getCalendarData();

        }
    );

    /*---- Jquery attach event with Title ---*/
    //$newJquery('#calendarprimaryview .fc-prev-button').css("display", "none");
    //$newJquery('#calendarprimaryview .fc-next-button').css("display", "none");
    $newJquery('#calendarnarrowview .fc-prev-button').click(function() {
        calendarallpopupclose();
    	$('.active-qtip-div').remove();	

      if($newJquery("#calendarprimaryview").fullCalendar('getView').name == "year")
		{
		    var startDate = $newJquery("#calendarnarrowview").fullCalendar('getView').intervalStart;
		    startDate = startDate.add("1","month");
		    var newDate = parseInt(startDate.format("YYYY"),10)-1+"-"+startDate.format("MM-DD");
		    $newJquery("#calendarnarrowview").fullCalendar("gotoDate", newDate);
		    $newJquery("#calendarprimaryview").fullCalendar("gotoDate", newDate);
		    return false;


		}	  
		
		
        if ($newJquery("#calendarnarrowview  .fc-prev-button").attr("ajaxrequired") != "false") {
            var moment = $newJquery('#calendarnarrowview').fullCalendar('getDate');
            //var startDate = moment.format("YYYY-MM-DD");
            //var endDate = moment.endOf('month').format("YYYY-MM-DD");
            
            var startDate = $newJquery("#calendarnarrowview").fullCalendar('getView').start.format("YYYY-MM-DD");
			var endDate = $newJquery("#calendarnarrowview").fullCalendar('getView').end.format("YYYY-MM-DD");


            // // // console.log("startDate:" + startDate + " endDate:" + endDate);
			adminCalCreateLoader();
            
            /*if($.browser.msie && $.browser.version == 9)
			{
				callAjaxForIE('/?q=administration/calendarviewapi?startDate='+startDate+"&endDate="+endDate,function(data)
				{
					adminCalDestroyLoader()
					calendardata = JSON.parse(data);
	
    	            $newJquery('#calendarprimaryview').fullCalendar('removeEvents');
        	        $newJquery('#calendarnarrowview').fullCalendar('removeEvents');
					updateAdminCalendarEvents(calendardata);
                	$newJquery('#calendarprimaryview').fullCalendar('rerenderEvents');
                    $newJquery('#calendarnarrowview').fullCalendar('rerenderEvents');

	                $newJquery("#calendarprimaryview  .fc-prev-button").attr("ajaxrequired", "false");

	                prevNarrowCalendarNavigation();

        	        attachEvents('#calendarprimaryview .fc-title a');
				}
				);
    		}
    		else*/
    		{
    		    var ac_searchparams = getACSearchParams();
	            $.ajax({
    	            url: "/?q=administration/calendarviewapi",
        	        METHOD: "post",
            	    data: "startDate=" + startDate + "&endDate=" + endDate+ac_searchparams,
                	success: function(data) {
                    //alert(data);
                    	adminCalDestroyLoader()
	                    calendardata = JSON.parse(data);
	
    	                $newJquery('#calendarprimaryview').fullCalendar('removeEvents');
        	            $newJquery('#calendarnarrowview').fullCalendar('removeEvents');
						updateAdminCalendarEvents(calendardata);
                	   // $newJquery('#calendarprimaryview').fullCalendar('rerenderEvents');
                    	//$newJquery('#calendarnarrowview').fullCalendar('rerenderEvents');

	                    $newJquery("#calendarprimaryview  .fc-prev-button").attr("ajaxrequired", "false");
	                   // $newJquery("#calendarprimaryview").fullCalendar("changeView","month");

	    	            prevNarrowCalendarNavigation();

        	            attachEvents('#calendarprimaryview .fc-title a');
             	   },
                	failure:function(data)
	                {
    	            	adminCalDestroyLoader()
        	        }
            	});
            }
        } else
            $newJquery("#calendarnarrowview  .fc-prev-button").attr("ajaxrequired", "true");
    });

    $newJquery('#calendarprimaryview .fc-prev-button').click(function() {
        calendarallpopupclose();
   	 $("#calendarsummary .today-events, #calendarsummary .tmrw-events").fadeOut();
      //  if (!$(".fc-basicWeek-button").hasClass("fc-state-active") && !$(".fc-agendaDay-button").hasClass("fc-state-active")) 
      {
      
   
		 if($newJquery("#calendarprimaryview").fullCalendar('getView').name == "year")
		{
		    var startDate = $newJquery("#calendarnarrowview").fullCalendar('getView').intervalStart;
		    //startDate = startDate.add("1","month");
		    var newDate = parseInt(startDate.format("YYYY"),10)-1+"-"+startDate.format("MM-DD");
		    $newJquery("#calendarnarrowview").fullCalendar("gotoDate", newDate);
		    $newJquery("#calendarprimaryview").fullCalendar("gotoDate", newDate);
		    return false;


		}	  


            if ($newJquery("#calendarprimaryview  .fc-prev-button").attr("ajaxrequired") != "false") {
            	adminCalCreateLoader();
            	 var selDate = $newJquery('#calendarprimaryview').fullCalendar('getDate');
          	      	$newJquery('#calendarprimaryview').fullCalendar('removeEvents');
    	   			 $newJquery('#calendarnarrowview').fullCalendar('removeEvents');
    	   			


    //            var startDate = moment.format("YYYY-MM-DD");
     //           var endDate = moment.endOf('month').format("YYYY-MM-DD");
                
                var startDate = $newJquery("#calendarprimaryview").fullCalendar('getView').start.format("YYYY-MM-DD");
				var endDate = $newJquery("#calendarprimaryview").fullCalendar('getView').end.format("YYYY-MM-DD");

                //("startDate:" + startDate + " endDate:" + endDate);
                
                	/*if($.browser.msie && $.browser.version == 9)
					{
						callAjaxForIE('/?q=administration/calendar/settings',function(data)
						{
                		        adminCalDestroyLoader()
                        		calendardata = JSON.parse(data);

		                       prevNavigationDataProcessPrimary(calendardata);

                        		attachEvents('#calendarprimaryview .fc-title a');
						}
						);
    				}
    				else*/
    				{
    					var ac_searchparams = getACSearchParams();

                		$.ajax({
		                    url: "/?q=administration/calendarviewapi",
        	    	        METHOD: "post",
            	    	    data: "startDate=" + startDate + "&endDate=" + endDate+ac_searchparams,
		                    success: function(data) {
        		                //alert(data);
                		        adminCalDestroyLoader()
                        		calendardata = JSON.parse(data);

			                   prevNavigationDataProcessPrimary(calendardata);

                        		attachEvents('#calendarprimaryview .fc-title a');
                        	
                    			},
			                    failure:function(data)
            			        {
                    				adminCalDestroyLoader()
			                    }
            		    });
            		}
            } else {
                $newJquery("#calendarprimaryview  .fc-prev-button").attr("ajaxrequired", "true");
            }
        }
    });


    $newJquery('#calendarnarrowview .fc-next-button').click(function() {
        calendarallpopupclose();
    	$('.active-qtip-div').remove();	  
        
        
        if($newJquery("#calendarprimaryview").fullCalendar('getView').name == "year")
		{
		    var startDate = $newJquery("#calendarnarrowview").fullCalendar('getView').intervalStart;
		    startDate = startDate.add("-1","month");
		    var newDate = parseInt(startDate.format("YYYY"),10)+1+"-"+startDate.format("MM-DD");
		    $newJquery("#calendarnarrowview").fullCalendar("gotoDate", newDate);
		    $newJquery("#calendarprimaryview").fullCalendar("gotoDate", newDate);
		    return false;


		}	  
		


        if ($newJquery("#calendarnarrowview  .fc-next-button").attr("ajaxrequired") != "false") {
            //$newJquery("#calendarnarrowview  .fc-next-button").attr("ajaxrequired","true");            
            var moment = $newJquery('#calendarnarrowview').fullCalendar('getDate');
            //var startDate = moment.format("YYYY-MM-DD");
            var startDate = $newJquery("#calendarnarrowview").fullCalendar('getView').start.format("YYYY-MM-DD");
			var endDate = $newJquery("#calendarnarrowview").fullCalendar('getView').end.format("YYYY-MM-DD");

            //var endDate = moment.endOf('month').format("YYYY-MM-DD");
            //("startDate:" + startDate + " endDate:" + endDate);
			adminCalCreateLoader();
            
            /*if($.browser.msie && $.browser.version == 9)
			{
				callAjaxForIE('?q=administration/calendarviewapi?startDate=' + startDate + '&endDate=' + endDate,function(data)
				{
					
    				calendardata = JSON.parse(data);
                    adminCalDestroyLoader()
                    $newJquery('#calendarprimaryview').fullCalendar('removeEvents');
                    $newJquery('#calendarnarrowview').fullCalendar('removeEvents');

                   	updateAdminCalendarEvents(calendardata);
                    $newJquery('#calendarprimaryview').fullCalendar('rerenderEvents');
                    $newJquery('#calendarnarrowview').fullCalendar('rerenderEvents');
                    // 	// // // console.log("calendarnarrowview .fc-next-button custom:"+custom);
                    //  	alert(custom);
                    // if(custom == undefined)
                    //  if($newJquery("#calendarnarrowview  .fc-next-button").attr("ajaxrequired") != "false")
                    $newJquery("#calendarprimaryview  .fc-next-button").attr("ajaxrequired", "false");
//                    $newJquery("#calendarprimaryview").fullCalendar("changeView","month");

					nextNarrowCalendarNavigation();
                   // $newJquery("#calendarprimaryview  .fc-next-button").click();
                    //  else
                    //    $newJquery("#calendarnarrowview  .fc-next-button").attr("ajaxrequired","true");

                    attachEvents('#calendarprimaryview .fc-title a');
				}
				);
    		}
    		else*/
    		{
    		 var ac_searchparams = getACSearchParams();

            $.ajax({
                url: "/?q=administration/calendarviewapi",
                METHOD: "post",
                data: "startDate=" + startDate + "&endDate=" + endDate+ac_searchparams,
                success: function(data) {
                    calendardata = JSON.parse(data);
                    adminCalDestroyLoader()
                    
                    $newJquery('#calendarprimaryview').fullCalendar('removeEvents');
                    $newJquery('#calendarnarrowview').fullCalendar('removeEvents');

                   	updateAdminCalendarEvents(calendardata);
                  //  $newJquery('#calendarprimaryview').fullCalendar('rerenderEvents');
                  //  $newJquery('#calendarnarrowview').fullCalendar('rerenderEvents');
                    $newJquery("#calendarprimaryview  .fc-next-button").attr("ajaxrequired", "false");
//                    $newJquery("#calendarprimaryview").fullCalendar("changeView","month");
					
                    //$newJquery("#calendarprimaryview  .fc-next-button").click();
					nextNarrowCalendarNavigation();
                    attachEvents('#calendarprimaryview .fc-title a');
                }
            });
            }
        } else
            $newJquery("#calendarnarrowview  .fc-next-button").attr("ajaxrequired", "true");
    });

    $newJquery('#calendarprimaryview .fc-next-button').click(function() {
        var selDate = $newJquery('#calendarprimaryview').fullCalendar('getDate');
        calendarallpopupclose();		
        $("#calendarsummary .today-events, #calendarsummary .tmrw-events").fadeOut();
        if($newJquery("#calendarprimaryview").fullCalendar('getView').name == "year")
		{
		    var startDate = $newJquery("#calendarnarrowview").fullCalendar('getView').intervalStart;
		    //startDate = startDate.add("1","month");
		    var newDate = parseInt(startDate.format("YYYY"),10)+1+"-"+startDate.format("MM-DD");
		    $newJquery("#calendarnarrowview").fullCalendar("gotoDate", newDate);
		    $newJquery("#calendarprimaryview").fullCalendar("gotoDate", newDate);
		    return false;


		}	  
		
		var startDate = $newJquery("#calendarprimaryview").fullCalendar('getView').start.format("YYYY-MM-DD");
		
       // if (!$(".fc-basicWeek-button").hasClass("fc-state-active") && !$(".fc-agendaDay-button").hasClass("fc-state-active")) 
       {

            if ($newJquery("#calendarprimaryview  .fc-next-button").attr("ajaxrequired") != "false") {
                $newJquery('#calendarprimaryview').fullCalendar('removeEvents');
		    	$newJquery('#calendarnarrowview').fullCalendar('removeEvents');
		    	

		    	adminCalCreateLoader();

                //var endDate = moment.endOf('month').format("YYYY-MM-DD");
                var endDate = $newJquery("#calendarprimaryview").fullCalendar('getView').end.format("YYYY-MM-DD");

                //("startDate:" + startDate + " endDate:" + endDate);
                	/*if($.browser.msie && $.browser.version == 9)
					{
						callAjaxForIE('/?q=administration/calendarviewapi&startDate=' + startDate + "&endDate=" + endDate,function(data)
						{
		                        adminCalDestroyLoader()
        		                calendardata = JSON.parse(data);
	
                        nextNavigationDataProcessPrimary(calendardata);
                        attachEvents('#calendarprimaryview .fc-title a');
						}
						);
    				}
    				else*/
    				{
    				var ac_searchparams = getACSearchParams();

              		  $.ajax({
                    		url: "/?q=administration/calendarviewapi",
		                    METHOD: "post",
        		            data: "startDate=" + startDate + "&endDate=" + endDate+ac_searchparams,
                		    success: function(data) {
                        	//alert(data);
		                        adminCalDestroyLoader()
        		                calendardata = JSON.parse(data);
        		                
                   nextNavigationDataProcessPrimary(calendardata);

                        attachEvents('#calendarprimaryview .fc-title a');
                    }
                });
                }
            } else {
                $newJquery("#calendarprimaryview  .fc-next-button").attr("ajaxrequired", "true");
            }
        }
    });
});


function prevNavigationDataProcessPrimary(calendardata)
{
		var selectedDate = $newJquery('#calendarprimaryview').fullCalendar('getDate');
		var currentView = $newJquery('#calendarprimaryview').fullCalendar('getView').name;					
		$newJquery('#calendarprimaryview').fullCalendar('removeEvents');
        $newJquery('#calendarnarrowview').fullCalendar('removeEvents');
        var params = {'refreshMode': 'both'};
        updateAdminCalendarEvents(calendardata, params);
    //	$newJquery('#calendarprimaryview').fullCalendar('rerenderEvents');
     //   $newJquery('#calendarnarrowview').fullCalendar('rerenderEvents');
        
        	formatCalendarDataSummary(selectedDate.format("YYYY-MM-DD"),'',selectedDate);
        
        if (!$(".fc-basicWeek-button").hasClass("fc-state-active") && !$(".fc-agendaDay-button").hasClass("fc-state-active")) 
        {
    	    $newJquery("#calendarnarrowview  .fc-prev-button").attr("ajaxrequired", "false");
        	$newJquery("#calendarnarrowview  .fc-prev-button").click();

        }
        else
        {
        	var startDateMonthPrimary = $newJquery("#calendarprimaryview").fullCalendar('getView').intervalStart.format("MM");
    		var startDateMonthNarrow = $newJquery("#calendarnarrowview").fullCalendar('getView').intervalStart.format("MM");
			if((startDateMonthPrimary - startDateMonthNarrow) != 0)
			{
        		$newJquery("#calendarnarrowview  .fc-prev-button").attr("ajaxrequired", "false");
          	 	 $newJquery("#calendarnarrowview  .fc-prev-button").click();
			
			}
        }
        


	      
          /*  $newJquery("#calendarnarrowview  .fc-prev-button").attr("ajaxrequired", "false");
	        $newJquery("#calendarnarrowview  .fc-prev-button").click();
			*/
    attachEvents('#calendarprimaryview .fc-title a');
}

function nextNavigationDataProcessPrimary(calendardata)
{
						var selectedDate = $newJquery('#calendarprimaryview').fullCalendar('getDate');
						var currentView = $newJquery('#calendarprimaryview').fullCalendar('getView').name;
						$newJquery('#calendarprimaryview').fullCalendar('removeEvents');
                        $newJquery('#calendarnarrowview').fullCalendar('removeEvents');
                        var params = {'refreshMode': 'both'};
                       	updateAdminCalendarEvents(calendardata, params);
                  //      $newJquery('#calendarprimaryview').fullCalendar('rerenderEvents');
                   //     $newJquery('#calendarnarrowview').fullCalendar('rerenderEvents');
                        
                        	formatCalendarDataSummary(selectedDate.format("YYYY-MM-DD"),'',selectedDate);
                       
                        
                        if (!$(".fc-basicWeek-button").hasClass("fc-state-active") && !$(".fc-agendaDay-button").hasClass("fc-state-active")) 
						{
                        	$newJquery("#calendarnarrowview  .fc-next-button").attr("ajaxrequired", "false");
	                        $newJquery("#calendarnarrowview  .fc-next-button").click();
	                    }
	                    else
	                    {
	                    	var startDateMonthPrimary = $newJquery("#calendarprimaryview").fullCalendar('getView').intervalStart.format("MM");
	                    	var startDateMonthNarrow = $newJquery("#calendarnarrowview").fullCalendar('getView').intervalStart.format("MM");
							if((startDateMonthPrimary - startDateMonthNarrow) != 0)
							{
	                        	$newJquery("#calendarnarrowview  .fc-next-button").attr("ajaxrequired", "false");
		                        $newJquery("#calendarnarrowview  .fc-next-button").click();
								
							}
	                    	
	                    }
	                    

                        attachEvents('#calendarprimaryview .fc-title a');
}
function updateAdminCalendarEvents(calendardata, params)
{
	try {
		var refreshMode = (params != undefined && params.hasOwnProperty('refreshMode')) ? params.refreshMode : 'both'; 
		//console.log('refreshMode:' + refreshMode);
		if (refreshMode == 'primary') {
			$newJquery('#calendarprimaryview').fullCalendar('addEventSource', calendardata);  
			if($newJquery("#calendarprimaryview").fullCalendar('getView').name == 'agendaDay')
				applayJSScrollForDayView();
		} else if (refreshMode == 'narrow') {
			$newJquery('#calendarnarrowview').fullCalendar('addEventSource', calendardata);  
		} else {
			$newJquery('#calendarprimaryview').fullCalendar('addEventSource', calendardata);      
			$newJquery('#calendarnarrowview').fullCalendar('addEventSource', calendardata);
			if($newJquery("#calendarprimaryview").fullCalendar('getView').name == 'agendaDay')
				applayJSScrollForDayView();
        	
		}
	} catch(e) {
	}
}
function applayJSScrollForDayView(){
	$("#calendarprimaryview .fc-agendaDay-view .fc-scroller").jScrollPane();
	$("#calendarprimaryview .fc-agendaDay-view .fc-day-grid #fc-content-skeleton").jScrollPane();
}
function displayAdminCalendarSettingsHideShow() {
    calendarallpopupclose();	
	$('.active-qtip-div').remove();

    $("#qtipdiv").html(displayAdminCalendarSettings());
    var admincalendar_pref = $.cookie("admincalendar_pref");
    /*if (admincalendar_pref == null || admincalendar_pref == undefined) {
        admincalendar_pref = {
            "view_classroom": true,
            "view_virtualclassroom": true,
            "view_announcements": true,
            "view_events": true,
            "view_orders": true,
            "view_scheduled_report": true,
            "view_locations": true,
            "view_discussions": true,
            "view_instructors":true
        };
        $.cookie("admincalendar_pref", JSON.stringify(admincalendar_pref));

    }*/
    admincalendar_pref = JSON.parse(admincalendar_pref);
    if(admincalendar_pref.view_orders == false)
    	$("#view_orders").prop("checked",false);
    else
    	$("#view_orders").prop("checked",true);
    	
     if(admincalendar_pref.view_locations == false)
    	$("#view_locations").prop("checked",false);
    else
    	$("#view_locations").prop("checked",true);
    	
	 if(admincalendar_pref.view_discussions == false)
    	$("#view_discussions").prop("checked",false);
    else 
    	$("#view_discussions").prop("checked",true);
    	
   /* if(admincalendar_pref.view_events == false)
    	$("#view_events").prop("checked",false);
    else
    	$("#view_events").prop("checked",true); */
    	
    if(admincalendar_pref.view_instructors == false)
    	$("#view_instructors").prop("checked",false);
    else
    	$("#view_instructors").prop("checked",true);
    	
    if(admincalendar_pref.view_classroom == false)
    	$("#view_classroom").prop("checked",false);
    else
    	$("#view_classroom").prop("checked",true);
    	
    if(admincalendar_pref.view_virtualclassroom == false)
    	$("#view_virtualclassroom").prop("checked",false);
    else
    	$("#view_virtualclassroom").prop("checked",true);
    
    /*if(admincalendar_pref.view_announcements == false)
    	$("#view_announcements").prop("checked",false);
    else
    	$("#view_announcements").prop("checked",true);*/
    
    
    
    	
    	
    	
    	
	$('#admincalendarsettingdiv').show();
	//closePopupOnEscAndOutsideClick("#admincalendarsettingdiv");
	
    /*try {
        if (Drupal.settings.ajaxPageState.theme == "expertusoneV2") {
            $('#qtipdiv').show();
        } else {
            $('#qtipdiv').toggle();
        }
        closePopupOnEscAndOutsideClick("#qtipdiv");
    } catch (e) {
        // to do
    }*/

}

function getAndDisplayMonthData(startDate, endDate) {}

function displayAdminCalendarSettingsData(result)
{
	if (result == 'session_out') {
				self.location='?q=user';
				return;
			}
			
			var admincalendar_pref = $.cookie("admincalendar_pref");
		    admincalendar_pref = JSON.parse(admincalendar_pref);
		    ADMIN_CALENDAR_PERM_LIST = result;
		    if((ADMIN_CALENDAR_PERM_LIST.view_orders == 'enabled') && (ADMIN_CALENDAR_PERM_LIST.commerce == false)){
		    	ADMIN_CALENDAR_PERM_LIST.view_orders = 'disabled';
		    }

		    if((ADMIN_CALENDAR_PERM_LIST.view_discussions == 'enabled') && (ADMIN_CALENDAR_PERM_LIST.forum == false)){
		    	ADMIN_CALENDAR_PERM_LIST.view_discussions = 'disabled';
		    }
		    
		    var viewclassroomchked 			= (ADMIN_CALENDAR_PERM_LIST.view_classroom == 'enabled') ? "checked=checked" :  "disabled=disabled"; 
		    var view_vc_classroomchked 		= (ADMIN_CALENDAR_PERM_LIST.view_virtualclassroom == 'enabled') ? "checked=checked" :  "disabled=disabled"; 
		    //var view_announcementschked 	= (ADMIN_CALENDAR_PERM_LIST.view_announcements == 'enabled') ? "checked=checked" :  "disabled=disabled"; 
		    //var view_eventtypeschked 		= (ADMIN_CALENDAR_PERM_LIST.view_events == 'enabled') ? "checked=checked" :  "disabled=disabled"; 
		    var view_orderschked 			= (ADMIN_CALENDAR_PERM_LIST.view_orders == 'enabled') ? "checked=checked" :  "disabled=disabled"; 
		    var view_sch_reportchked 		= (ADMIN_CALENDAR_PERM_LIST.view_scheduled_report == 'enabled') ? "checked=checked" :  "disabled=disabled"; 
		    var view_locations_chked 		= (ADMIN_CALENDAR_PERM_LIST.view_locations == 'enabled') ? "checked=checked" :  "disabled=disabled"; 
		    var view_discussions_chked 		= (ADMIN_CALENDAR_PERM_LIST.view_discussions == 'enabled') ? "checked=checked" :  "disabled=disabled"; 
		    var view_instructors_chked 		= (ADMIN_CALENDAR_PERM_LIST.view_instructors == 'enabled') ? "checked=checked" :  "disabled=disabled"; 
		    
		    if (ADMIN_CALENDAR_PERM_LIST.view_classroom == 'enabled' && admincalendar_pref != null && admincalendar_pref.view_classroom == false)
		    	viewclassroomchked = "";
		    if (ADMIN_CALENDAR_PERM_LIST.view_virtualclassroom == 'enabled' &&  admincalendar_pref != null && admincalendar_pref.view_virtualclassroom == false)
		        view_vc_classroomchked = "";
		    /*if (ADMIN_CALENDAR_PERM_LIST.view_announcements == 'enabled' &&  admincalendar_pref != null && admincalendar_pref.view_announcements == false)
		       view_announcementschked = "";
		    if (ADMIN_CALENDAR_PERM_LIST.view_events == 'enabled' &&  admincalendar_pref != null && admincalendar_pref.view_events == false)
		        view_eventtypeschked = "";*/
		    if (ADMIN_CALENDAR_PERM_LIST.view_orders == 'enabled' &&  admincalendar_pref != null && admincalendar_pref.view_orders == false)
		        view_orderschked = "";
		    if (ADMIN_CALENDAR_PERM_LIST.view_scheduled_report == 'enabled' &&  admincalendar_pref != null && admincalendar_pref.view_scheduled_report == false)
		        view_sch_reportchked = "";
		    if (ADMIN_CALENDAR_PERM_LIST.view_locations == 'enabled' &&  admincalendar_pref != null && admincalendar_pref.view_locations == false)
		        view_locations_chked = "";
		    if (ADMIN_CALENDAR_PERM_LIST.view_discussions == 'enabled' &&  admincalendar_pref != null && admincalendar_pref.view_discussions == false)
		        view_discussions_chked = "";
		    if (ADMIN_CALENDAR_PERM_LIST.view_instructors == 'enabled' &&  admincalendar_pref != null && admincalendar_pref.view_instructors == false)
		        view_instructors_chked = "";
		    

							var lang = getCurrentLanguage();
       var langCls = "";
       if(lang == "ru")
           langCls = "lang-ru";

		    var html = '<div id="admincalendarsettingdiv" class="'+langCls+'" style="display:block;">';
		    html += '<div class="dropdownadd-dd-list-arrow"></div>';
		    html += '<a  class="qtip-close-button account-close-popup"></a>';
		    html += '<table cellspacing="0" cellpadding="0" id="bubble-face-table">';
		    html += '<tbody>';
		    html += '<tr>';
		    html += '<td class="bubble-tl"></td>';
		    html += '<td class="bubble-t"></td>';
		    html += '<td class="bubble-tr"></td>';
		    html += '</tr>';
		    html += '<tr>';
		    html += '<td class="bubble-cl"></td>';
		    html += '<td valign="top" class="bubble-c">';
		    
		    html += '<div class="links header-popup '+ADMIN_CALENDAR_PERM_LIST.view_classroom+'"><span class="vtip" title="'+Drupal.t("ACLBL0011")+'">'+Drupal.t("ACLBL0011")+'<span class="fadeout"></span></span>';
		    html += '</div><span class="form-checkbox"><input  class="eventtypes" type="checkbox" id="view_classroom" ' + viewclassroomchked + '></input></span>';
		    html += ' <div class="links header-popup '+ADMIN_CALENDAR_PERM_LIST.view_virtualclassroom+'"><span class="vtip" title="'+Drupal.t("ACLBL0012")+'">'+Drupal.t("ACLBL0012")+'<span class="fadeout"></span></span>';
		    html += ' </div><span class="form-checkbox"><input  class="eventtypes" type="checkbox"  id="view_virtualclassroom" ' + view_vc_classroomchked + '></input></span>';
		   
		   /* Added/Changed by ganesh on May 9th 2017 for #73716 
		    
		    html += ' <div class="links header-popup '+ADMIN_CALENDAR_PERM_LIST.view_events+'"><span class="vtip" title="'+Drupal.t("LBL816")+' '+Drupal.t("LBL565")+'">'+Drupal.t("LBL816")+' '+Drupal.t("LBL565")+'<span class="fadeout"></span></span>';
		    html += ' </div><span class="form-checkbox"><input type="checkbox" id="view_events"   class="eventtypes" ' + view_eventtypeschked + '></input></span>';*/
		   
		    /*html += ' <div class="links header-popup '+ADMIN_CALENDAR_PERM_LIST.view_announcements+'">'+Drupal.t("ACLBL0013");
		    html += ' </div><span class="form-checkbox"><input type="checkbox"  class="eventtypes" id="view_announcements" ' + view_announcementschked + '></input></span>';*/
		    html += ' <div class="links header-popup '+ADMIN_CALENDAR_PERM_LIST.view_orders+'"><span class="vtip" title="'+Drupal.t("ACLBL0015")+'">'+Drupal.t("ACLBL0015")+'<span class="fadeout"></span></span>';
		    html += ' </div><span class="form-checkbox"><input type="checkbox" id="view_orders" ' + view_orderschked + ' class="eventtypes"></input></span>';
		    html += ' <div class="links header-popup '+ADMIN_CALENDAR_PERM_LIST.view_scheduled_report+'"><span class="vtip" title="'+Drupal.t("ACLBL0016")+'">'+Drupal.t("ACLBL0016")+'<span class="fadeout"></span></span>';
		    html += ' </div><span class="form-checkbox"><input type="checkbox" id="view_scheduled_report" ' + view_sch_reportchked + ' class="eventtypes"></input></span>';
		    html += ' <div class="links header-popup '+ADMIN_CALENDAR_PERM_LIST.view_locations+'"><span class="vtip" title="'+Drupal.t("ACLBL0017")+'">'+Drupal.t("ACLBL0017")+'<span class="fadeout"></span></span>';
		    html += ' </div><span class="form-checkbox"><input type="checkbox" id="view_locations" ' + view_locations_chked + ' class="eventtypes"></input></span>';
		    html += ' <div class="links header-popup '+ADMIN_CALENDAR_PERM_LIST.view_instructors+'"><span class="vtip" title="'+Drupal.t("ACLBL0019")+'">'+Drupal.t("ACLBL0019")+'<span class="fadeout"></span></span>';
		    html += ' </div><span class="form-checkbox"><input type="checkbox" id="view_instructors" ' + view_instructors_chked + ' class="eventtypes"></input></span>';
		    html += ' <div class="links header-popup '+ADMIN_CALENDAR_PERM_LIST.view_discussions+'"><span class="vtip" title="'+Drupal.t("ACLBL0018")+'">'+Drupal.t("ACLBL0018")+'<span class="fadeout"></span></span>';
		    html += ' </div><span class="form-checkbox"><input type="checkbox" id="view_discussions" ' + view_discussions_chked + ' class="eventtypes"></input></span>';
		    
		    html += ' <div class="action-btn-container"><div class="white-btn-bg-container"><div class="white-btn-bg-left"></div><input class="white-btn-bg-middle close learner-sign-close-link"  type="button" id="edit-close"  onclick="calendarallpopupclose()"  name="op" value="'+Drupal.t("LBL123")+'"><div class="white-btn-bg-right"></div></div>';
		    html += ' <div class="single-admin-save-button-container"><div class="admin-save-button-left-bg"></div><input class="admin-save-button-middle-bg"  onclick="adminCalendarSettingPref()" type="button" id="edit-submit" name="op" value="'+Drupal.t("LBL141")+'"><div class="admin-save-button-right-bg"></div></div></div>';

		    html += ' </td>';
		    html += ' <td class="bubble-cr"></td>';
		    html += '</tr>';
		    html += '<tr>';
		    html += ' <td class="bubble-bl"></td>';
		    html += ' <td class="bubble-blt"></td>';
		    html += ' <td class="bubble-br"></td>';
		    html += '</tr>';
		    html += '</tbody>';
		    html += '</table>';
		    html += '</div>';
		    
		    $("#qtipdiv").html(html);
		    vtip();
}
function displayAdminCalendarSettings() {
	try {
	$("#admincalendarsettingdiv").remove();
	adminCalCreateLoader();
	/*if($.browser.msie && $.browser.version == 9)
	{
		callAjaxForIE('/?q=administration/calendar/settings',function(result)
		{
			adminCalDestroyLoader()
			displayAdminCalendarSettingsData(result);
		}
		);
	}
	else*/
	{
		$.ajax({
			type: "GET",
			url: '/?q=administration/calendar/settings',
			success: function(result){
				adminCalDestroyLoader()
				displayAdminCalendarSettingsData(result);
		
			},
			failure:function(data)
			{
				adminCalDestroyLoader()
			}
		});
	}
	} catch(e) {
		// console.log(e);
	}
}

function displayShortTitle(title) {
    var shortTitle = "";
    if (title.length > 22)
        shortTitle = title.substring(0, 22) + "...";
    else
        shortTitle = title;
    return shortTitle;
}

function calendarsummarydot(date) {
    return "<div class='link cal-sum-dot' href='javascript:void(0);' selecteddate='" + date + "' onclick='displayMorePopupFromCalendarSummary(this);'><span class=\"vtip\" title='"+Drupal.t('LBL713')+"'>...</span></div>";
}

function iterateEventsObject(records, day, cls,moreData, selectedDate) {
    var events = "";
    var counter = 0;
    if (records == "" || records == undefined)
        return "";

    cls = "ilt-cls";


	var admincalendar_pref = $.cookie("admincalendar_pref");
    /*if (admincalendar_pref == null || admincalendar_pref == undefined) {
        admincalendar_pref = {
            "view_classroom": true,
            "view_virtualclassroom": true,
            "view_announcements": true,
            "view_events": true,
            "view_orders": true,
            "view_scheduled_report": true,
            "view_locations": true,
            "view_discussions": true,
            "view_instructors": true
        };
        $.cookie("admincalendar_pref", JSON.stringify(admincalendar_pref));
    }*/

	try
	{
    admincalendar_pref = JSON.parse(admincalendar_pref);
    }catch(e){
     /*admincalendar_pref = {
            "view_classroom": true,
            "view_virtualclassroom": true,
            "view_announcements": true,
            "view_events": true,
            "view_orders": true,
            "view_scheduled_report": true,
            "view_locations": true,
            "view_discussions": true,
            "view_instructors": true
        };*/
    }
    if(moreData == undefined || moreData == null || moreData == 'undefined')
    {
    	moreData = false;
    }
    else
    {
//    	$("#today-events-container, #tomo-events-container").css("max-height","200px");
//    	$("#today-events-container, #tomo-events-container").jScrollPane({
//        	width: "1px"
//    	});
    }
    for(var i = 0 ; i < records.length; i++)
	{
		
    	if(admincalendar_pref.view_classroom == true && records[i].type == "lrn_cls_dty_ilt")
		{
			cls = "ilt-cls";
			var eventStrTime = '';
			if(records[i].allDay == false)
				notalldaycls = cls + " notallday";
			else
				notalldaycls = cls;
			
			if(records[i].startTime != undefined || records[i].startTime != '')
				eventStrTime = calTimeFormat(records[i].from_date,records[i].startTime,false,'summary');
				
			shortTitle = '<span class="list-spot"></span><span class="time">' + eventStrTime + '</span><span class="summary-title">' + displayShortTitle(records[i].title) + '</span>';
	        
		    if (isEventFallInTodayDate(records[i], day.substring(0, 10))) {
        	    events += "<div class='list-item' data=\"" + escape(JSON.stringify(records[i])) + "\" onclick='showSessionDetailsFromCalendarSummary(this,event);'><span title='"+ eventStrTime + ' ' + records[i].title +"' class='vtip event-item " + notalldaycls + "'>" + shortTitle + "</span></div>";
            	counter = counter + 1;
	            /*if (counter > 4 && moreData == false) {
    	            events += calendarsummarydot(selectedDate.substring(0, 10));
        	        return events;
            	}*/
        	}
		}
		if(admincalendar_pref.view_virtualclassroom == true  && records[i].type == "lrn_cls_dty_vcl")
		{
			cls = "vcl-cls";
			var eventStrTime = '';
			if(records[i].allDay == false)
				notalldaycls = cls + " notallday";
			else
				notalldaycls = cls;
				
			if(records[i].startTime != undefined || records[i].startTime != '')
				eventStrTime = calTimeFormat(records[i].from_date,records[i].startTime,false,'summary');
			
			shortTitle = '<span class="list-spot"></span><span class="time">' + eventStrTime + '</span><span class="summary-title">' + displayShortTitle(records[i].title) + '</span>';
        	
		    if (isEventFallInTodayDate(records[i], day.substring(0, 10))) {
    	        events += "<div class='list-item'  data=\"" + escape(JSON.stringify(records[i])) + "\" onclick='showSessionDetailsFromCalendarSummary(this,event);'><span title='"+ eventStrTime + ' ' + records[i].title +"' class='vtip event-item " + notalldaycls + "'>" + shortTitle + "</span></div>";
        	    counter++;
            	/*if (counter > 4 && moreData == false) {
                	events += calendarsummarydot(selectedDate.substring(0, 10));
	                return events;
    	        }*/
        
    		}

		}

	
    if(admincalendar_pref.view_orders == true && records[i].type == "order")
	{
     	cls = "order";
     	var eventStrTime = '';
		if(records[i].allDay == false)
			notalldaycls = cls + " notallday";
		else
			notalldaycls = cls;
		
		if(records[i].startTime != undefined || records[i].startTime != '')
    		eventStrTime = calTimeFormat(records[i].from_date,records[i].startTime,false,'summary');
		
    	shortTitle = '<span class="list-spot"></span><span class="time">' + eventStrTime + '</span><span class="summary-title">' + displayShortTitle(records[i].title) + '</span>';
        	
	    if (isEventFallInTodayDate(records[i], day.substring(0, 10))) {
    	        events += "<div class='list-item'  data=\"" + escape(JSON.stringify(records[i])) + "\" onclick='showSessionDetailsFromCalendarSummary(this,event);'><span title='"+ eventStrTime + ' ' +records[i].title +"' class='vtip event-item " + notalldaycls + "'>" + shortTitle + "</span></div>";
        	    counter++;
            	/*if (counter > 4  && moreData == false) {
                	events += calendarsummarydot(selectedDate.substring(0, 10));
	                return events;
    	        }*/
    	}
    }
    if(admincalendar_pref.view_scheduled_report == true && records[i].type == "report")
	{
     	cls = "report";
     	var eventStrTime = '';
		if(records[i].allDay == false)
			notalldaycls = cls + " notallday";
		else
			notalldaycls = cls;
			
		if(records[i].startTime != undefined || records[i].startTime != '')
    		eventStrTime = calTimeFormat(records[i].from_date,records[i].startTime,false,'summary');
		
    	shortTitle = '<span class="list-spot"></span><span class="time">' + eventStrTime + '</span><span class="summary-title">' + displayShortTitle(records[i].title) + '</span>';
        	
	       // if (records[i].start.substring(0, 10) == day.substring(0, 10)) {
    	if (isEventFallInTodayDate(records[i], day.substring(0, 10))) {
    	        events += "<div class='list-item'  data=\"" + escape(JSON.stringify(records[i])) + "\" onclick='showSessionDetailsFromCalendarSummary(this,event);'><span title=\""+ eventStrTime + ' ' + records[i].title +"\" class='vtip event-item " + notalldaycls + "'>" + shortTitle + "</span></div>";
        	    counter++;
            /*if (counter > 4  && moreData == false) {
                events += calendarsummarydot(selectedDate.substring(0, 10));
                return events;
            }*/
        }
    }
  }

    /*
    
    for (var i = 0; i < records.length; i++) {
        // // // console.log(records[i].start + " == " + day);
var shortTitle = "";
               if(records[i].title.length  > 20)
                       shortTitle = records[i].title.substring(0,20)+"...";
               else
                       shortTitle =records[i].title;

var notalldaycls = "";

               if(cls == "ilt-cls") {
                      shortTitle = "<span></span>"+shortTitle;
                          notalldaycls = cls+ " notallday";
               }
               else {
                     notalldaycls = cls;
               }
              // alert(records[i].start.substring(0,10)+" == "+day);

        	if (records[i].start.substring(0,10) == day.substring(0,10))
	           events += "<div class='list-item'><span class='event-item " +notalldaycls+ "'>" +shortTitle + "</span><span class='time'>"+records[i].startTime +"</span></div>";
    }*/
    
    return events;
}

function isEventFallInTodayDate(event,todayDate)
{
	var momentToday = moment(todayDate);
	var momentEventStart = moment(event.start.substring(0, 10));
	var momentEventEnd = "";
	
	var dayOftheWeek = moment(momentToday).weekday();
	if(momentEventStart.diff(momentToday,'days') == 0)
	{
		return true;
	 }
	 
	if(event.end != undefined && event.end != null && event.end != "undefined" && event.end != "")
	{			
		momentEventEnd = moment(event.end.substring(0, 10));
	}
	
	// console.log(event.title+" = "+event.dow+"==="+dayOftheWeek);
	if(momentToday >= momentEventStart && momentToday <= momentEventEnd)
	{
		if(event.dow != undefined)
		{
			if(event.dow.indexOf(dayOftheWeek) >= 0)
				return true;
		}
		else
			return true;		
	}
	else
		return false;
}




if( typeof helper == 'undefined' ) {
	  var helper = { } ;
	}

	helper.arr = {
	         /**
	     * Function to sort multidimensional array
	     * 
	     * <a href="/param">@param</a> {array} [arr] Source array
	     * <a href="/param">@param</a> {array} [columns] List of columns to sort
	     * <a href="/param">@param</a> {array} [order_by] List of directions (ASC, DESC)
	     * @returns {array}
	     */
	    multisort: function(arr, columns, order_by) {
	        if(typeof columns == 'undefined') {
	            columns = []
	            for(x=0;x<arr[0].length;x++) {
	                columns.push(x);
	            }
	        }

	        if(typeof order_by == 'undefined') {
	            order_by = []
	            for(x=0;x<arr[0].length;x++) {
	                order_by.push('ASC');
	            }
	        }

	        function multisort_recursive(a,b,columns,order_by,index) {  
	            var direction = order_by[index] == 'DESC' ? 1 : 0;

	            var is_numeric = !isNaN(+a[columns[index]] - +b[columns[index]]);


	            var x = is_numeric ? +a[columns[index]] : a[columns[index]].toLowerCase();
	            var y = is_numeric ? +b[columns[index]] : b[columns[index]].toLowerCase();



	            if(x < y) {
	                    return direction == 0 ? -1 : 1;
	            }

	            if(x == y)  {               
	                return columns.length-1 > index ? multisort_recursive(a,b,columns,order_by,index+1) : 0;
	            }

	            return direction == 0 ? 1 : -1;
	        }

	        return arr.sort(function (a,b) {
	            return multisort_recursive(a,b,columns,order_by,0);
	        });
	    }
	};


function formatCalendarDataSummary(day,isMoreData, selectedDate) {
	checkSessionOut();
    var selDateObject 		= moment(selectedDate);
    var selDateObjTmw 		= moment(selectedDate);
    var tomwday = selDateObjTmw.add("days", 1).format('YYYY-MM-DD');
    var ac_searchparams = getACSearchParams();

    var cltoDaycalendardata = $newJquery('#calendarprimaryview').fullCalendar('getEventSources')
    var cltodayEvents = iterateEventsObject(cltoDaycalendardata, day,'',isMoreData, selectedDate);
	var cltmrwEvents = iterateEventsObject(cltoDaycalendardata,tomwday, '',isMoreData, selectedDate);
    
    //if (cltodayEvents.length > 0 || cltmrwEvents.length > 0)
    $("#calendarsummary").css("display", "block");
	
    var currentDateObject 	= '';
    // today calc
    if(calendardata != null) {
 	   currentDateObject = moment(calendarmetadata[0].todaydate).format('YYYY-MM-DD');
    } else {
 	   currentDateObject = moment(new Date()).format('YYYY-MM-DD');
    }
    var dayDifferent		= selDateObject.diff(currentDateObject, 'days');
    var todayLabel = '';
    var tomoLabel = '';
    
    if (dayDifferent == 0) {
 	   todayLabel 	= Drupal.t('LBL031'); //'Today';
 	   tomoLabel 	= Drupal.t('ACLBL0020'); //'Tomorrow';
    } else if (dayDifferent == 1) {
 	   todayLabel = Drupal.t('ACLBL0020'); //'Tomorrow';
 	   tomoLabel = selDateObject.add(1, 'day').format('MMM DD, YYYY');
    } else if (dayDifferent == -1) {
 	   todayLabel = selDateObject.format('MMM DD, YYYY');
 	   tomoLabel = Drupal.t('LBL031'); //'Today';
    } else {
 	   todayLabel = selDateObject.format('MMM DD, YYYY');
 	   tomoLabel  = selDateObject.add(1, 'day').format('MMM DD, YYYY');
    }
    cltodayEvents = "<div class='today-events'><h4>"+todayLabel+"</h4><div id='today-events-container'>" + cltodayEvents + "</div><div id ='tdyoverflowitems' class ='tdyoverflowitems'></div></div>";
    cltmrwEvents = "<div class='tmrw-events' ><h4>"+tomoLabel+"</h4><div id='tomo-events-container'>" + cltmrwEvents + "</div><div id ='tmwoverflowitems' class ='tmwoverflowitems'></div></div>";
    $("#calendarsummary").html(cltodayEvents + cltmrwEvents);
    var settings = {
		autoReinitialise: true
	};
    $("#calendarsummary #today-events-container").jScrollPane(settings);
    $("#calendarsummary #tomo-events-container").jScrollPane(settings);
    
    if($('#tdyoverflowitems').hasClass('tdyoverflowitems')){
    	var tdyHeg = $("#today-events-container").height() - 25;
    	$('#calendarsummary #today-events-container .jspVerticalBar').css('height',tdyHeg);
    }
    if($('#tmwoverflowitems').hasClass('tmwoverflowitems')){
    	var tmrHeg = $("#tomo-events-container").height() - 25;
    	$('#calendarsummary #tomo-events-container .jspVerticalBar').css('height',tmrHeg);
    }
   /* var hit = $('.attr-selected-pane').height() - 2;
	$('.attr-selected-pane .jspVerticalBar').css('height',hit);*/
    vtip();
   // $('#today-events-container').scroll(function(){
    	if($('#tdyoverflowitems').hasClass('tdyoverflowitems')){
    		$('#tdyoverflowitems').removeClass('tdyoverflowitems');
    		formatCompleteCalendarDataSummary(day,todayLabel,'today');
    	}
   // });
    
  //  $('#tomo-events-container').scroll(function(){
    	if($('#tmwoverflowitems').hasClass('tmwoverflowitems')){
    		$('#tmwoverflowitems').removeClass('tmwoverflowitems');
    		formatCompleteCalendarDataSummary(tomwday,tomoLabel,'tomorrow');
    	}
  //  });
}

function formatCompleteCalendarDataSummary(day,dateLbl,fromScroll) {
	checkSessionOut();
	if(fromScroll == 'today'){
		cltodayEvents = "<h4>"+dateLbl+"</h4><div id='today-events-container'></div><div id ='tdyoverflowitems' class ='tdyoverflowitems'></div>";
		$("#calendarsummary .today-events").html(cltodayEvents);
		adminCalCreateLoader('today-events-container');
	}
	else{
		cltomoEvents = "<h4>"+dateLbl+"</h4><div id='tomo-events-container'></div><div id ='tmwoverflowitems' class ='tmwoverflowitems'></div>";
		$("#calendarsummary .tmrw-events").html(cltomoEvents);
		adminCalCreateLoader('tomo-events-container');
	}
    var ac_searchparams = getACSearchParams();
    $.ajax({
        url: "/?q=administration/calendarviewapi",
        METHOD: "post",
        data: "startDate=" + day + "&endDate=" + day+ac_searchparams+"&narrowcaldaywise=true",
        success: function(data) {
        	if(fromScroll == 'today')
        		adminCalDestroyLoader('today-events-container');
        	else
        		adminCalDestroyLoader('tomo-events-container');
	        var cltoDaycalendardata = JSON.parse(data);
	        var cltoDaycalendardata = helper.arr.multisort(cltoDaycalendardata, ['startTime', 'title'], ['ASC']);
	        var cltodayEvents = iterateEventsObject(cltoDaycalendardata, day,'','', day);
	        var settings = {
				autoReinitialise: true
			};
            if(fromScroll == 'today'){
            	$("#calendarsummary #today-events-container").html(cltodayEvents);
            	$("#calendarsummary #today-events-container").jScrollPane(settings);
            } else {
            	$("#calendarsummary #tomo-events-container").html(cltodayEvents);
            	$("#calendarsummary #tomo-events-container").jScrollPane(settings);
            }
            
            vtip();
         },
         failure:function(data)
	     {
         	if(fromScroll == 'today')
        		adminCalDestroyLoader('today-events-container');
        	else
        		adminCalDestroyLoader('tomo-events-container');

         }
    });
}

function formatCalendarSummary(day,isMoreData, selectedDate) {
    var todayEvents = "";
	if(calendardata.length != null && calendardata.length != undefined)
	{
		calendardata = calendardata.sort(ac_events_sorting);
	}

    todayEvents = iterateEventsObject(calendardata, day,'',isMoreData, selectedDate);
    /*   todayEvents = iterateEventsObject(calendardata.classrooms, day, "ilt-cls");
       todayEvents += iterateEventsObject(calendardata.vcrooms, day, "vcl-cls");
       todayEvents += iterateEventsObject(calendardata.announcements, day, "announcement");
      
       todayEvents += iterateEventsObject(calendardata.orders, day, "order");
       todayEvents += iterateEventsObject(calendardata.events, day, "regend regcancel"); */
    return todayEvents;
}


function printAdminCalendar() {
	$('.active-qtip-div').remove();	
	calendarallpopupclose();
    printPreview();
    //window.print();
}

function exportAdminCalendar() {
	$('.active-qtip-div').remove();	
	calendarallpopupclose();
    //   window.open('data:application/vnd.ms-excel,' + $('#calendar').html());
    if($newJquery('#calendarprimaryview').fullCalendar('getView').name == "year")
    {
    	adminCalendarPopup("Alert","Export is disabled for year view");
		return;
    }
    var startDate =  $newJquery('#calendarprimaryview').fullCalendar('getView').start.format("YYYY-MM-DD");
	var endDate =    $newJquery('#calendarprimaryview').fullCalendar('getView').end.format("YYYY-MM-DD");


    calendarExcelConvert(startDate, endDate);
}

function adminCalendarSettingClose() {
    try {
        $('#admincalendarsettingdiv').css('display', 'none');
    } catch (e) {
        // to do
    }
}
function reArangeSearchFilter(){
	  var search_selected = $('#search_all_calendar_type-hidden').val();
	  var admincalendar_pref =jQuery.parseJSON($.cookie('admincalendar_pref'));
	  //console.log("reArangeSearchFilter:"+admincalendar_pref);
	  var filter_arr =new Array();

	   filter_arr['All'] =Drupal.t('LBL1039');
	
	  if(admincalendar_pref.view_classroom == true || admincalendar_pref.view_classroom == "true" || admincalendar_pref.view_virtualclassroom == true || admincalendar_pref.view_virtualclassroom == "true")
	  	filter_arr['view_classroom'] = Drupal.t('LBL262'); //Added/Changed by ganesh on May 9th 2017 for #73716
	  /*if(admincalendar_pref.view_announcements == true || admincalendar_pref.view_announcements == "true")
	  	filter_arr['view_announcements'] =Drupal.t('LBL196');*/
	  	//Added for #0064978 by vetrivel.P
	  if(admincalendar_pref.view_instructors == true || admincalendar_pref.view_instructors == "true")
	  	filter_arr['view_instructors'] =Drupal.t('Instructor'); //Added/Changed by ganesh on May 9th 2017 for #73716
	  if(admincalendar_pref.view_locations == true || admincalendar_pref.view_locations == "true")
		filter_arr['view_locations'] =Drupal.t('Location');
	  if( ADMIN_CALENDAR_PERM_LIST.commerce == true && (admincalendar_pref.view_orders == true || admincalendar_pref.view_orders == "true"))
	  	filter_arr['view_orders'] =Drupal.t('ACLBL0041'); //Added/Changed by ganesh on May 9th 2017 for #73716
	  if(admincalendar_pref.view_scheduled_report == true || admincalendar_pref.view_scheduled_report == "true")
	  	filter_arr['view_scheduled_report'] =Drupal.t('LBL799'); //Added/Changed by ganesh on May 9th 2017 for #73716
	  	
	  /*if(admincalendar_pref.view_events == true || admincalendar_pref.view_events == "true")
	  	filter_arr['view_events'] =Drupal.t('LBL565');*/ //Added/Changed by ganesh on May 9th 2017 for #73716
	  
	  	
	  var need_change_filter = false;
	  if(search_selected in filter_arr){}
	  else
		  need_change_filter = true;
	  //console.log(need_change_filter);
	  var html = '';
	  var i =0;
	  var lang = getCurrentLanguage();
	  for (var key in filter_arr)
	  {
		if(i==0 && need_change_filter == true)
		{
			$('#select-list-calendar-dropdown').css("display","block");
			$("#admin-dropdown-arrow").css("display","block");
			
			$('#search_all_calendar_type-hidden').val(key);
			/*if(filter_arr[key].length > 7)
				$('#select-list-calendar-dropdown').html(filter_arr[key]); 
			else*/
				$('#select-list-calendar-dropdown').html(filter_arr[key]);
		}
		i++;
		html +='<li onclick="moreClassSearchTypeText(\''+escape(filter_arr[key])+'\',\''+key+'\');" class="'+key+'">'+ filter_arr[key]+'</li>';
	}
	if( i == 0)
	{
		$('#select-list-calendar-dropdown').css("display","none");
		$("#admin-dropdown-arrow").css("display","none");
		$('#search_all_calendar_type-hidden').val("");
	}
	
	  $('#select-list-calendar-dropdown-list').html(html);
}	  
function adminCalendarSettingPref() {
	adminCalCreateLoader();
	calendarallpopupclose();
	//console.log("test value " +$("#view_classroom").prop("checked"))
	if(($("#view_classroom").prop("checked") == false) && ($("#view_virtualclassroom").prop("checked") == false) &&
		($("#view_orders").prop("checked") == false) && ($("#view_scheduled_report").prop("checked") == false) &&
		($("#view_locations").prop("checked") == false) && ($("#view_discussions").prop("checked") == false) && ($("#view_instructors").prop("checked") == false))
		{
		adminCalendarPopup("Alert",Drupal.t("ERR106"));
		adminCalDestroyLoader();
		return;
		}
	
    var calendar_pref = {
        "view_classroom": $("#view_classroom").prop("checked"),
        "view_virtualclassroom": $("#view_virtualclassroom").prop("checked"),
        //"view_announcements": $("#view_announcements").prop("checked"),
        //"view_events": $("#view_events").prop("checked"),
        "view_orders": $("#view_orders").prop("checked"),
        "view_scheduled_report": $("#view_scheduled_report").prop("checked"),
        "view_locations": $("#view_locations").prop("checked"),
        "view_discussions": $("#view_discussions").prop("checked"),
        "view_instructors":$("#view_instructors").prop("checked")
        
    };
    // console.log("admincalendar_pref="+JSON.stringify(calendar_pref));
    $.cookie("admincalendar_pref", JSON.stringify(calendar_pref));
    reArangeSearchFilter();
    $.ajax({
		type: "GET",
		cache: false,
		url: '/?q=administration/calendar/update-preference',
		success: function(result){
		  if (result == 'session_out') {
				self.location='?q=user';
				return;
			}
		  // refresh the data
		  if (result) {
			 $newJquery("#calendarprimaryview").fullCalendar('removeEvents', []);
			 $newJquery("#calendarnarrowview").fullCalendar('removeEvents', []);
			 getCalendarData();
			if((calendar_pref.view_classroom == false) && (calendar_pref.view_virtualclassroom == false) &&	(calendar_pref.view_scheduled_report == false) && (calendar_pref.view_orders == false))
			{
				$('#narrow-search-calendar-filter').val(Drupal.t('LBL304')).prop("disabled","disabled");
				//$('#narrow-search-calendar-filter').prop("disabled","disabled");
				$('.select-list-dropdown-calendar').css('opacity',0.3);
				$('.select-list-calendar-dropdown-link').css('opacity',0.3);
				$('#narrow-search-calendar-filter-container .eol-search-go').css({'background-color':'#f2f2f2','border':'1px solid #e5e5e5'});
				//$('#narrow-search-calendar-filter-container .eol-search-go').css('border','1px solid #e5e5e5');
				$('#narrow-search-calendar-filter-container li.eol-search-input span').css('background-color','#f2f2f2');
				$('#narrow-search-calendar-filter-container li.eol-search-input input').css('background-color','#f2f2f2');
			}else{
				$('#narrow-search-calendar-filter').removeAttr('disabled');
				$('.select-list-dropdown-calendar').css('opacity',1);
				$('.select-list-calendar-dropdown-link').css('opacity',1);
				$('#narrow-search-calendar-filter-container .eol-search-go').css('background-color','white');
				$('#narrow-search-calendar-filter-container .eol-search-go').css('border','1px solid #e5e5e5');
				$('#narrow-search-calendar-filter-container li.eol-search-input span').css('background-color','white');
				$('#narrow-search-calendar-filter-container li.eol-search-input input').css('background-color','white');
			  }
		  }
		},
		failure:function(err){alert(err);}
	}); 

    //$newJquery("#calendarprimaryview").fullCalendar('rerenderEvents'); 
    //$newJquery("#calendarnarrowview").fullCalendar('rerenderEvents');
    //adminCalendarSettingClose();
    //$("#qtipdiv").html(displayAdminCalendarSettings());

    /*$(".eventtypes").each(function(i,evt){
    	alert($(this).attr("id"));
    	});*/
  /*  $("#admincalendarsettings").click(function(e) {
        try {

            if (Drupal.settings.ajaxPageState.theme == "expertusoneV2") {
                $('#admincalendarsettingdiv').toggle();
            } else {
                $('#admincalendarsettingdiv').show();
            }
        } catch (e) {
            // to do
        }
    });
	*/
    adminCalDestroyLoader()

}


function calendarUpdateData(event,revertFunc) {
	  checkSessionOut();
	  var id = new String(event.id);
	
	  if(id != null && id.indexOf("-") >0)
	  {
	  	id = id.substring(id.indexOf("-")+1,id.length);
	  }

	  
	var type = event.type;
	var newdate = event.start.format("YYYY-MM-DD HH:mm:ss");
	var enddate = "";
	if(event.end == null || event.end == "")
		enddate = "";
	else
	    enddate = event.end.format("YYYY-MM-DD HH:mm:ss");
	
	var classid = event.class_id;
	var location_id = event.location_id;
	adminCalCreateLoader();
	
	if(event.startTime != "All day")	
	{
		//62077: When you drag and drop any class, Time get changed on the loading time ( changed time format to 24 hours)
		var newStartTime = event.start.format("HH:mm"); //24 hours format  for display
		event.startTime = newStartTime;
		if(event.end == undefined || event.end == null || event.end == ""){
			event.endTime ="";
		}else{
			event.endTime = event.end.format("HH:mm");
		}
		//console.log('event.end ALL DAY after--'+event.end);
		
	}

	
	if(event.actualEnd == undefined || event.actualEnd == null || event.actualEnd == "")
	{
		enddate = "";
	}
	$newJquery('#calendarprimaryview').fullCalendar('updateEvent', event);
	
	if(event.type == "lrn_cls_dty_ilt" || event.type == "lrn_cls_dty_vcl")
	{
		event.startTime =  event.start.format("HH:mm"); //24hours format
		if(event.endTime == undefined || event.endTime == null || event.endTime == ""){
			event.endTime = "";
		}else{
			event.endTime = event.end.format("HH:mm");
		}
		

	}
		var params = "allDay="+event.allDay+"&id=" + id + "&type=" + type + "&newdate=" + newdate+"&classid="+classid+"&location_id="+location_id+"&startTime="+event.startTime+"&endTime="+event.endTime+"&enddate="+enddate;
		
		if(event.type == "lrn_cls_dty_vcl") {
			params += "&timezone="+event.timezone;
		}
    		
    $.ajax({
        url: "?q=administration/calendarUpdateData",
        METHOD: "post",
        data: params,
        success: function(data) {
            adminCalDestroyLoader()
            if ($.trim(data) == "success") {
				getCalendarData();
            	window.setTimeout(function(){attachEvents('#calendarprimaryview .fc-title a');},200);
            }
            else 
            {
            		var view = $newJquery('#calendarprimaryview').fullCalendar('getView');
    				if(view.name == "agendaDay")
            			getCalendarData();
            	adminCalendarPopup("Alert",data);
	            revertFunc();
            }
        },
        failure: function(data){
	        adminCalDestroyLoader()
        }
    });

}

function displayCreateNewObjectPopupData(dateTypeFeaturePast)
{
	try {
		var html = '<div id="admincalendarcreatenewobjectdiv" style="display:block;">';
	    html += '<div class="dropdownadd-dd-list-arrow" ></div>';
	    html += '<a  class="qtip-close-button account-close-popup"></a>';
	    html += '<table cellspacing="0" cellpadding="0" id="bubble-face-table">';
	    html += '<tbody>';
	    html += '<tr>';
	    html += '<td class="bubble-tl"></td>';
	    html += '<td class="bubble-t"></td>';
	    html += '<td class="bubble-tr"></td>';
	    html += '</tr>';
	    html += '<tr>';
	    html += '<td class="bubble-cl"></td>';
	    html += '<td valign="top" class="bubble-c" id="popupcontent">';
	    if (ADMIN_CALENDAR_PERM_LIST.create_class == 'enabled') {
		    html += '<div class="links header-popup cls-room"><a data-wrapperid="paint-narrow-search-results" class="createnewclass narrow-search-actionbar-textbtn addedit-form-expertusone-throbber ctools-modal-ctools-admin-course-class-addedit-style use-ajax ajax-processed" onclick="calendarallpopupclose(\'ilt-cls\'); addAdminPageClasses(\'ilt-cls\'); setCloseHandler(\'lrn_cls_dty_ilt\');" href="/?q=administration/learning/course-class/ajax/addedit/">'+Drupal.t("ACLBL0001")+'</a>';
		    html += '</div>';
	    } else {
	    	 html += '<div class="links header-popup cls-room"><a data-wrapperid="paint-narrow-search-results" class="disabled">'+Drupal.t("ACLBL0001")+'</a>';
			 html += '</div>';
	    }
	    if (ADMIN_CALENDAR_PERM_LIST.create_class == 'enabled') {
	    	html += '<div class="links header-popup vc-cls"><a data-wrapperid="paint-narrow-search-results" class="createnewclass narrow-search-actionbar-textbtn addedit-form-expertusone-throbber ctools-modal-ctools-admin-course-class-addedit-style use-ajax ajax-processed"  onclick="calendarallpopupclose(\'vcl-cls\'); addAdminPageClasses(\'vcl-cls\');setCloseHandler(\'lrn_cls_dty_vcl\');"  href="/?q=administration/learning/course-class/nojs/addedit/">'+Drupal.t("ACLBL0002")+'</a>';
	    	html += ' </div>';
	    }  else {
	    	 html += '<div class="links header-popup cls-room"><a data-wrapperid="paint-narrow-search-results" class="disabled">'+Drupal.t("ACLBL0002")+'</a>';
			 html += '</div>';
	    }
	    /*if (ADMIN_CALENDAR_PERM_LIST.create_announcement == 'enabled') {
	    	if(dateTypeFeaturePast != "fc-past")
	    	{
	    		html += '<div class="links header-popup announcement"><a data-wrapperid="paint-narrow-search-results" class="createnewclass announcement narrow-search-actionbar-textbtn addedit-form-expertusone-throbber ctools-modal-ctools-admin-announcement-notice-addedit-style use-ajax ajax-processed" onclick="calendarallpopupclose(\'announcement\'); addAdminPageClasses(\'announcement\'); setCloseHandler(\'\');$(\'#root-admin\').data(\'narrowsearch\').setPositionToCtoolPop(\'/administration/manage/announcement/nojs/addedit\')" href="/?q=/administration/manage/announcement/nojs/addedit">'+Drupal.t("Create Announcement")+'</a>';
		    	html += ' </div>';
		    }
	    }  else {
	    	 html += '<div class="links header-popup cls-room"><a data-wrapperid="paint-narrow-search-results" class="disabled">'+Drupal.t("Create Announcement")+'</a>';
			 html += '</div>';
	    }*/
	    if (ADMIN_CALENDAR_PERM_LIST.commerce == true && ADMIN_CALENDAR_PERM_LIST.view_orders == 'enabled') {
	    	if(dateTypeFeaturePast != "fc-past" && dateTypeFeaturePast != "fc-future")
	    	{
	    		html += '<div class="links header-popup order"><a class="" onclick="calendarallpopupclose(\'order\'); addAdminPageClasses(\'order\'); createOrderOrDisplayDiscussionFromAdminCalendar(this);" data="order"   href="javascript:void(0)">'+Drupal.t("ACLBL0004")+'</a>';
	    		html += ' </div>';
	    	}
	    }  else {
	    	if(dateTypeFeaturePast != "fc-past" && dateTypeFeaturePast != "fc-future")
	    	{
	    		html += '<div class="links header-popup cls-room"><a data-wrapperid="paint-narrow-search-results" class="disabled">'+Drupal.t("ACLBL0004")+'</a>';
	    		html += '</div>';
	    	}
	    }
	    if (ADMIN_CALENDAR_PERM_LIST.create_report == 'enabled') {
	    // html += '<div class="links header-popup report"><a id="report-schedule-link-116" data-wrapperid="paint-narrow-search-results" class="createnewclass use-ajax report-schedule-region ajax-processed"  href="?q=administration/report-search/schedules/116" data-qtip-obj="{  \'holderId\': \'create-schedule-116\', \'id\': \'create-schedule-qtip-116\', \'placement\': \'above-or-below\'}">Schedule a Report </a><div id="create-schedule-116"><div id="schedule-qtip-popup"></div></div>';
	    	if(dateTypeFeaturePast != "fc-past" && dateTypeFeaturePast != "fc-future")
	    	{
	    		html += '<div class="links header-popup report"><a data-wrapperid="paint-narrow-search-results" class="report" onclick="displayReportsForCalendar();"  href="javascript:void(0);" >'+Drupal.t("ACLBL0005")+'</a>';
		    	html += ' </div>';
		    }
	    }  else {
	    	if(dateTypeFeaturePast != "fc-past" && dateTypeFeaturePast != "fc-future")
	    	{
	    	 html += '<div class="links header-popup cls-room"><a data-wrapperid="paint-narrow-search-results" class="disabled">'+Drupal.t("ACLBL0005")+'</a>';
			 html += '</div>';
	    }
	    }
	    //html += ' <div class="white-btn-bg-container"><div class="white-btn-bg-left"></div><input class="white-btn-bg-middle close learner-sign-close-link"  type="button" id="edit-close"  onclick="calendarallpopupclose()" name="op" value="Close"><div class="white-btn-bg-right"></div></div>';
	
	    //titleLink += '<a class="titlelinkstyle vtip order narrow-search-actionbar-textbtn addedit-form-expertusone-throbber ctools-modal-ctools-admin-order_edit-style use-ajax ajax-processed"  id="visible-ctools-' + event.type + '-' + event.uc_order_id + '"  href="/?q=administration/commerce/order/ajax/addedit/' + event.uc_order_id + '" data-wrapperid="paint-narrow-search-results">' + '<span class="link">' + shortTitle + '</span>' + timeLegend ;
	    //<a id="report-schedule-link-104" class="use-ajax report-schedule-region ajax-processed" href="?q=administration/report-search/schedules/104" data-qtip-obj="{  'holderId': 'create-schedule-104', 'id': 'create-schedule-qtip-104', 'placement': 'above-or-below'}">Schedule  	</a>
	
	
	    //  titleLink += '<a class="titlelinkstyle vtip announcement narrow-search-actionbar-textbtn addedit-form-expertusone-throbber  ctools-modal-ctools-admin-announcement-notice-addedit-style use-ajax ajax-processed"  id="visible-ctools-' + event.type + '-' + event.id + '"  href="/?q=administration/manage/announcement/ajax/addedit/' + event.id + '" data-wrapperid="paint-narrow-search-results"><span class="link">' + "test" + '</span>' + timeLegend ;
	
	
	    html += ' </td>';
	    html += ' <td class="bubble-cr"></td>';
	    html += '</tr>';
	    html += '<tr>';
	    html += ' <td class="bubble-bl"></td>';
	    html += ' <td class="bubble-blt"></td>';
	    html += ' <td class="bubble-br"></td>';
	    html += '</tr>';
	    html += '</tbody>';
	    html += '</table>';
	    html += '</div>';
	    $(".createnewpopupdiv").html(html);
		// console.log('inside...1');
	    $('#admincalendarcreatenewobjectdiv').show();
	
		var lang = getCurrentLanguage();
	    var langCls = "";
		if(lang == "ru")
	       langCls = "lang-ru";
		$(".createnewpopupdiv").addClass(langCls); 
	    attachEvents('.createnewclass');
	}
	catch(e) {
		// to do
	}
}

function displayCreateNewObjectPopup(dateTypeFeaturePast) {
	try {
		calendarallpopupclose();
		/*if(ADMIN_CALENDAR_PERM_LIST != null)
			window.setTimeout(function(){displayCreateNewObjectPopupData(dateTypeFeaturePast)},200);
		else
		{
		
		callAjaxForIE("?q=administration/calendar/settings&ts="+new Date().getTime(),function(result){
				if(result != null)
				{
			    	ADMIN_CALENDAR_PERM_LIST = JSON.parse(result);
			    	displayCreateNewObjectPopupData(dateTypeFeaturePast);
			    }
		});*/
	   $.ajax({
			type: "GET",
			cache: false,
			url: '/?q=administration/calendar/settings',
			success: function(result){
			  if (result == 'session_out') {
					self.location='?q=user';
					return;
				}
			    ADMIN_CALENDAR_PERM_LIST = result;
			    displayCreateNewObjectPopupData(dateTypeFeaturePast);
			},
			failure:function(err){alert(err);}
		}); 
	} catch(e) {
		// console.log(e);
	}
    
}

function adminCalendarCreateNewClose() {
    try {
        $('#admincalendarcreatenewobjectdiv').css('display', 'none');
    } catch (e) {
        // to do
    }
}




//Close styling pop-up
$('.account-close-popup').live('click', function() {
    calendarallpopupclose();
	$('.active-qtip-div').remove();
});

function addAdminPageClasses(eventtype) {
	adminCalCreateLoader();
	$("body").removeClass(" page-administration page-administration-learning-catalog page-administration-manage-announcement page-administration-commerce page-administration-commerce-order page-reports section-administration");
	if (eventtype == "ilt-cls") {
        $("body").addClass("page-administration-learning-catalog page-administration");
    } else if (eventtype == "vcl-cls") {
        $("body").addClass("page-administration-learning-catalog page-administration");
    } else if (eventtype == "announcement") {
        $("body").addClass("page-administration-manage-announcement page-administration");
    } else if (eventtype == "order") {
        $("body").addClass("page-administration-commerce-order page-administration-commerce page-administration section-administration");
    } else if (eventtype == "report") {
        $("body").addClass("page-reports");
    } else if (eventtype == "schedule") {    	
        //$("body").addClass("page-reports");
    }
    //loadCSSIfNotAlreadyLoadedForSomeReason (eventtype);
   
}
function loadCSSIfNotAlreadyLoadedForSomeReason (eventtype) {
    var ss = document.styleSheets;
    //console.log("loadAnnCSSIfNotAlreadyLoadedForSomeReason---");
    //console.log(ss);
    for (var i = 0, max = ss.length; i < max; i++) {
    	//console.log(ss[i].href);
    	if(ss[i].href == '' || ss[i].href == null || ss[i].href == undefined || ss[i].href == 'undefined' || ss[i].href == 'null'){
    		//console.log('ddddd---'+ss[i].href);
    	}else{
    		//console.log('kkkkkk---'+ss[i].href);
    		if (eventtype == "announcement" && ss[i].href.indexOf("exp_sp_administration_announcement_v2.css") >= 0){
    			//console.log('eventtype Yes---'+eventtype+'--'+ss[i].href);
    			return;
    		}else if(eventtype == "order" && ss[i].href.indexOf("exp_sp_administration_order_v2.css") >= 0 && ss[i].href.indexOf("exp_sp_uc_cart_v2.css") >= 0){
    			//console.log('eventtype Yes---'+eventtype+'--'+ss[i].href);
    			return;
    		}
    	}
    	//console.log('i==='+i);
    }
    if(eventtype == "announcement"){
    	//console.log('eventtype==='+eventtype);
   		$("head").append("<link class='admincalpopupcss' href='sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_announcement/exp_sp_administration_announcement_v2.css' type='text/css' rel='stylesheet' />");
    }else if(eventtype == "order"){
    	//console.log('eventtype==='+eventtype);
    	$("head").append("<link class='admincalpopupcss' href='sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_commerce/exp_sp_administration_order/exp_sp_administration_order_v2.css' type='text/css' rel='stylesheet' />");
    	$("head").append("<link class='admincalpopupcss' href='sites/all/modules/core/exp_sp_commerce/css/exp_sp_uc_cart_v2.css' type='text/css' rel='stylesheet' />");
    }else{
    	//console.log('eventtype==='+eventtype+'remove');
    	$(".admincalpopupcss").remove();
    }
}
function calendarallpopupclose(eventtype) {
    try {
        $('.admincalendarviewmorepopupdiv').css('display', 'none');

        $('#admincalendarsettingdiv').css('display', 'none');
        $('#admincalendarcreatenewobjectdiv').css('display', 'none');
        $('#admincalendarviewreportorderobjectdiv').css('display', 'none');

        $(".today-events div.order").removeClass("order");
        $(".today-events div.regend").removeClass("regend");
        $(".today-events div.regcancel").removeClass("regcancel");
        $(".today-events div.ilt-cls").removeClass("ilt-cls");
        $(".today-events div.vcl-cls").removeClass("vcl-cls");
        $(".today-events div.announcement").removeClass("announcement");

        $(".tmrw-events div.order").removeClass("order");
        $(".tmrw-events div.regend").removeClass("regend");
        $(".tmrw-events div.regcancel").removeClass("regcancel");
        $(".tmrw-events div.ilt-cls").removeClass("ilt-cls");
        $(".tmrw-events div.vcl-cls").removeClass("vcl-cls");
        $(".tmrw-events div.announcement").removeClass("announcement");
        
        $(".ac_order_links").hide();
    	//if($newJquery("#calendarprimaryview").fullCalendar("getView").name == "month")
    	//	mouseOverDelay(document.getElementById('calendarprimaryview'), admincalendarmouseover);


        

    } catch (e) {
        // to do
    }

}

function getCalendarCellPosition(event) {
    var posx = 0;
    var posy = 0;
    if (!e) var e = window.event;
    if (e.pageX || e.pageY) {
        posx = e.pageX;
        posy = e.pageY;
    } else if (e.clientX || e.clientY) {
        posx = e.clientX + document.body.scrollLeft + document.documentElement.scrollLeft;
        posy = e.clientY + document.body.scrollTop + document.documentElement.scrollTop;
    }
    // posx and posy contain the mouse position relative to the document
    // Do something with this information
    return {
        "x": posx,
        "y": posy
    }
}

function isDatePastTodayFuture(object, type)
{
	var today = "";
	var type = (type != undefined || type != '') ?  type : '';
	// today calc
	if(calendardata != null) {
		today = moment(moment(calendarmetadata[0].todaydate).format('YYYY-MM-DD HH:mm'));
	} else {
		today = moment(moment(new Date()).format('YYYY-MM-DD HH:mm'));
	}
	if (type == 'add_entity') {
		today = moment(moment(new Date()).format('YYYY-MM-DD'));
		var evtDateString = moment(object, "MM-DD-YYYY").format('YYYY-MM-DD');
		var evtDate		= moment(object, "MM-DD-YYYY");
	} else if (type == 'display' || type == 'update') {
		if (object.type == 'cre_sys_obt_anm' || (object.type == 'report' && object.frequency == 'daily') ) {
			var end = object.actualEnd; //(object.end == null) ? object.actualEnd : object.end;
 			var todayString = moment(calendarmetadata[0].todaydate).format("YYYY-MM-DD HH:mm");
			if (end instanceof Object) { 
				var evtDateString = end.format("YYYY-MM-DD HH:mm");
			} else {
				var evtDateString = moment(end).format("YYYY-MM-DD HH:mm");
			}	
		} else {
			var evtDateString = moment(object.start).format('YYYY-MM-DD HH:mm');
			var evtDate		= moment(object.start);
		}
	} else {
		var evtDateString = moment(object.start).format('YYYY-MM-DD HH:mm'); // old start format support
		var evtDate		= moment(object.start).format('YYYY-MM-DD HH:mm');
	}
	//alert(evtDate.format("YYYY-MM-DD HH:MM:SS"));
	// console.log("today:"+today);
	// console.log("evtDate:"+evtDate);
	// console.log("today:"+today.format("YYYY-MM-DD"));
	// console.log("evtDate:"+evtDate.format("YYYY-MM-DD"));
	if (object.type == 'cre_sys_obt_anm' || (object.type == 'report' && object.frequency == 'daily') ) {
		//console.log('todayString: ' + todayString);
		//console.log( 'evtDateString: ' + evtDateString);
		var datePastTodayFuture = moment(evtDateString).diff(moment(todayString), 'minutes');
	} else {
		var datePastTodayFuture = evtDate.diff(today, 'days');
	}
	//console.log('datePastTodayFuture of event diff: ' + datePastTodayFuture);
		// // // console.log("Today:" + moment().format("MM-DD-YYYY") + " = Evt:" + evtDate.format("MM-DD-YYYY") + " = dif:" + datePastTodayFuture);
    var clsForGreyout = "";
	if (datePastTodayFuture == 0)
		clsForGreyout = "fc-today";
	else if (datePastTodayFuture > 0)
		clsForGreyout = "fc-future";
	else
		clsForGreyout = "fc-past";
	return clsForGreyout;
	
}
function addObjectThroughAdmCalendar(object, event) {
	$('.active-qtip-div').remove();	
	//console.log('addObjectThroughAdmCalendar--->');
    var year 	= $newJquery("#calendarprimaryview").fullCalendar("getDate").format("YYYY");
    var view 	=  $newJquery("#calendarprimaryview").fullCalendar("getView");
    var dateType = "", selectedDate = "";
    //console.log('addObjectThroughAdmCalendar view.name--->'+view.name);
    if(view.name == "basicWeek")
    {
    	var selectedDate = (event.type == 'contextmenu') ? object.format("MM-DD-YYYY") : $(object).data('date');
    	//console.log('basicWeek: '+ selectedDate);
    	$.cookie("startdate", selectedDate);
    	dateType = isDatePastTodayFuture(selectedDate, 'add_entity');
    }
    else if(view.name == "month")
    {
    	selectedDate = $(object).parent().parent().attr("data-date");
		if(selectedDate == null || selectedDate == undefined || selectedDate == 'undefined')
		{
				//will be undefined for right click. plugin object date in object parameter
			    $.cookie("startdate", moment(selectedDate).format("MM-DD-YYYY"));
	    		dateType = isDatePastTodayFuture(object, 'add_entity');	    		
        }
        else
        {
        	 	$.cookie("startdate", moment(selectedDate).format("MM-DD-YYYY"));
		    	dateType = isDatePastTodayFuture(moment(selectedDate), 'add_entity');

        }
	}
	else
	{
		var selectedDate = (event.type == 'contextmenu') ? object.format("MM-DD-YYYY") : $(object).data('date');
		//console.log('selectedDate3: '+ selectedDate);
		$.cookie("startdate", selectedDate);
    	dateType = isDatePastTodayFuture(selectedDate, 'add_entity');
	}

    if ($(object).parent().parent().hasClass("fc-past")) {
    		dateType ="fc-past";
        //alert("Entites will not be allowed to create for past date");
      //  adminCalendarPopup("Alert","Entites will not be allowed to create for past date ")
       // return false;
    }
    
    $(".createnewpopupdiv").remove();
    var tooltip = '<div class="createnewpopupdiv" style="position:absolute;"></div>';
    $("body").append(tooltip);
    $(object).css("z-index", 10000);
 
    $(".createnewpopupdiv").css("position", "absolute");
    $(".createnewpopupdiv").css("top", event.pageY + 10);
    
    if($(object).hasClass("popup"))
    {
    	$(".createnewpopupdiv").css("left", event.pageX - 165);
    }
    else
    	$(".createnewpopupdiv").css("left", event.pageX - 170);
    
    $(".createnewpopupdiv").html(displayCreateNewObjectPopup(dateType));
    try {
        if (Drupal.settings.ajaxPageState.theme == "expertusoneV2") {
            $('#admincalendarcreatenewobjectdiv').toggle();
        } else {
            $('#admincalendarcreatenewobjectdiv').show();
        }
        //closePopupOnEscAndOutsideClick(".createnewpopupdiv");   
    } catch (e) {
        // to do
    }

    //  closePopupOnEscAndOutsideClick("#admincalendarcreatenewobjectdiv");
}




function getDataViewReportOrderPopup(object,frstartDate,pageY) {
	if(frstartDate=='' && object !='' && object != undefined){
	    var view =  $newJquery("#calendarprimaryview").fullCalendar("getView");
	    var dateType = "",startDate = "";
	    if(view.name == "basicWeek")
	    {
	    	var year = $newJquery("#calendarprimaryview").fullCalendar("getDate").format("YYYY");	    	
	    	startDate = $(object).attr("data-date");
	    	startDate = moment(startDate,'MM-DD-YYYY').format("YYYY-MM-DD");
	    }	
		else if(view.name == "month")
		{
			 startDate = $newJquery(object).parent().parent().attr("data-date");
		}
		else
		{
			startDate = $(object).parent().parent().find(".date").attr("data-date");
	    	startDate = moment(startDate,'MM-DD-YYYY').format("YYYY-MM-DD");
	   }
	   //console.log(view.name+'----'+startDate);
	   if($(object).hasClass("popup"))
	   		startDate = $(object).attr("data-date");
	}else{
		startDate = frstartDate;
	}
	
	adminCalCreateLoader();
    	$.ajax({
        	url: "/?q=administration/viewdaywisereportorderlocations",
	        METHOD: "post",
    	    data: "startDate=" + startDate,
        	success: function(data) {
	        	adminCalDestroyLoader()
	            displayViewReportPopupDayWise(object, data,startDate,pageY);
    	    },
        	failure:function(data){
	        adminCalDestroyLoader()
    	    }
	    });
	}
function createDropDown(parentid){
	$(".orderlinks").css("display","none");
	$("#selOrderlinksId_"+parentid).css("display","block");
}
$(function() {
	try {
		$('body').live('click', function(event) {
			if($(event.target).is('.blue-btn-bg-right') || ($(event.target).parents('a.blue-btn-bg-right').length)) {
				return false;
			}
			if (!$(event.target).parents('.ac_order_links').length) {
				$('.orderlinks').hide();
			}
		});
	}
	catch (e) {
		// TODO: handle exception
	}
});
function displayViewReportPopupDayWise(object, data,frstartDate,pageY) {
    calendarallpopupclose();
    var admincalendar_pref = $.cookie("admincalendar_pref");

    var jsonobj = JSON.parse(data);
	var recordExists = false;
    admincalendar_pref = JSON.parse(admincalendar_pref);

    var html = '<div id="admincalendarviewreportorderobjectdiv" style="display:none;">';
    html += '<div class="dropdownadd-dd-list-arrow"></div>';
    html += '<a  class="qtip-close-button account-close-popup"></a>';
    html += '<table cellspacing="0" cellpadding="0" id="bubble-face-table">';
    html += '<tbody>';
    html += '<tr>';
    html += '<td class="bubble-tl"></td>';
    html += '<td class="bubble-t"></td>';
    html += '<td class="bubble-tr"></td>';
    html += '</tr>';
    html += '<tr>';
    html += '<td class="bubble-cl"></td>';
    html += '<td valign="top" class="bubble-c">';
    html += '<div class="listviewcontainer" style="min-height:50px;max-height:200px;overflow:auto; width:auto;">';
    if (ADMIN_CALENDAR_PERM_LIST.view_scheduled_report == 'enabled' && admincalendar_pref.view_scheduled_report == true && jsonobj.length > 0) {
	    recordExists = true;
        html += '<div class="reports categ-block">';
        html += '<h4>'+Drupal.t("REPORTS")+': <span>('+Drupal.t("ACLBL0023")+')</span></h4>';
        for (var i = 0; i < jsonobj.length; i++) {
            var row = jsonobj[i];
            if(row.type == "report")
            {
            html += '<div class="links header-popup" >';
            html += "<span class='list vtip' title='"+row.title+"' onclick=\"calendarallpopupclose(\'report\');addAdminPageClasses(\'report\');setCloseHandler(\'edit\');$('body').data('reportsearch').reportAddWizard(); $('body').data('reportsearch').launchReport(" + row.report_id + ", 'report-data-source');\">" + titleRestrictionFadeoutImage(row.title ,'admin-cal-left-title') + '</span>';//<span class="link">View</span>
            html += '</div>';
            }
        }

        html += '</div>';
    }
   
    if (ADMIN_CALENDAR_PERM_LIST.view_orders == 'enabled' && admincalendar_pref.view_orders == true && jsonobj.length >0 && ADMIN_CALENDAR_PERM_LIST.commerce == true) {
		recordExists = true;
        html += '<div class="orders categ-block">';
        html += '<h4>'+Drupal.t("Orders")+': <span>('+Drupal.t("ACLBL0022")+')</span></h4>';
        for (var i = 0; i < jsonobj.length; i++) {
            var row = jsonobj[i];
            if(row.type == "order")
			{ 
				var fDate = "'"+frstartDate+"'"; 
				var msg_title ='';
            html += '<div class="links header-popup"><div class="order-by">';
            html += '<span class="list-item vtip" title="' + row.title + '"><a onclick="calendarallpopupclose(\'order\');addAdminPageClasses(\'order\');setCloseHandler(\'edit\');" class="order_row narrow-search-actionbar-textbtn addedit-form-expertusone-throbber ctools-modal-ctools-admin-order_edit-style use-ajax ajax-processed"  id="visible-ctools-' + row.type + '-' + row.uc_order_id + '"  href="/?q=administration/commerce/order/ajax/addedit/' + row.uc_order_id + '" data-wrapperid="paint-narrow-search-results">' + titleRestrictionFadeoutImage(row.title ,'admin-cal-left-title') + '</a></span>';
            if(row.order_status=='canceled'){
            	html += '<span id="applyorderactionid" class="applyorderactionid"><span class="label clsCanceled">'+Drupal.t("Canceled")+'</span></span>';
            }
			if(row.order_status == "pending"){	
				msg_title = "'"+Drupal.t("LBL1163")+"'";				
            	html += '<span class="action-link applyorderactionid" id="applyorderactionid_'+i+'"><span class="label clsApprove" onclick="displayDragConfirm('+msg_title+', '+row.uc_order_id+', \'approve\',\'order_update\', '+fDate+');"  style="padding-right:0px">'+Drupal.t("ACLBL0024")+'</span><a data="'+row.uc_order_id +'" class="blue-btn-bg-right" onclick="createDropDown('+i+');"></a></span>';
            	html += "<div class=\"orderlinks\" id=\"selOrderlinksId_"+i+"\" style=\"display:none\"><ul class=\"dd-"+row.uc_order_id+" ac_order_links\"><li  class=\"label clsddCancel\" style=\"cursor:pointer;\" onclick=\"applyOrderAction("+row.uc_order_id+", \'cancel\',\'"+frstartDate+"\');\" >"+Drupal.t("LBL109")+"</li></ul></div>";
            	//$("#applyorderactionid_"+i).append("<div class=\"orderlinks\" id=\"selOrderlinksId_"+i+"><ul class=\"dd-"+row.uc_order_id+" ac_order_links\"><li  class=\"label clsddCancel\" style=\"cursor:pointer;\" onclick=\"applyOrderAction("+row.uc_order_id+", \'cancel\');\" >"+Drupal.t("LBL109")+"</li></ul></div>");
			}
            if ((row.order_status == "payment_received" || row.order_status == "completed") && row.complete_count == 0){
            	msg_title = "'"+Drupal.t("LBL1164")+"'";
            	html += '<span class="action-link applyorderactionid" id="applyorderactionid"><span class="label clsCancelLink"  onclick="displayDragConfirm('+msg_title+', '+row.uc_order_id+', \'cancel\',\'order_update\', '+fDate+');">'+Drupal.t("LBL109")+'</span></span>';
            }else if((row.order_status == "payment_received" || row.order_status == "completed") && row.complete_count > 0){
            	html += '<span  class="applyorderactionid"><span class="label clsCancel">'+Drupal.t("LBL109")+'</span></span>';
			//$("body").append("<div class=\"orderlinks\"><a class=\"label\" style=\"cursor:pointer;\" onclick=\"displayDragConfirm("+row.uc_order_id+", \'cancel\');\" >"+Drupal.t("LBL109")+"</a></div>");
			}
			 html += '</div><div class="order-details">';
			 html += '<span class="order-no">'+Drupal.t("LBL1025")+': '+row.uc_order_id+'</span>';
			 html += '<span  class="order-price">'+row.currency_type + ' '+ row.price+'</span>';
			 html += '</div>';
			 html += '</div>';
			
			}
        }

        html += '</div>';
    }
    if (ADMIN_CALENDAR_PERM_LIST.view_locations == 'enabled' && admincalendar_pref.view_locations == true &&  jsonobj.length >0) {
    	recordExists = true;
        html += '<div class="locations categ-block">';
        html += '<h4>'+Drupal.t("ACLBL0006")+': <span>('+Drupal.t("ACLBL0007")+')</span></h4>';
        for (var i = 0; i < jsonobj.length; i++) {
            var row = jsonobj[i];
            if(row.type == "location")
            {
            //alert(JSON.stringify(row));
            html += '<div class="links header-popup">';
            html += '<span class="loca-time">' + calTimeFormat(row.start,row.startTime,true) + " - " + calTimeFormat(row.start,row.endTime,true) + '</span><span class="loca-list vtip" title="' + row.title + '">' + titleRestrictionFadeoutImage(row.title ,'admin-cal-left-title') + '</span>';
           // html += '<span class="list-spot"></span><span class="time">' + calTimeFormat(row.start,row.startTime) + '</span><span class="summary-title">' + displayShortTitle(row.title) + '</span>';
            html += '</div>';
            }
        }
        html += '</div>';
    }

 	if (ADMIN_CALENDAR_PERM_LIST.view_instructors == 'enabled' && admincalendar_pref.view_instructors == true &&  jsonobj.length >0) {
 		recordExists = true;
        html += '<div class="instructors categ-block">';
        html += '<h4>'+Drupal.t('ACLBL0008')+': <span>('+Drupal.t('ACLBL0007')+')</span></h4>';
        for (var i = 0; i < jsonobj.length; i++) {
            var row = jsonobj[i];
           if(row.type == "instructor")
           {
            html += '<div class="links header-popup">';
            html += '<span class="loca-time">' + calTimeFormat(row.start,row.startTime,true) + " - " + calTimeFormat(row.start,row.endTime,true) + '</span><span class="loca-list vtip" title="' + row.title + '">' + titleRestrictionFadeoutImage(row.title ,'admin-cal-left-title') + '</span>';
            //html += '<span class="loca-list vtip" title="' + row.title + '">' + row.title + '</span><span class="loca-time">' + row.startTime + " - " + row.endTime + "</span>";
            html += '</div>';
            }
        }
        html += '</div>';
    }
    

    if (ADMIN_CALENDAR_PERM_LIST.forum == true && ADMIN_CALENDAR_PERM_LIST.view_discussions == 'enabled' && admincalendar_pref.view_discussions == true &&  jsonobj.length >0) {
    	recordExists = true;
        html += '<div class="discussions categ-block">';
        html += '<h4>'+Drupal.t('DISCUSSIONS')+': <span>('+Drupal.t('DISCUSSIONS')+')</span></h4>';
        for (var i = 0; i < jsonobj.length; i++) {
            var row = jsonobj[i];
            if(row.type == "discussion")
            {
            //alert(JSON.stringify(row));
            html += '<div class="links header-popup">';
            html += '<span class="list vtip" title="' + row.title + '"  style="cursor:pointer;" data="discussion" onclick="createOrderOrDisplayDiscussionFromAdminCalendar(this)" href="?q=learning/forum-topic-list/' + row.tid + '">' + row.title + '</span>';
            html += '</div>';
            }
        }
        html += '</div>';
    }


	if(!recordExists)
	{
		if(pageY!=''){
			$(".viewreportorderpopupdiv").removeClass('arrowbelow');
			$(".viewreportorderpopupdiv").css("top", pageY + 10);
		}		
		html += "<span class='label'>"+Drupal.t("MSG403")+"</span>";
	}
    //html += ' <div class="white-btn-bg-container"><div class="white-btn-bg-left"></div><input class="white-btn-bg-middle close learner-sign-close-link"  type="button" id="edit-close"   onclick="calendarallpopupclose()"  name="op" value="Close"><div class="white-btn-bg-right"></div></div>';

    html += '</div>';

    html += ' </td>';
    html += ' <td class="bubble-cr"></td>';
    html += '</tr>';
    html += '<tr>';
    html += ' <td class="bubble-bl"></td>';
    html += ' <td class="bubble-blt"></td>';
    html += ' <td class="bubble-br"></td>';
    html += '</tr>';
    html += '</tbody>';
    html += '</table>';
    html += '</div>';
    $(".viewreportorderpopupdiv").html(html);

    try {
        if (Drupal.settings.ajaxPageState.theme == "expertusoneV2") {
            $('#admincalendarviewreportorderobjectdiv').toggle();
        } else {
            $('#admincalendarviewreportorderobjectdiv').show();
        }
        vtip();
    } catch (e) {
        // to do
    }


    attachEvents(".order_row");
    $(".listviewcontainer .categ-block:last").css('border-bottom', 0);
    $(".listviewcontainer").jScrollPane({
        width: "1px"
    });

   // closePopupOnEscAndOutsideClick("#admincalendarviewreportorderobjectdiv"); #ticket no-0062848
    if(!recordExists)
	    $(".listviewcontainer").css("height","30px");

}

function listObjectThroughAdmCalendar(object, event) {
    $(".viewreportorderpopupdiv").remove();
	$('.active-qtip-div').remove();	

	var lang = getCurrentLanguage();
	var arrowClass='';
       var langCls = "";
       if(lang == "ru")
           langCls = "lang-ru";
	if(event.pageY > 580){
		var arrowClass="arrowbelow";
	}
	if(event.pageX < 700 && event.pageY < 580 ){
	  var arrowClass="arrowleft";
	}
	if(event.pageX > 700 && event.pageY > 580 ){
	  var arrowClass="arrowbelowright";
	}
    var tooltip = '<div class="viewreportorderpopupdiv '+ langCls +' '+ arrowClass +'" style="position:absolute;z-index:101;"></div>';
    $("body").append(tooltip);
    $(object).css("z-index", 100);
    $(".viewreportorderpopupdiv").css("position", "absolute");
    if(event.pageY > 580){
			$(".viewreportorderpopupdiv").css("top", event.pageY - 240);
	}else{
			$(".viewreportorderpopupdiv").css("top", event.pageY + 10);
	}    

	if($(object).hasClass("popup"))
		$(".viewreportorderpopupdiv").css("left", event.pageX - 225);
	else if(event.pageX < 700)
	    $(".viewreportorderpopupdiv").css("left", event.pageX - 115);
	else
    	$(".viewreportorderpopupdiv").css("left", event.pageX - 306);
    getDataViewReportOrderPopup(object,'',event.pageY);
    $('#content .section').css('padding-top', '6px'); 
}


function printPreview() {
	checkSessionOut();
	var view = $newJquery('#calendarprimaryview').fullCalendar('getView');
	
	if(view.name == "year")
    {
    	adminCalendarPopup("Alert","Print is disabled for year view");
		return;
    }
	var startDate = "",endDate = "", endDateCal="";
	
	if(view.name == "month"){
		var startDate = $newJquery("#calendarprimaryview").fullCalendar('getView').intervalStart.format("YYYY-MM-DD");
		endDateCal = $newJquery("#calendarprimaryview").fullCalendar('getView').intervalEnd.format("YYYY-MM-DD"); 
	    endDate = moment(endDateCal).subtract(1, 'days').format("YYYY-MM-DD");
	}else{
		 startDate =  $newJquery('#calendarprimaryview').fullCalendar('getView').start.format("YYYY-MM-DD");
	   	if(view.name == "agendaDay"){
	   		endDate = startDate;
	   	}
	   	else{	
		    endDateCal = $newJquery("#calendarprimaryview").fullCalendar('getView').intervalEnd.format("YYYY-MM-DD"); 
		    endDate = moment(endDateCal).subtract(1, 'days').format("YYYY-MM-DD");
		}
	}
	   
	    var admincalendar_pref = $.cookie("admincalendar_pref");
	   
	adminCalCreateLoader();
		var ac_searchparams = getACSearchParams();

	    $.ajax({
    	    url: "/?q=administration/calendarviewapi",
        	METHOD: "post",
	        data: "startDate=" + startDate + "&endDate=" + endDate + ac_searchparams+"&print=true&type=export", //&selectedobjects="+admincalendar_pref+" 
    	    success: function(data) {
			adminCalDestroyLoader()
		        openWindowForPrintAC(data);
	        },
    	    failure:function(data)
        	{
        		adminCalDestroyLoader()
	        }
    	});
    /*  setTimeout(popupWin.print(), 20000);*/
}

function openWindowForPrintAC(data)
{
	calendarPDFConvert(data);
	    		/*var popupWin = window.open('', '_blank');
    	        popupWin.document.open();
        	    popupWin.document.write('<html><head><title>Print</title>');
        	    popupWin.document.write('<link type="text/css" rel="stylesheet" href="/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_admincalendar/fullcalendar.css?ts="'+new Date().getTime()+' media="all" />');
        	    popupWin.document.write('</head><body">')
            	calendarPDFConvert(data);
	            popupWin.document.write('<script type="text/javascript">window.print();<' + '/script>');
    	        popupWin.document.write('</body></html>');
        	    popupWin.document.close();*/
}
function calendarPDFConvert(data) {
    var oContent = document.getElementById("calendar").innerHTML;
    var oLanguage = "en";
    $('body').append("<form style=\"display:none;\" id=\"postcertihtmlform\" name=\"printform\" method=\"POST\" action=\"/sites/all/commonlib/pdf_template.php\" target=\"_self\"></form>");
    
    $('#postcertihtmlform').append($('<input type="hidden" name="bodyContent"></input>').attr('value', data));
    $('#postcertihtmlform').append($('<input type="hidden" name="contentLanguage"></input>').attr('value', oLanguage));
    $('#postcertihtmlform').submit();
    $('#postcertihtmlform').remove();
}

function calendarExcelConvert(startDate, endDate) {
	var url = "?q=administration/calendarviewapi&startDate=" + startDate + "&endDate=" + endDate;
    $('body').append("<form style=\"display:none;\" id=\"postcertihtmlform\" name=\"printform\" method=\"POST\" action="+url+" target=\"_self\"></form>");
    $('#postcertihtmlform').append($('<input type="hidden" name="startDate"></input>').attr('value', startDate));
    $('#postcertihtmlform').append($('<input type="hidden" name="endDate"></input>').attr('value', endDate));
    $('#postcertihtmlform').append($('<input type="hidden" name="export"></input>').attr('value', "true"));
    
    var search_type = $("#search_all_calendar_type-hidden").val();
    var search_text	= $("#narrow-search-calendar-filter").val();
	var langtitle = Drupal.t("LBL304").toUpperCase();
	
	if((search_text.toLowerCase()) == (langtitle.toLowerCase()))
	{
		search_text='';
	}
			  
    $('#postcertihtmlform').append($('<input type="hidden" name="search_type"></input>').attr('value', search_type));
    $('#postcertihtmlform').append($('<input type="hidden" name="search_text"></input>').attr('value', search_text));
    
	var admincalendar_pref = $.cookie("admincalendar_pref");
	var admincalendar_pref = $.cookie("admincalendar_pref");
	var view = $newJquery('#calendarprimaryview').fullCalendar('getView');
	$('#postcertihtmlform').append($('<input type="hidden" name="selectedobjects"></input>').attr('value', admincalendar_pref));
	$('#postcertihtmlform').append($('<input type="hidden" name="view"></input>').attr('value', view.name));
    $('#postcertihtmlform').submit();
    $('#postcertihtmlform').remove();
}

function displayMorePopupFromCalendarSummary(object) {
	//console.log(object);
    var date = $(object).attr("selecteddate");
    // var container = $(object).parent().attr('id');
    
  /*  displayMorePopup({
        "date": date,
        "dayEl": object
    });;
    $(".admincalendarviewmorepopupdiv").css("top", $(object).offset().top); 
    $(".admincalendarviewmorepopupdiv").css("left", $(object).offset().left);

	*/
	
	displayCalendarSummaryEventsList(date, true);
	vtip();
	
}

function displayMorePopup(cellInfo, e) {
	checkSessionOut();
//	alert();
	//mouseOverDelay(document.getElementById('calendarprimaryview'), null);
    $(".admincalendarviewmorepopupdiv").removeClass("left-narrow");
    $(".admincalendarviewmorepopupdiv").addClass("more-list");
    calendarallpopupclose();
    var html = '<div id="admincalendarviewmorepopup" style="display:none;">';
    html += '<div class="dropdownadd-dd-list-arrow"></div>';
    html += '<a  class="qtip-close-button account-close-popup"></a>';
    html += '<table cellspacing="0" cellpadding="0" id="bubble-face-table">';
    html += '<tbody>';
    html += '<tr>';
    html += '<td class="bubble-tl"></td>';
    html += '<td class="bubble-t"></td>';
    html += '<td class="bubble-tr"></td>';
    html += '</tr>';
    html += '<tr>';
    html += '<td class="bubble-cl"></td>';

    html += '<td valign="top" class="bubble-c">';
   // html += '<span>Loading...</span>';
  /*  html += '<div class="header-col">';
//    var momentObj = moment(cellInfo.attr("data-date")).format("DD");
	var momentObj = moment(cellInfo.date).format("DD");
    html += '<span class="date" >'+momentObj+'</span>';
    html += '<span class="add-list-icons">';
    html +=  getCalendarCellListImg("popup",moment(cellInfo.attr("data-date")).format("YYYY-MM-DD") )+"&nbsp;"+getCalendarCellAddImg("popup",moment(cellInfo.attr("data-date")).format("YYYY-MM-DD"));
    html += '</span>';
    html += '</div>';*/
	   html += '<div class="rows">';
	   html += '<span class="fc-title"></span>';
    html += '</div>';
    //html += ' <div class="white-btn-bg-container"><div class="white-btn-bg-left"></div><input class="white-btn-bg-middle close learner-sign-close-link"  type="button" id="edit-close"  onclick="calendarallpopupclose()"  name="op" value="Close"><div class="white-btn-bg-right"></div></div>';
    
    html += ' </td>';
    html += ' <td class="bubble-cr"></td>';
    html += '</tr>';
    html += '<tr>';
    html += ' <td class="bubble-bl"></td>';
    html += ' <td class="bubble-blt"></td>';
    html += ' <td class="bubble-br"></td>';
    html += '</tr>';
    html += '</tbody>';
    html += '</table>';
    html += '</div>';
    $(".admincalendarviewmorepopupdiv").html(html);
    /*$(".admincalendarviewmorepopupdiv").css("z-index",10001);
    $(".admincalendarviewmorepopupdiv").css("top",e.pageY); 
    $(".admincalendarviewmorepopupdiv").css("left",e.pageX);*/

    //	$(".admincalendarviewmorepopupdiv").css("top",$(cellInfo.dayEl).offset().top-41);    /* more list popup  */
    //	$(".admincalendarviewmorepopupdiv").css("left",$(cellInfo.dayEl).offset().left -520);

    $('.admincalendarviewmorepopupdiv').show();
    try {
        if (Drupal.settings.ajaxPageState.theme == "expertusoneV2") {
            $('#admincalendarviewmorepopup').toggle();
        } else {
            $('#admincalendarviewmorepopup').show();
        }
    } catch (e) {
        // to do
    }
    
    var momentObj = moment(cellInfo.date).format("YYYY-MM-DD");
    var startDate = momentObj;
    var endDate = moment(cellInfo.date).add(1, "days").format("YYYY-MM-DD");
    adminCalCreateLoader();
    	$.ajax({
	        url: "/?q=administration/calendarviewapi",
        	METHOD: "post",
    	    data: "type=more&startDate=" + startDate + "&endDate=" + startDate + "&search_type="+ search_type,
	        success: function(data) {
	        	adminCalDestroyLoader()
	    	    displayMorePopupData(data);
    	    },
	        failure:function(data)
        	{
        		adminCalDestroyLoader()
        	}
    	});
    }

function ac_events_sorting(a,b)
{
	var  aTime = moment(a.startTime,["h:mm A"]);
	var bTime =  moment(b.startTime,["h:mm A"]);
	if(aTime < bTime) return -1;
	if(aTime > bTime) return 1;
	return 0;
}
function ac_events_title_sorting(a,b)
{
	var  aTime = moment(a.startTime,["h:mm A"]);
	var bTime =  moment(b.startTime,["h:mm A"]);
	aTime = aTime + ' '+a.title;
	bTime = bTime + ' '+b.title;
	if(aTime < bTime) return -1;
	if(aTime > bTime) return 1;
	return 0;
}
function displayMorePopupData(data)
{
		//var dt = moment("12:15 AM", ["h:mm A"]).format("HH:mm");
	    var eventsData = "<span class='fc-title'>";

 		var events = JSON.parse(data);
 		if(events.length != null)		        	
	        events = events.sort(ac_events_sorting);

 		for(var j = 0; j < events.length; j++)
 		{
 			eventsData += displayCalendarEvent(events[j], '', '60', 'more_popup') + " "; /*  triple-dot in primary     */
        }

        eventsData += "</span>";
        $newJquery("#admincalendarviewmorepopup .rows").html(eventsData);

		closePopupOnEscAndOutsideClick("#admincalendarviewmorepopup");

		attachEvents('#admincalendarviewmorepopup .fc-title div.titlelinkstyle');
        $(".admincalendarviewmorepopupdiv .fc-title").jScrollPane({
            'width': '1px'
        });

        vtip();
}
function displayCalendarEvent(event, element, charCount, callFrom) {
    var admincalendar_pref = $.cookie("admincalendar_pref");
    var actionLink = '';
    var modalStyle = '';
    admincalendar_pref = JSON.parse(admincalendar_pref);

	var id = new String(event.id);
	
	  if(id != null && id.indexOf("-") >0)
	  {
	  	id = id.substring(id.indexOf("-")+1,id.length);
	  }

   
    var titleLink = "",
        shortTitle = "";

    if (event.title != null && removeHTMLTags(event.title) != null && removeHTMLTags(event.title).length > charCount)
        shortTitle = removeHTMLTags(event.title).substring(0, charCount) + "...";
    else
        shortTitle = event.title;
    shortTitle = titleRestrictionFadeoutImage(shortTitle,'admin-cal-title')
    // // // console.log("event.title=" + event.title + " ==shortTitle:" + shortTitle + "," + charCount);
    // shortTitle = event.title;		
    // <a href="/?q=administration/learning/course-class/ajax/addedit/" class="createnewclass narrow-search-actionbar-textbtn addedit-form-expertusone-throbber ctools-modal-ctools-admin-course-class-addedit-style use-ajax ajax-processed calendar-processed" data-wrapperid="paint-narrow-search-results">Create Classroom </a>
    //var dateType = isDatePastTodayFuture(event, 'display');
   if(event.type == 'report')
	   var dateType = isDatePastTodayFuture(event, 'display');
   else
	   var dateType =  event.dataType;
   
    	titleLink = "<span class='list-item-row fc-event-container "+dateType+"' style='width:100%;'>";
    var timeLegend = "";
    // // // console.log("event.type:" + event.type);
    var view = $newJquery('#calendarprimaryview').fullCalendar('getView');
    var timing = "";
    var toottipTiming = "";
    var today = "";
    var allDayEvent = '';
    var eventStartTime = '';    
       
	if(view.name == "agendaDay" || event.allDay == true)
	{
		allDayEvent = 'all-day-events';
		if(event.endTime != undefined && event.endTime != ""){
			timing = event.startTime +" - "+event.endTime;
			if(event.startTime != undefined && event.startTime != "")	
			eventStartTime = calTimeFormat(event.from_date,event.startTime);				
		}else
		{
			if(event.startTime == "All day"){
				timing = Drupal.t('LBL1039')+' '+Drupal.t('LBL910');
				if(event.startTime != undefined && event.startTime != "")
					eventStartTime = calTimeFormat(event.from_date,event.startTime);
			}else{
				timing = event.startTime;								
				if(event.startTime != undefined && event.startTime != "")							
				eventStartTime = calTimeFormat(event.from_date,event.startTime);						
			}
			
			/*if(calendardata != null) {
        		today = moment(calendarmetadata[0].todaydate);//.format('YYYY-MM-DD');
        	} else {
        		today = moment(new Date());//.format("YYYY-MM-DD");
        	}
        	if(today.format("YYYY-MM-DD") == event.start.format("YYYY-MM-DD"))
        	{
        	    	startTime = event.start.format("YYYY-MM-DD hh:mm A");//(11);
        	    	alert(event.title+"=="+startTime);
        	    	if(startTime == "12:00")
        	    	{
        	    		timing = "All day";
        	    	}
        	    	else
        	    	{
        	    		timing = event.start.format("hh:mm A");//startTime.substring(0,5);        	    		
        	    	}
        	}*/
	   }
	}
	else
	{
		if(event.startTime == "All day"){
			timing = Drupal.t('LBL1039')+' '+Drupal.t('LBL910');
			allDayEvent = 'all-day-events';
			if(event.startTime != undefined && event.startTime != "")
				eventStartTime = calTimeFormat(event.from_date,event.startTime);
		}else{
			timing = event.startTime;								
			eventStartTime = calTimeFormat(event.from_date,event.startTime);							
		}
		
	}
	var tagElement = '';
	var startTime = '';
	var toolStartTime = '';
	if(view.name != "agendaDay" || event.allDay == true)
		toolStartTime = eventStartTime;
    if(event.endTime != undefined && event.endTime != ""){
			toottipTiming = event.startTime +" - "+ event.endTime;
//			if(event.type == 'cre_sys_obt_anm')
//				toottipTiming = event.from_date +'('+ event.startTime +") - "+event.to_date +'('+event.endTime+')';
		}else{
			toottipTiming = timing;
		}	
    
    switch (event.type) {
	    case 'lrn_cls_dty_ilt':
	    	if (admincalendar_pref.view_classroom) {
	    		if(view.name != "agendaDay" || event.allDay == true)
	    			startTime ='<span class="titletimestyle' + timeLegend + '">' + eventStartTime + '</span>';
	    		tagElement = (callFrom != undefined && callFrom == 'more_popup') ? 'div' : 'a';
	    		actionLink = (event.editable == 1) ? 'administration/learning/course-class/ajax/addedit/' + event.course_id + '/' + event.class_id : 'administration/view-class/ajax/addedit/' + event.class_id;
	        	modalStyle = (event.editable == 1) ? 'ctools-modal-ctools-admin-course-class-addedit-style' : 'ctools-modal-ctools-viewscreen-wrapper';
	            titleLink += '<span class="vtip" title="' + toolStartTime + ' ' + event.title +  '"><'+ tagElement +' data-event="'+ event.id + '" onclick="calendarallpopupclose(\'ilt-cls\');addAdminPageClasses(\'ilt-cls\');setCloseHandler(\'edit\');" draggable=true class="'+allDayEvent+' titlelinkstyle ilt-cls narrow-search-actionbar-textbtn addedit-form-expertusone-throbber use-ajax ajax-processed '+ modalStyle +'"  id="visible-ctools-' + event.type + '-' + event.class_id + '"  href="/?q='+ actionLink +'" data-wrapperid="paint-narrow-search-results">' +startTime+'<span  class="link">' + shortTitle + '</span>' + timeLegend;
	            titleLink += "</span></" + tagElement + "></span>";
	
	        }
	        break;
	    case 'lrn_cls_dty_vcl':
	    	if (admincalendar_pref.view_virtualclassroom) {
	    		if(view.name != "agendaDay" || event.allDay == true)
	    			startTime ='<span class="titletimestyle' + timeLegend + '">' + eventStartTime + '</span>';
	    		tagElement = (callFrom != undefined && callFrom == 'more_popup') ? 'div' : 'a';
	        	actionLink = (event.editable == 1) ? 'administration/learning/course-class/ajax/addedit/' + event.course_id + '/' + event.class_id : 'administration/view-class/ajax/addedit/' + event.class_id;
	        	modalStyle = (event.editable == 1) ? 'ctools-modal-ctools-admin-course-class-addedit-style' : 'ctools-modal-ctools-viewscreen-wrapper';
	            titleLink += '<span class="vtip" title="' + toolStartTime + ' ' + event.title +  '"><'+ tagElement +' data-event="'+ event.id + '" onclick="calendarallpopupclose(\'vcl-cls\');addAdminPageClasses(\'vcl-cls\');setCloseHandler(\'edit\');" draggable=true class="'+allDayEvent+' titlelinkstyle vcl-cls narrow-search-actionbar-textbtn addedit-form-expertusone-throbber use-ajax ajax-processed '+ modalStyle +'"  id="visible-ctools-' + event.type + '-' + event.class_id + '"  href="/?q='+ actionLink +'" data-wrapperid="paint-narrow-search-results">' + startTime+'<span  class="link">' + shortTitle + '</span>' + timeLegend;
	            titleLink += "</span></" + tagElement + "></span>";
	
	        }
	        break;
        case 'order':
            if (admincalendar_pref.view_orders) {
            	if(view.name != "agendaDay" || event.allDay == true)
	    			startTime ='<span class="titletimestyle' + timeLegend + '">' + eventStartTime + '</span>';
            	tagElement = (callFrom != undefined && callFrom == 'more_popup') ? 'div' : 'a';
                titleLink += '<span class="vtip" title="' + toolStartTime + ' ' +event.title + '"><'+ tagElement +' data-event="'+ event.id + '" onclick="calendarallpopupclose(\'order\');addAdminPageClasses(\'order\');setCloseHandler(\'edit\');" draggable=true class="'+allDayEvent+' titlelinkstyle order narrow-search-actionbar-textbtn addedit-form-expertusone-throbber ctools-modal-ctools-admin-order_edit-style use-ajax ajax-processed"  id="visible-ctools-' + event.type + '-' + event.uc_order_id + '"  href="/?q=administration/commerce/order/ajax/addedit/' + event.uc_order_id + '" data-wrapperid="paint-narrow-search-results">' + startTime+'<span class="link">' + shortTitle + '</span>' + timeLegend;
                titleLink += "</span></" + tagElement + "></span>";

            }
            break;
        case 'report':
        	if (admincalendar_pref.view_scheduled_report) {
        		if(view.name != "agendaDay" || event.allDay == true)
	    			startTime ='<span class="titletimestyle' + timeLegend + '">' + eventStartTime + '</span>';
            	tagElement = (callFrom != undefined && callFrom == 'more_popup') ? 'div' : 'a';
            	var randId = event.report_id + '_'+ randomId(); // random number added to make unique link
            	actionLink = (event.editable == true) ? '/?q=administration/report-search/schedules/'+randId : 'javascript:void(0)';
            	titleLink += '<span class="vtip" title="' + toolStartTime + ' ' + event.title +  '"><'+ tagElement +' data-event="'+ event.id + '" onClick="setCSSForReport();" draggable=true  data-qtip-obj="{  \'holderId\': \'create-schedule-'+randId+'\', \'id\': \'create-schedule-qtip-'+randId+'\', \'placement\': \'above-or-below\', \'launchedFrom\': \'admincalendar\'}" href="'+actionLink+'" class="'+allDayEvent+' use-ajax titlelinkstyle report ajax-processed" id="report-schedule-link-'+randId+'">'+startTime+'<span class="link">' + shortTitle + '</span>';
				if($("#create-schedule-"+randId).size() ==  0)
			        $("body").append('<div id="create-schedule-'+randId+'" style="z-index:12000"><div class="schedule-qtip-popup"  style="z-index:12000"></div></div>');
                titleLink += "</span></" + tagElement + "></span>";

            }
            break;
        
    }
    titleLink += "</span>";
    return titleLink;
}


function showSessionDetailsFromCalendarSummary(object,event) {
    var data = JSON.parse((unescape($(object).attr("data"))));
    $(".today-events div.order").removeClass("order");
    $(".today-events div.ilt-cls").removeClass("ilt-cls");
    $(".today-events div.vcl-cls").removeClass("vcl-cls");
    $(".today-events div.report").removeClass("report");

    $(".tmrw-events div.order").removeClass("order");
    $(".tmrw-events div.ilt-cls").removeClass("ilt-cls");
    $(".tmrw-events div.vcl-cls").removeClass("vcl-cls");
    $(".tmrw-events div.report").removeClass("report");
    
    $(".admincalendarviewmorepopupdiv").remove();
    $(".qtip-close-button").click();
    var tooltip = '<div class="admincalendarviewmorepopupdiv" style="position:absolute;z-index:101;"></div>';
    var popupFromMouseOver = $(object).attr("autocompletemousever");
    if(popupFromMouseOver != undefined && popupFromMouseOver == "true")
	    $("#narrow-search-calendar-filter-container").append(tooltip);
	else
		$("#calendar").append(tooltip);
		
    if ($(object).find(".order").size() > 0)
        $(object).addClass("order");
    else if ($(object).find(".ilt-cls").size() > 0)
        $(object).addClass("ilt-cls");
    else if ($(object).find(".vcl-cls").size() > 0)
        $(object).addClass("vcl-cls");
    else if ($(object).find(".report").size() > 0)
        $(object).addClass("report");

    var htmlStr = "";
   // console.log("data full " +data.toSource());
    if(data.end == null || data.end == "null")
    	 data.end = "";
	
    if(data.start.length> 10)
    {
        if(data.allDay == true && data.start.substring(11,data.start.length - 3) != "00:00")
        {
        	//data.startTime = moment(data.start).format("hh:mm");//.substring(11,data.start.length - 3);
	        data.start = moment(data.start.substring(0,10)).format('MMM DD, YYYY');
	    }
	    else{
           data.start = moment(data.start.substring(0,10)).format('MMM DD, YYYY'); 
	    }
	               
    }

    if (data.type == "lrn_cls_dty_ilt" || data.type == "lrn_cls_dty_vcl") {

        htmlStr += "<div class='event_title'>" + data.title + "</div>";
        htmlStr += "<div class='event_sessions'>";
        htmlStr += "<table width='100%'>";
        
        htmlStr += "<tr>";
        htmlStr += "<td width='40%'> ";
        htmlStr += "<span class='label'>"+Drupal.t("LBL250")+":</span>";
    	htmlStr += "</td>";

   		htmlStr += "<td><span class='date'>";
        htmlStr += data.session_title;
        htmlStr += "</span></td></tr>";
        if(data.scheduled_duration !='' && data.scheduled_duration!=null && data.scheduled_duration!=undefined){
        htmlStr += "<tr>";
        htmlStr += "<td> ";
        htmlStr += "<span class='label'>"+Drupal.t("LBL248")+":</span>";
    	htmlStr += "</td>";
    	
   		htmlStr += "<td><span class='date'>";
        htmlStr += data.scheduled_duration;
        htmlStr += "</span></td></tr>";
        }
        
        htmlStr += "<tr>";
        htmlStr += "<td> ";
        htmlStr += "<span class='label'>"+Drupal.t("ACLBL0037")+":</span>";
        htmlStr += "</td>";
        htmlStr += "<td><span class='date'>";
        htmlStr += data.start;
        htmlStr += "</span> - ";
	    htmlStr += "<span class='time'> ";
	    htmlStr += calTimeFormat(data.startDate,data.startTime,true);
	    htmlStr += "</span></td>";
        htmlStr += "</tr>";
        htmlStr += "<tr>";
        htmlStr += "<td>";
        htmlStr += "<span class='label'>"+Drupal.t("ACLBL0038")+":</span>";
        htmlStr += "</td>";
        htmlStr += "<td><span class='date'>";
        
        if(data.end != null && data.end.length > 10)
        	htmlStr +=  moment(data.end.substring(0,10)).format('MMM DD, YYYY');
        else
	        htmlStr += "";

        htmlStr += "</span> - ";
	        htmlStr += "<span class='time'> ";
	        htmlStr += calTimeFormat(data.endDate,data.endTime,true);
	        htmlStr += "</span></td>";
        htmlStr += "</tr>";
       
        htmlStr += "<tr>";
        htmlStr += "<td> ";
       /* if(data.type == "lrn_cls_dty_vcl")
        {*/
	        htmlStr += "<span class='label'>"+Drupal.t("LBL297")+":</span>";
	        htmlStr += "</td>";
	        			
	   		htmlStr += "<td><span class='date'>";
    	    htmlStr += data.timezone;
        	htmlStr += "</span> ";

	   /* }
        else
        {
	        htmlStr += "<span class='label'>"+Drupal.t("Location")+":</span>";
    		htmlStr += "</td>";
			htmlStr += "<td width='45%'><span class='date'>";
	        htmlStr += data.locationname;
    	    htmlStr += "</span></td>";
   		}*/
   
        htmlStr += "<span class='time'> ";
        htmlStr += "&nbsp;";
        htmlStr += "</span></td>";

        htmlStr += "</tr>";
        
        htmlStr += "<tr>";
        htmlStr += "<td>";
        htmlStr += "<span class='label'>"+Drupal.t("Instructor")+":</span>";
    	htmlStr += "</td>";

   		htmlStr += "<td><span class='date'>";
        htmlStr += data.instructor;
        htmlStr += "</span></td></tr>";

        htmlStr += "<tr>";
        htmlStr += "<td> ";
        htmlStr += "<span class='label'>"+Drupal.t("LBL263")+":</span>";
    	htmlStr += "</td>";

   		htmlStr += "<td><span class='date'>";
        htmlStr += data.code;
        htmlStr += "</span></td></tr>";
        
        
        htmlStr += "<tr>";
        htmlStr += "<td> ";
        htmlStr += "<span class='label'>"+Drupal.t("LBL038")+":</span>";
    	htmlStr += "</td>";

   		htmlStr += "<td><span class='date'>";
        htmlStr += data.languagename;
        htmlStr += "</span> ";
   
        htmlStr += "<span class='time'> ";
        htmlStr += "&nbsp;";
        htmlStr += "</span></td>";

        htmlStr += "</tr>";
        

        htmlStr += "<tr>";
        htmlStr += "<td> ";
        htmlStr += "<span class='label'>"+Drupal.t("LBL102")+":</span>";
    	htmlStr += "</td>";

   		htmlStr += "<td><span class='date'>";
        htmlStr += data.status;
        htmlStr += "</span></td></tr>";
        

        htmlStr += "</table>";
        htmlStr += "</div>";
    }else if (data.type == "order") {
        htmlStr += "<div class='event_title'>" + data.title + "</div>";
        htmlStr += "<div class='event_sessions'>";
        htmlStr += "<table width='100%'>";
        htmlStr += "<tr>";
        htmlStr += "<td width='30%'> ";
        htmlStr += "<span class='label'>"+Drupal.t("LBL042")+":</span>";
        htmlStr += "</td>";
        htmlStr += "<td><span class='date'>";
        htmlStr += data.start;
        htmlStr += "</span> - ";
        htmlStr += "<span class='time'>";
        htmlStr += calTimeFormat(data.startDate,data.startTime,true);
        htmlStr += "</span></td>";
        htmlStr += "</tr>";
        
        htmlStr += "<tr>";
        htmlStr += "<td> ";
        htmlStr += "<span class='label'>"+Drupal.t("LBL884")+":</span>";
    	htmlStr += "</td>";

   		htmlStr += "<td><span class='date'>";
        htmlStr += data.uc_order_id;
        htmlStr += "</span></td></tr>";
        
        htmlStr += "<tr>";
        htmlStr += "<td> ";
        htmlStr += "<span class='label'>"+Drupal.t("LBL102")+":</span>";
    	htmlStr += "</td>";

   		htmlStr += "<td><span class='date'>";
        htmlStr += data.lan_order_status;
        htmlStr += "</span></td></tr>";
        
        htmlStr += "<tr>";
        htmlStr += "<td> ";
        htmlStr += "<span class='label'>"+Drupal.t("LBL040")+":</span>";
    	htmlStr += "</td>";

   		htmlStr += "<td><span class='date'>";
        htmlStr += data.currency_type+' '+data.price;
        htmlStr += "</span></td></tr>";
        
        htmlStr += "</table>";
        htmlStr += "</div>";
    }
     else if (data.type == "report") {
        htmlStr += "<div class='event_title'>" + data.title + "</div>";
        htmlStr += "<div class='event_sessions'>";
        htmlStr += "<table width='100%'>";
        htmlStr += "<tr>";
        htmlStr += "<td width='30%'> ";
        htmlStr += "<span class='label'>"+Drupal.t("ACLBL0037")+":</span>";
        htmlStr += "</td>";
        htmlStr += "<td><span class='date'>";               
        htmlStr += data.start;        
        htmlStr += "</span> - ";
        htmlStr += "<span class='time'> ";
        htmlStr += calTimeFormat(data.startDate,data.startTime,true);
        htmlStr += "</span></td>";
        htmlStr += "</tr>";
        htmlStr += "<tr><td> ";
        htmlStr += "<span class='label'>"+Drupal.t("LBL102")+":</span>";
    	htmlStr += "</td>";

   		htmlStr += "<td><span class='date'>";
        htmlStr += data.status;
        htmlStr += "</span></td></tr>";
        htmlStr += "</table>";
        htmlStr += "</div>";
    }
//Added by vetrivel.P for #0064978
    if (data.type != "location" && data.type != "instructor"){
    	
    //	calendarallpopupclose();
    var html = '<div id="narrowadmincalendarviewmorepopup" style="display:none;">';

    if(popupFromMouseOver != undefined && popupFromMouseOver == "true")
	    html += '<div class="dropdownadd-dd-list-arrow-right"></div>';
	else
	{
	    html += '<div class="dropdownadd-dd-list-arrow"></div>';   
	    html += '<a  class="qtip-close-button account-close-popup"></a>';
	}
    html += '<table cellspacing="0" cellpadding="0" id="bubble-face-table" style="width:300px;">';
    html += '<tbody>';
    html += '<tr>';
    html += '<td class="bubble-tl"></td>';
    html += '<td class="bubble-t"></td>';
    html += '<td class="bubble-tr"></td>';
    html += '</tr>';
    html += '<tr>';
    html += '<td class="bubble-cl"></td>';
    html += '<td valign="top" class="bubble-c rows">';
    html += '<div class="calviewmorepopup-content">' + htmlStr + '</div>';
    //html += ' <div class="white-btn-bg-container"><div class="white-btn-bg-left"></div><input class="white-btn-bg-middle close learner-sign-close-link"  type="button" id="edit-close"  onclick="calendarallpopupclose()"  name="op" value="Close"><div class="white-btn-bg-right"></div></div>';
    html += ' </td>';
    html += ' <td class="bubble-cr"></td>';
    html += '</tr>';
    html += '<tr>';
    html += ' <td class="bubble-bl"></td>';
    html += ' <td class="bubble-blt"></td>';
    html += ' <td class="bubble-br"></td>';
    html += '</tr>';
    html += '</tbody>';
    html += '</table>';
    html += '</div>';
    }
    if(popupFromMouseOver != undefined && popupFromMouseOver == "true")
    {
        $(".admincalendarviewmorepopupdiv").removeClass('more-list');
	    $(".admincalendarviewmorepopupdiv").addClass('left-narrow').html(html);
    	$(".admincalendarviewmorepopupdiv").css("position","absolute");
	    $(".admincalendarviewmorepopupdiv").css("top",event.pageY - 121);
    	$(".admincalendarviewmorepopupdiv").css("left", $(object).position().left - 210);
    }
    else
    {
	  	$(".admincalendarviewmorepopupdiv").removeClass('more-list');
	    $(".admincalendarviewmorepopupdiv").addClass('left-narrow').html(html);
	    $(".admincalendarviewmorepopupdiv").css("position","absolute");
	    $(".admincalendarviewmorepopupdiv").css("top",event.pageY -170);
	    $(".admincalendarviewmorepopupdiv").css("left", $(object).position().left + 189);
	}
    
    vtip();
    $('.admincalendarviewmorepopupdiv').show();

    try {
        //if (Drupal.settings.ajaxPageState.theme == "expertusoneV2") {
            $('#narrowadmincalendarviewmorepopup').toggle();
            //console.log('length ' + $('.calviewmorepopup-content').length)
            //console.log('height ' + $('.calviewmorepopup-content').height())
            if($('.calviewmorepopup-content').height() > 270 && $('.more-text').length == 0)
            	$('.calviewmorepopup-content').jScrollPane();
        /*} else {
            $('#admincalendarviewmorepopup').show();
            //$('.calviewmorepopup-content').jScrollPane();
        }*/
    } catch (e) {
        // to do
    }
    closePopupOnEscAndOutsideClick(".admincalendarviewmorepopupdiv");


}

function closePopupOnEscAndOutsideClick(cls) {
    var notH = 1,
        $pop = $newJquery(cls).hover(function() {
            notH ^= 1;
        });
    

    $newJquery(document).on('mousedown keydown', function(e) {
    	var classList = e.target.className.split(" ");
    	var isClickedFromLink = classList.indexOf('link'); // skip popup close and other when you click from link
    	if (isClickedFromLink == -1 && notH || e.which == 27) {
            $pop.hide();
            $(".today-events div.order").removeClass("order");
            $(".today-events div.ilt-cls").removeClass("ilt-cls");
            $(".today-events div.vcl-cls").removeClass("vcl-cls");
            $(".today-events div.report").removeClass("report");

            $(".tmrw-events div.order").removeClass("order");
            $(".tmrw-events div.ilt-cls").removeClass("ilt-cls");
            $(".tmrw-events div.vcl-cls").removeClass("vcl-cls");
            $(".tmrw-events div.report").removeClass("report");
        }

    });
}

function adminCalCloseHandler(context)
{
	if(context == "classaddedit")
	{
	    $.cookie("delivery_type", "");
	    $.cookie("startdate", "");
	    getCalendarData();

	
	}
}

function setCloseHandler(dt) {
	///$newJquery("#calendarprimaryview").off('mouseover');
    // // // console.log("setclose....");
    // mouseOverDelay(document.getElementById('calendarprimaryview'), null);
   // $newJquery("#calendarprimaryview .fc-more").off("mouseover");

    window.setTimeout(function() {
        // // // console.log("Close..." + $(".close").size());
        $(".ui-icon-closethick").click(function() { //span .close, 
        	//log("ui-icon-closethick clicked....");
            calendarallpopupclose();
           // $newJquery('.fc-today-button').click();
            $.cookie("delivery_type", "");
            $.cookie("startdate", "");
 			getCalendarData();

        });
        /*$(".addedit-edit-announcement-basic-cancel").live("click", function() {
 			getCalendarData();

        });
        /*$(".qtip-close-button, #cancel-scheduling").live("click",function()
        {
        	calendarallpopupclose();
        	//getCalendarData();
        });*/
        

        if ( dt!='edit') {
            $.cookie("delivery_type", dt);
        }
        


    }, 5000);

    if ( dt!='edit') {
    window.setTimeout(function() {
        if ($("#edit-from-date").size() > 0) {
            $("#edit-from-date").val($.cookie("startdate"));
        } else {
            window.setTimeout(function() {
                if ($("#edit-from-date").size() > 0) {
                    $("#edit-from-date").val($.cookie("startdate"));
                }

            }, 2000);
        }


    }, 2000);
    }



}

function displayLoadingImageForCalendar() {
    if ($("#loadingcalendarimg").size() == 0)
        $("body").append("<div id='loadingcalendarimg' style='display:block; position:absolute; left:54%; top:0; margin-top:420px; background:none; padding:10px; z-index:1;'><img src='/sites/all/themes/core/expertusoneV2/expertusone-internals/images/loader.gif'></img></div>");
    else
        $("#loadingcalendarimg").show();

}

function hideLoadingImageForCalendar() {
    $("#loadingcalendarimg").hide();
}

function displayReportsRowsForAC(data)
{
	 //$("#popupcontent").html("");
            adminCalDestroyLoader()
            var reportdata = JSON.parse(data);
            
            var html = "";
            html += '<div class="reports">';
            if(reportdata.length > 0){
            html += '<h4>'+Drupal.t("REPORTS") +': </h4>';
	            //  // // // console.log(data);
	            for (var i = 0; i < reportdata.length; i++) {
	                var row = reportdata[i];
	                // console.log("row.title:"+row.title+":");
	                if(row.title != null && row.title != "null" && row.title != "")
	                {
		                $("body").append('<div id="create-schedule-' + row.id + '" class="create-schedule-popup"></div>');
	    	            html += '<div class="links header-popup" >';
	        	        html += '<a onClick="setCSSForReport();" id="report-schedule-link-' + row.id + '" data-wrapperid="paint-narrow-search-results" class="createnewclass use-ajax report-schedule-region ajax-processed"  href="?q=administration/report-search/schedules/' + row.id + '" data-qtip-obj="{  \'holderId\': \'create-schedule-' + row.id + '\', \'id\': \'create-schedule-qtip-' + row.id + '\', \'placement\': \'above-or-below\', \'launchedFrom\': \'admincalendar\'}"><span class="report-list-icon"></span><span class="vtip" title="' + row.title + '"><span>' + row.title + '</span></span></a>';
	            	    html += '</div>';
	            	}
	            }
            }else{
            	 html += '<div class = "no-records">';
     	        html += Drupal.t("No published reports are present in the system");
         	    html += '</div>';
            }
            $("#popupcontent").html(html);
            $(".reports").css("height", "200px");
            $(".reports").css("width", "235px");
            $(".reports").css("overflow", "auto");
            $(".reports").jScrollPane({
                width: "1px"
            });
            attachEvents('.createnewclass');
            vtip();
            //closePopupOnEscAndOutsideClick(".create-schedules");
}
function displayReportsForCalendar() {
    //calendarallpopupclose(\'schedule\');
    $("body").removeClass("page-administration-learning-catalog page-administration-manage-announcement page-administration-commerce page-administration-commerce-order page-reports section-administration");
    //$("body").addClass("page-reports");
    adminCalCreateLoader();
    var startDate = $newJquery('#calendarnarrowview').fullCalendar('getDate');
	    $.ajax({
    	    url: "/?q=administration/viewreportsforcalendar",
        	METHOD: "post",
	        data: "startDate=" + startDate,
    	    success: function(data) {
	    	    adminCalDestroyLoader()
        	   	displayReportsRowsForAC(data);
	        }
    	});
    }

function changeOrderAction(object,event) {
    $(".orderlinks").css("top",event.pageY);
    $(".orderlinks").css("left",event.pageX - 80);
    $(".orderlinks").css("display","block");
    var dd_selctor = "dd-"+$(object).attr("data");
    $("."+dd_selctor).css("display","block");
    
	
    
}

function applyOrderAction(id, status,frstartDate) {
	revertFuncDummy();
	$(".orderlinks").css("display","none");
	var payStatus = '';
	switch (status) {
		case 'approve':
			payStatus = 'payment_received';
			break;
		case 'cancel':
			payStatus = 'canceled';
			break;	
		default:
			payStatus = '';
			break;
	}
	adminCalCreateLoader();
	 	$.ajax({
				type: "GET",
				url: '/?q=administration/calendar/order-update/'+id+'/'+payStatus,
				success: function(result){
					adminCalDestroyLoader()
					getDataViewReportOrderPopup('',frstartDate,'');
					if (result == 'session_out') {
						self.location='?q=user';
						return;
					}
				},failure: function(res){
					adminCalDestroyLoader()
				}
			});
	}

function magnifyCell(object) {
    // // // console.log("Object:" + $newJquery(object).html());
}


function removeHTMLTags(data) {
    var rex = /(<([^>]+)>)/ig;
    if(data != null)
	    return data.replace(rex, "");
	return data;
}

function displayShortTitle(title, count) {
    var shortTitle = "";
    if(title == null)
    	return "";
    shortTitle = titleRestrictionFadeoutImage(title,'admin-cal-left-title');
    return shortTitle;
}


function createOrderOrDisplayDiscussionFromAdminCalendar(object)
{
	calendarallpopupclose();
	if($(object).attr("data") == "order")
		myWindow = window.open('/?q=administration/order/create','myWindow', "width=1200, height=800, scrollbars=1");
	else
		myWindow = window.open($(object).attr("href"),'myWindow', "width=1200, height=600, scrollbars=1");

	var intervalID = window.setInterval(function(){
		if (myWindow && myWindow.closed) {
        	window.clearInterval(intervalID);
			getCalendarData();
	    }
	}, 500);


}


function admincalendarmouseover(mouseEvent) {
			//calendarallpopupclose();
								//		return false;
        	 var days = $newJquery('#calendarprimaryview .fc-day');
             for(var i = 0 ; i < days.length ; i++) {
                var day = $newJquery(days[i]);
                var mouseX = mouseEvent.pageX;
                var mouseY = mouseEvent.pageY;
                var offset = day.offset();
                var width  = day.width();
                var height = day.height();
                if (   mouseX >= offset.left && mouseX <= offset.left+width 
                    && mouseY >= offset.top  && mouseY <= offset.top+height ){
               	  var eventCount = 0;		
                   	  $newJquery('#calendarprimaryview').fullCalendar('clientEvents', function(event) {
						 // // // console.log("event.start="+event.start.format("MM-DD-YYYY")+"=="+moment(day.attr("data-date")).format("MM-DD-YYYY"));
               			 if(event.start.format("MM-DD-YYYY") == moment(day.attr("data-date")).format("MM-DD-YYYY")) 
               			 {
               			 		eventCount++;
               		        }
               		    
			        	});
                   	if(eventCount > 3)
                   	{	
						$newJquery(".admincalendarviewmorepopupdiv").remove();
					    var tooltip = '<div class="admincalendarviewmorepopupdiv" style="position:absolute;z-index:15001;"></div>';
					    $newJquery("body").append(tooltip);
					    $newJquery(".admincalendarviewmorepopupdiv").css("top", offset.top - 10);
   	    	 		    $newJquery(".admincalendarviewmorepopupdiv").css("left", offset.left - 10);
   		     		    $newJquery(".admincalendarviewmorepopupdiv").css("display", "block");
					    $newJquery(".admincalendarviewmorepopupdiv").html("");
					    displayMorePopup(day,mouseEvent);
				   }
                   }
               }
               attachEvents('#calendarprimaryview .fc-title a');
            attachEvents(".order_row");
        	

}

function getCalendarCellAddImg(cls,date)
{
	if(date == undefined)
		date = "";
	 var addImg = "<a class='add_img "+cls+" vtip' href='javascript:void(0)' data-date='"+date+"' title='"+Drupal.t("ACLBL0036")+"' onclick='addObjectThroughAdmCalendar(this,event);'><span class='add-icon'>A </span></a>";
	 return addImg;
	 
 
}

function getCalendarCellListImg(cls, date)
{
	if(date == undefined)
		date = "";
	var listImg = "<a class='list_img "+cls+" vtip' href='javascript:void(0)' data-date='"+date+"' onclick='listObjectThroughAdmCalendar(this,event);' title='"+Drupal.t("ACLBL0029")+"' ><span class='list-icon'> L</span></a>";
	return listImg;
	           
}			


               

var mouseOverDelay = function (elem, callback) {
    var timeout = null;
    
    
 $newJquery("#calendarprimaryview .fc-more").on("mouseover",function(event){
 	// console.log("mouseover");
        globalMouseEvent = event;
        if(timeout == null)
        {
    		var obj = $newJquery(this);
	        timeout =setTimeout(function(){
    	        obj.click();
        	    return false;
            	},500);
        	}
        	
		}); 


$newJquery("#calendarprimaryview .fc-more").on("mouseout",function(event){
		//console.log("clear timeout");
		if(timeout != null)
		{
	        clearTimeout(timeout);
	        timeout = null;
	    }
 });
   /* elem.onmouseover = function(mouseEvent) {
        // Set timeout to be a timer which will invoke callback after 1s
        var pageX = mouseEvent.pageX;
        var pageY = mouseEvent.pageY;
        $(elem).attr("pageX",pageX);
        $(elem).attr("pageY",pageY);
        timeout = setTimeout(function(){
		var pageX = $("#calendarprimaryview").attr("pageX");
		var pageY = $("#calendarprimaryview").attr("pageY");
        // console.log("pageX:"+pageX+"===pageY:"+pageY);

		if(callback != undefined || callback != "" || callback != null)
	        callback({"pageX":pageX,"pageY":pageY});
        }, 500);
    };

    elem.onmouseout = function() {
        // Clear any timers set to timeout
        clearTimeout(timeout);
    } */
};


function setCSSForReport() {
	$(".admincalendarviewmorepopupdiv, #admincalendarviewmorepopup").css("z-index", "1001");	
    $("body").addClass("page-reports");
    setCloseHandler("");
}

function adminCalendarPopup(title, msgTxt){
	try{
		$('#admincalendar-wizard').remove();	    
	    var html = '';
	    
	    html += '<div id="admincalendar-wizard" style="display:none; padding: 0px;">';
	    html += '<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html += '<tr><td height="30"></td></tr>';
	    html += '<tr>';
	    html += '<td align="center" class="commanTitleAll">'+msgTxt+'</td>';
	    html += '</tr>';
	    html += '</table>';
	    html += '</div>';
	    $("body").append(html);
	    $("#admincalendar-wizard").dialog({
	        position	:[(getWindowWidth()-500)/2,100],
	        bgiframe	: true,
	        width		: 500,
	        resizable	: false,
	        modal		: true,
	        title		: SMARTPORTAL.t(title),
	        closeOnEscape : false,
	        draggable	: true,
	        overlay		: {
	           				opacity: 0.9,
	           				background: "black"
	         			  }
	    });	
	  
	    $("#admincalendar-wizard").show();
	    changeDialogPopUI();
	    $("#admincalendar-wizard").after('<div class="popupBotLeftCorner ui-helper-clearfix"><div class="popupBotRightCorner"><div class="popupBotmiddle"></div></div></div>');
	    $('.ui-dialog-titlebar-close').click(function(){
	        $("#admincalendar-wizard").remove();
	    });
	    var chgHeight = parseInt($('#admincalendar-wizard').css('min-height')) - 30;
	    // console.log('chgHeight' + chgHeight);
	    $('#admincalendar-wizard').css('min-height', chgHeight);
	}catch(e){
		// to do
		// console.log(e);
	}
}


function callAjaxForIE(url, callback){
    var xmlhttp;
    // compatible with IE7+, Firefox, Chrome, Opera, Safari
    xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function(){
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200){
            callback(xmlhttp.responseText);
        }
    }
    xmlhttp.open("GET", url, true);
    xmlhttp.send();
}


function eventDropErrorMsg(event, delta, revertFunc)
{
		adminCalendarPopup("Alert",Drupal.t("ACLBL0033"));
		revertFunc();
		return false;
}

function eventCanBeDisplayed(event)
{
    var admincalendar_pref = $.cookie("admincalendar_pref");
    admincalendar_pref = JSON.parse(admincalendar_pref);

    switch (event.type) {
        case 'order':
            if (admincalendar_pref.view_orders) 
				return true;
            break;
        case 'event_regend':
            if (admincalendar_pref.view_events) 
				return true;
            break;
        case 'event_regcancel':
            if (admincalendar_pref.view_events) 
            	return true;
            break;
        case 'lrn_cls_dty_ilt':
        	if (admincalendar_pref.view_classroom) 
        		return true;
            break;
        case 'lrn_cls_dty_vcl':
        	if (admincalendar_pref.view_virtualclassroom) 
        		return true;	
			break;
        case 'cre_sys_obt_anm':
            if (admincalendar_pref.view_announcements) 
            	return true;
            break;
        case 'report':
            if (admincalendar_pref.view_scheduled_report) 
            	return true;
            break;
            
    	case 'location':
            if (admincalendar_pref.view_locations) 
            	return true;
            break;
            //Added by Vetrivel.P for #0064978
    	case 'instructor':
            if (admincalendar_pref.view_instructors) 
            	return true;
            break;
    }
    return false;
    
}

function eventClass(event)
{
	var elemClass = 'event-default'
	switch (event.type) {
        case 'order':
        	elemClass = 'event_order';
            break;
        case 'event_regend':
        	elemClass = 'event_redend';
	        break;
        case 'event_regcancel':
        	elemClass = 'event_regcancel';
            break;
        case 'lrn_cls_dty_ilt':
        	elemClass = 'event_ilt';
            break;
        case 'lrn_cls_dty_vcl':
        	elemClass = 'event_vcl';	
			break;
        case 'cre_sys_obt_anm':
        	elemClass = 'event_announcement';
            break;
        case 'report':
        	elemClass = 'event_report';
            break;
       case 'location':
    		elemClass = 'event_location';
            break;
    }
    return elemClass;
}

function randomId()
{
    var text = "";
    var possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    for( var i=0; i < 5; i++ )
        text += possible.charAt(Math.floor(Math.random() * possible.length));

    return text;
}

function revertFuncDummy() {
	closeDragConfirmWizard();
}
function showHideLabelFocus(){
	$("#narrow-search-calendar-filter").addClass("admincalendar_search_textbox");
	var lblval = $('#narrow-search-calendar-filter').val();
	if(lblval == Drupal.t("LBL304"))
		 $('#narrow-search-calendar-filter').val('')
}
function showHideLabelBlur(){

	var lblval = $('#narrow-search-calendar-filter').val();

	if(lblval == '' || lblval ==Drupal.t("LBL304"))
	{
		 $('#narrow-search-calendar-filter').val(Drupal.t("LBL304"))
		 $("#narrow-search-calendar-filter").removeClass("admincalendar_search_textbox");
	}
}
function moreClassSearchHideShow() {
	var admincalendar_pref =jQuery.parseJSON($.cookie('admincalendar_pref'));
	if((admincalendar_pref.view_classroom == false) && (admincalendar_pref.view_virtualclassroom == false) &&
		(admincalendar_pref.view_scheduled_report == false) && (admincalendar_pref.view_orders == false || ADMIN_CALENDAR_PERM_LIST.commerce == false))
		{
		return ;
		}
	reArangeSearchFilter();
	$("#narrow-search-calendar-filter").flushCache();
    $('#select-list-calendar-dropdown-list').slideToggle();
    $('#select-list-calendar-dropdown-list li:last').css('border-bottom','0px none');
    
	}

function moreClassSearchTypeText(dCode,dText) {
	dCode = unescape(dCode);
	dCode =$.trim(dCode);
	
    $('#select-list-calendar-dropdown-list').hide();
    $('#select-list-calendar-dropdown').attr('title',dCode);
	
	//Added/Changed by ganesh on May 9th 2017 for #73716
    if(dCode.length >= 13){
	    $('#select-list-calendar-dropdown').html(dCode+'<span class="fade-out"></span>');
	}else{	
		$('#select-list-calendar-dropdown').html(dCode);
	}
	
	//$('#select-list-calendar-dropdown').html(dCode);
	
    $('#search_all_calendar_type-hidden').val(dText);
}
$(function() {

$("#narrow-search-calendar-filter").focus(function(event){
	$("#select-list-calendar-dropdown-list").css("display","none");
});
$("#narrow-search-calendar-filter").keyup(function(event){
	try{
	  if(event.keyCode == 13){
		  var search_title = $('#narrow-search-calendar-filter').val();
		  var langtitle = Drupal.t("LBL304").toUpperCase();
		  if((search_title.toLowerCase()) == (langtitle.toLowerCase()))
			  search_title='';
		  if(search_title!=''){
			  $.ajax({
		            url: "/?q=administration/calendarviewapi",
	  	        METHOD: "post",
	      	    data: "search_type=" + $("#search_all_calendar_type-hidden").val() + "&search_text=" + search_title,
	          	success: function(data) {
	              	adminCalDestroyLoader()
	                calendardata = JSON.parse(data);
		            $newJquery('#calendarprimaryview').fullCalendar('removeEvents');
	  	            $newJquery('#calendarnarrowview').fullCalendar('removeEvents');
					updateAdminCalendarEvents(calendardata);
	          	//    $newJquery('#calendarprimaryview').fullCalendar('rerenderEvents');
	             // 	$newJquery('#calendarnarrowview').fullCalendar('rerenderEvents');
	                // $newJquery("#calendarprimaryview  .fc-prev-button").attr("ajaxrequired", "false");
	  	            //prevNarrowCalendarNavigation();
	  	            //attachEvents('#calendarprimaryview .fc-title a'); 
	       	   },
	          	failure:function(data)
	              {
		            	adminCalDestroyLoader()
	  	        }
	      	});
		  }
	   }
	}catch(e){
		// to do
	}
	});

$('#narrow-search-calendar-filter-go').click(function() {
	try{
		var search_title = $('#narrow-search-calendar-filter').val();
		var langtitle = Drupal.t("LBL304").toUpperCase();
		var AC_SEARCH_PARAMS = "";
		//if ($("#search_all_calendar_type-hidden").val() == Drupal.t('LBL1039') && search_title == Drupal.t('LBL304')) {
		if (search_title == Drupal.t('LBL304')) {
			  //$newJquery('.fc-today-button').click();
			  	var ac_searchparams = getACSearchParams();
				startDate = $newJquery("#calendarprimaryview").fullCalendar('getView').start.format("YYYY-MM-DD");
				endDate = $newJquery("#calendarprimaryview").fullCalendar('getView').end.format("YYYY-MM-DD");
			 	AC_SEARCH_PARAMS ="startDate=" + startDate + "&endDate=" + endDate+ac_searchparams;
			 // return;
		}else{
			AC_SEARCH_PARAMS =	"omitdatefilter=true&search_type=" + $("#search_all_calendar_type-hidden").val() + "&search_text=" + $("#narrow-search-calendar-filter").val();
		}
		
		var searchtimeplace='', searchtimeposition='0';	
		var grepcalendardata='';
		var calendarView=$newJquery("#calendarprimaryview").fullCalendar('getView').name;

			adminCalCreateLoader();
			$.ajax({
	            url: "/?q=administration/calendarviewapi",
		        METHOD: "post",
		        data: AC_SEARCH_PARAMS,//"search_type=" + $("#search_all_calendar_type-hidden").val() + "&search_text=" + $("#narrow-search-calendar-filter").val(),
		        timeout:Drupal.settings.cal.admin_calendar_search_max_time,
		        success: function(data) {
		          	adminCalDestroyLoader()
		            calendardata = JSON.parse(data);
		            if(calendarView=='agendaDay' && calendardata.length > 0){		            			            	
		            	grepcalendardata = jQuery.grep(calendardata, function(element, index){							
  							return element.allDay==false; // retain appropriate elements
						});	
		            	if(grepcalendardata.length > 0)
		            		searchtimeplace=((moment(grepcalendardata[0].start).format('h:mm a').replace(/ /g,'')).replace(':00', '')).replace(':15', ':30').replace(':45', ':30');
						
		            		$('.fc-time-grid-container').scrollTop(0);
		            $('.fc-time-grid-container .fc-slats table tr').each(function(){   
		               	  if($(this).find('.fc-axis span').eq(0).text() == searchtimeplace){

	           			 	searchtimeposition = parseInt($(this).offset().top) - parseInt(294);
	        			  }
        			 });
        			}
		            $newJquery('#calendarprimaryview').fullCalendar('removeEvents');
			        $newJquery('#calendarnarrowview').fullCalendar('removeEvents');
					updateAdminCalendarEvents(calendardata);
		     // 	    $newJquery('#calendarprimaryview').fullCalendar('rerenderEvents');
		      //    	$newJquery('#calendarnarrowview').fullCalendar('rerenderEvents');
		          	//alert(calendardata[0].start);

					if(calendardata.length > 0){
					   var selectedDate = moment(calendardata[0].start.substring(0,10));
			   		   formatCalendarDataSummary(selectedDate.format("YYYY-MM-DD"),'',selectedDate);
					}
			       $newJquery("#calendarprimaryview  .fc-prev-button").attr("ajaxrequired", "false");
				        //prevNarrowCalendarNavigation();
				   attachEvents('#calendarprimaryview .fc-title a');
		          	if(calendardata.length > 0 && (search_title.toLowerCase()) != (langtitle.toLowerCase()) )
		          	{
		                $newJquery("#calendarprimaryview").fullCalendar("gotoDate", moment(calendardata[0].start.substring(0,10)));		          	
		                $newJquery("#calendarnarrowview").fullCalendar("gotoDate", moment(calendardata[0].start.substring(0,10)));		          	
		            }
		          	if(calendarView=='agendaDay' && calendardata.length > 0){
		          		 setTimeoutConst = setTimeout(function(){
		          		 	setscrollpostion(searchtimeposition);
      					}, 300);
		          	}
		        },error: function(xhr, status, message) {
			        if(status == "timeout") {
			        	adminCalDestroyLoader();
			        	var error = new Array();
						error[0] = Drupal.t("ERR1762");
			        	$('#show_expertus_message').html(expertus_error_message(error,'error'));
						$('#show_expertus_message').show();
			       }
			   },failure:function(data)
		       {
	            	adminCalDestroyLoader()
		       }
			});
		//}
	}catch(e){
		// to do
	}
});
if($('#narrow-search-calendar-filter')) {
	try{
	format_ac_item = function (item, position, length){ 
	var jsonrow = JSON.parse(item);
	return  "<div class='list-item' autocompletemousever='true' data=\""+escape(escape(JSON.stringify(jsonrow)))+"\" onMouseover='showSessionDetailsFromCalendarSummary(this,event);'><span title='"+ jsonrow.title +"'>" + jsonrow.title + "</span></div>";//<span class='time'>" + jsonrow.startTime + "</span>
  } 
	$('#narrow-search-calendar-filter').autocomplete(
			"/?q=administration/calendar-autocomplete",{
			minChars :3,
			max :50,
			autoCompleteTimeOut: Drupal.settings.cal.admin_calendar_search_max_time,
			errMsg:Drupal.t("ERR1762"),
			autoFill :true,
			mustMatch :false,
			matchContains :false,
			inputClass :"ac_input",
			loadingClass :"ac_loading",
			formatItem:format_ac_item,
            error: function () {   alert("Request time out"); if(status == "timeout") {        alert("Request time out1");    } },
			select: function (event, ui){alert("|" + $("#targetID").val() + "|1stAlert");},
            error: function () {       response([]);       },
			extraParams: {calendar_searchtype: function() {return $("#search_all_calendar_type-hidden").val()}}
	}).result(function(event, data, formatted) {	
		
		var title = JSON.parse(data).title;
		//if(title != null && title.indexOf("-") > 0)
			//title = $.trim(title.substring(0,title.indexOf("-")));

		//$("#narrow-search-calendar-filter").val(title);
		$("#narrow-search-calendar-filter").val($('<div/>').html(title).text());
		//$('#narrow-search-calendar-filter-go').click();
		
	});		
	}catch(e){
		// to do
	}
}
});


function setscrollpostion(searchtimeposition){
	$('.fc-time-grid-container').scrollTop(searchtimeposition);
}

function eventDropUpdate(event, delta, revertFunc) {
		// console.log(event);
        var view = $newJquery('#calendarprimaryview').fullCalendar('getView');
        closeDragConfirmWizard();
        if(view.name == "agendaDay")
        {
        	if(event.type == "event_regend" || event.type == "event_regcan" || event.type == "order" || event.type == "report")
        	{
    		adminCalendarPopup("Calendar",Drupal.t("ACLBL0039"));
            	revertFunc();
    	    	return false;
    
            	
        	}
     		if( (event.allDay == true || (event.end == null || event.end == "")))
			{
			adminCalendarPopup("Calendar",Drupal.t("ACLBL0039"));
            	revertFunc();
        	    return false;	
			}
        	else
        	{
            	var startDate = event.start.format("MM-DD-YYYY");
	        	var endDate = event.end.format("MM-DD-YYYY");

   	    	 	if(startDate != endDate)
    	    	{
					adminCalendarPopup("Calendar",Drupal.t("ACLBL0039"));
            		revertFunc();
        	    	return false;	
	        	}
	        }
        }

		if( event.type == "order" || event.type == "report")
		{
				adminCalendarPopup("Calendar",Drupal.t("ACLBL0021"));
            	revertFunc();
    	    	return false;
		} 
        var dateType = isDatePastTodayFuture(event, 'update');
        if(dateType == "fc-past" && event.type != "lrn_cls_dty_ilt" && event.type != "lrn_cls_dty_vcl" && event.type != "event_regend"){
            adminCalendarPopup("Calendar",Drupal.t("ACLBL0021"));
        	revertFunc();
        	return "";
        }
       
        else if(event.editable != null && event.editable == 1)
        {
             calendarUpdateData(event,revertFunc);
        }
        else
        {
        	eventDropErrorMsg(event, delta, revertFunc);

        }
}
function checkSessionOut(){
	var stdid		= obj.getLearnerId();
	if(stdid == "0" || stdid == "")
	{
	  	 self.location='/';
	  	 return false;
	}
}
function getEventbyId(data, code) {
	  return data.filter(
	      function(data){return data.id == code}
	  );
}   

function translateTimingStr (time) {
  if (time == '') 
	  return time;
  if(time.indexOf("AM") > 0){
		timing = time.replace("AM", Drupal.t("LBL863").toUpperCase());
	}else if(time.indexOf("PM") > 0){
		timing = time.replace("PM", Drupal.t("LBL864").toUpperCase());
	}else{
		timing = time;
	}
	return timing;
} 

function displayDragConfirm(title, event, delta, revertFunc, fdate){
	try {
		var uniqueClassPopup = '';
		var currTheme = Drupal.settings.ajaxPageState.theme;
		var wSize = (wSize) ? wSize : 300;
		var closeButAction = '';

		$('#delete-msg-wizard').remove();
		if(revertFunc == 'order_update') {
			title = title;
		}else{
			title = title+''+'?';
		}
	    var html = '';
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
		if(currTheme == 'expertusoneV2'){
	    		html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
	    } else {
	    		html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+'</td></tr>';
	    }
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var boxtitle ='';
	    if(revertFunc == 'order_update') {
		   closeButAction = function(){ applyOrderAction(event, delta, fdate); };
		   var closeButt={};
		   closeButt[Drupal.t('LBL109')]=function(){ closeDragConfirmWizard(); revertFuncDummy(); };
		   closeButt[Drupal.t('Yes')]= closeButAction;
		   boxtitle = Drupal.t('LBL820')+' - '+event;
		   
		} else {
		   closeButAction = function(){ eventDropUpdate(event, delta, revertFunc); };
		   var closeButt={};
		   closeButt[Drupal.t('LBL109')]=function(){ closeDragConfirmWizard(); revertFunc(); };
		   closeButt[Drupal.t('Yes')]= closeButAction;
		   boxtitle = Drupal.t('LBL943') + ' ' + Drupal.t('LBL042');
	   }
	    $("#delete-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:wSize,
	        minHeight:"auto",
	        resizable:false,
	        modal: true,
	        title: boxtitle,//"title",
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
	        if(revertFunc == 'order_update') {
	        	revertFuncDummy();
	        }else{
	        	revertFunc();
	        }	        	
	    });
		if(currTheme == 'expertusoneV2'){
	    	changeDialogPopUI();
	    }
		$("#delete-msg-wizard .commanTitleAll").css('padding','30px 24px');
		/*-- 37211: Unable to delete a class in FF version 32.0 --*/
	    if($('div.qtip-defaults').length > 0) {
	    	var prevZindex = $('.qtip-defaults').css('z-index');
	    	$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
	    	$('.ui-widget-overlay').css('z-index', prevZindex+1);
	    }
	}catch(e){
		// to do
	}
}

function closeDragConfirmWizard() {
	$('#delete-msg-wizard').dialog('destroy');
	$('#delete-msg-wizard').dialog('close');
	$('#delete-msg-wizard').remove();
}

function attachMouseOver() {
    // Set time out event
    var delay=1000, setTimeoutConst;
    $("#calendarprimaryview .fc-more").hover(function(){
      setTimeoutConst = setTimeout(function(){
        /* Do Some Stuff*/
        $(this).append( $( "<span> ***</span>" ) );
        $(this).click();
      }, delay);
    },function(){
      clearTimeout(setTimeoutConst );
    })
}
function calculateTimeDiff(val) {
   var times =  val.split(" - ");
   var startTime = times[0];
   var endTime = times[1];
   var startTimeArray = startTime.split(":");
   var startInputHrs = parseInt(startTimeArray[0]);
   var startInputMins = parseInt(startTimeArray[1]);

   var endTimeArray = endTime.split(":");
   var endInputHrs = parseInt(endTimeArray[0]);
   var endInputMins = parseInt(endTimeArray[1]);

   var startMin = startInputHrs*60 + startInputMins;
   var endMin = endInputHrs*60 + endInputMins;

   var result;

   if (endMin < startMin) {
       var minutesPerDay = 24*60; 
       result = minutesPerDay - startMin;  // Minutes till midnight
       result += endMin; // Minutes in the next day
   } else {
      result = endMin - startMin;
   }

   var minutesElapsed = result % 60;
   var hoursElapsed = (result - minutesElapsed) / 60;
   return hoursElapsed + ":" + (minutesElapsed < 10 ? '0'+minutesElapsed : minutesElapsed);
}
function calTimeFormat(fdate,startTime,fullfrmt,summary){
   moment.locale('en-us');
   var from_date = (fdate == undefined || fdate == 'undefined') ? moment.utc().format('YYYY-MM-DD') : fdate;
   var eventStartDateTime = (summary == 'undefined' || summary == undefined) ? moment(from_date+'T'+startTime+':00').format('h:mm a') 
   							: moment(from_date+'T'+startTime+':00').format('hh:mm a');
   if(summary == 'undefined' || summary == undefined)
   var edatetime = eventStartDateTime.replace(':00', '');   
   else
   	var edatetime = eventStartDateTime;  
   	//lang = getCurrentLanguage();
    //moment.locale(lang);
   if(fullfrmt == true)
	   return edatetime;
   
   return edatetime.slice(0, -1).replace(/ /g,''); 
}

function removeExistingEvents(newEvents, oldEvents) {
	for( var i=newEvents.length - 1; i>=0; i--){
	 	for( var j=0; j<oldEvents.length; j++){
	 	    if(newEvents[i] && (newEvents[i].id === oldEvents[j].event.id)){
	 	    	newEvents.splice(i, 1);
	    	}
	    }
	}
	return newEvents;
}

function adminCalCreateLoader(loaderdiv) {
	loaderdiv = typeof loaderdiv !== 'undefined' ? loaderdiv : 'calendar';
	obj.createLoader(loaderdiv);
}

function adminCalDestroyLoader(loaderdiv) {
	loaderdiv = typeof loaderdiv !== 'undefined' ? loaderdiv : 'calendar';
	obj.destroyLoader(loaderdiv);
	if($('#loaderdivmain-wrapper').length > 0)
		obj.destroyLoader('main-wrapper');
}