(function($) {
	var loadObj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
	jQuery.fn.expertusCalendar = function (aDefn) {	
		return this.each(function(){
			
			$.fn.expertusCalendarXmlParser(this.id,aDefn);
		});
   };
   
   /*jQuery.fn.expertusCalendarRender = function (objId,startDates,calType) {
	   var dispDay = [SMARTPORTAL.t('Sunday'),SMARTPORTAL.t('Monday'),SMARTPORTAL.t('Tuesday'),SMARTPORTAL.t('Wednesday'),SMARTPORTAL.t('Thursday'),SMARTPORTAL.t('Friday'),SMARTPORTAL.t('Saturday')];
	   if(calType != undefined && calType == 'isHomePage'){
		   dispDay = [SMARTPORTAL.t('Su'),SMARTPORTAL.t('Mo'),SMARTPORTAL.t('Tu'),SMARTPORTAL.t('We'),SMARTPORTAL.t('Th'),SMARTPORTAL.t('Fr'),SMARTPORTAL.t('Sa')];
	   }

	   this.startDates = startDates;
	   $('#'+objId).datepicker(
		{
			changeMonth: true,
			changeYear : true,
			changeFirstDay:false,
			selectMultiple:true,
			dayNamesMin:dispDay,
			monthNames:[SMARTPORTAL.t('January'), SMARTPORTAL.t('February'), SMARTPORTAL.t('March'), SMARTPORTAL.t('April'), SMARTPORTAL.t('May'), SMARTPORTAL.t('June'), SMARTPORTAL.t('July'), SMARTPORTAL.t('August'), SMARTPORTAL.t('September'), SMARTPORTAL.t('October'), SMARTPORTAL.t('November'), SMARTPORTAL.t('December')],
			showOtherMonths:false,
			dateFormat: 'd/M/Y',
			beforeShowDay:function(date){
				return $.fn.expertusCalendarBeforeShowDay(date,startDates,calType);
			},
			onChangeMonthYear:function(year, month, inst) {				
				setTimeout('$.fn.expertusCheckDatePickerLoaded(\"'+ calType +'\")', 50);	
			}
		
		});	   
	   $.fn.expertusDatepickerCSSUpdate(calType,startDates);
	   vtip();
   };*/

   jQuery.fn.expertusCheckDatePickerLoaded = function(objId, calDefn, year, month, inst) {
	    if( $(".ui-datepicker-header").length > 0 && $(".ui-datepicker-calendar").length > 0 ){
	    	$("#actualDate").val(year+'-'+month+'-01');
			$.fn.expertusCalendarXmlParser(objId, calDefn, year, month, inst);
	    	//$('#CatalogServicetabDiv.hasDatepicker').css('display', 'block'); Moved to calendar_style.css
		} else {
			setTimeout('$.fn.expertusCheckDatePickerLoaded(\"'+ calDefn.calType +'\")', 50);		
		}
   };
   
   jQuery.fn.expertusDatepickerCSSUpdate = function(calType,startDates) {
	   try {
			  if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
				  var scrollCnt = 0;
				  var scrollCnt1 = 0;
				  $('.ui-datepicker-calendar tbody td').click(function(){
				  if($(this).hasClass("ui-datepicker-set-date")){ 
						$('#catalog-admin-cal-session-details').html("");
						$(this).css("background","#ffffff");
						$('.ui-datepicker-calendar tbody td').removeClass("selectDateTD");
					}
					else{
						$('.ui-datepicker-calendar tbody td').removeClass("selectDateTD");
						$(this).addClass("selectDateTD");
					}
				  });
				  $('.mycal').each(function(){	
					  	var obj = this;
						var sessionDet = '';
						var sessionDet1 = '';
						//var classIds = new Array();
						var eventsCode = new Array();
						var Month = $('#catalog-admin-cal .ui-datepicker-month').val();
						month = parseInt(Month)+1;
						month = ("0"+ month).slice(-2);
						var year = $('#catalog-admin-cal .ui-datepicker-year').val();
						var dateVal = (parseInt($(this).text()) < 10) ? ("0"+$(this).text()) : $(this).text();
		               
						var dateFormatted = year+"/"+month+"/"+dateVal+" "+"00:00:00";
						var inc = 0;
						 var repHtml = "";
						 var statusHtml ="";
						 var sessLen = startDates.length;
						 for(var i=0;i<startDates.length;i++){
							//if($.fn.in_array(startDates[i].classId,classIds) == false){
								 inc=inc+1;			
								   if(dateFormatted == startDates[i].eventDate){
									   //classIds.push(startDates[i].classId);
									   	   if(startDates[i].myclass=='Instructor')
									   		   statusText="Instructor";
									   	   else
									   		   statusText=startDates[i].statustext;
									   	   
										   if($.fn.in_array(startDates[i].eventCode,eventsCode) == false) {
											   if(repHtml!=""){
												   repHtml +=  "</tbody></table></td></tr></tbody></table>";
											   }
											   scrollCnt = scrollCnt + 1;
											   eventsCode.push(startDates[i].eventCode);
											   repHtml += "<table border='0' cellpadding='0' cellspacing='0' id='my-calendar-details-tab-id' class='calendar-session-details'><tbody><tr><td><div class='"+startDates[i].iconTypeClass+"'> </div></td><td><table cellpadding='0' cellspacing='0' class='calendar-session-details-list'><tbody><tr><td colspan='3' class='calendar-session-head' >";
									   repHtml += "<div class='limit-title-row'><span class='limit-title vtip' title='"+htmlEntities(startDates[i].eventTitle)+"'>"+startDates[i].eventTitle+"</span></div>";
											   if(startDates[i].mroflag!=null && startDates[i].mroflag!='' && startDates[i].myclass!='Instructor'){
												   if(startDates[i].mroflag == 'Mandatory'){
												     repHtml += "<span title="+Drupal.t('Mandatory')+" class='calendar-mandatory-bg vtip'>M</span>";
												   }else if(startDates[i].mroflag == 'Recommended'){
													 repHtml += "<span title="+Drupal.t('Recommended')+" class='calendar-recommended-bg vtip'>R</span>";   
												   }
											   }
											   repHtml += "</td></tr>";
											   var statusHtml = '';
											   if(statusText == 'Registered'){
												   statusHtml += "<td class='mycalendar-class-action registered'>"+Drupal.t("Registered")+"</td>";
											   }else if(statusText == 'Completed'){
												   statusHtml += "<td class='mycalendar-class-action completed'>"+Drupal.t("Completed")+"</td>";
											   }	
											   else if(statusText == "Instructor"){
												   statusHtml += "<td class='mycalendar-class-action instructor'>"+Drupal.t("Instructor")+"</td>";
											   }else{
												   statusHtml += "<td class='mycalendar-class-action not-registered'>"+Drupal.t("Not Registered")+"</td>";
											   } var qtipop = 'calendar-'+startDates[i].classId;
											   var disp = startDates[i].iltdateWithoutTime+" "+startDates[i].iltstartDateTime+" "+startDates[i].sessiontimezonecode+" "+startDates[i].tz_code;
											   var status = qtip_popup_paint(qtipop, disp, '', 1, 'mylearning-calendar'); 
											   repHtml += "<tr><td class='eventCodetitle'><span class='vtip' title='"+htmlEntities(startDates[i].eventCode)+"'>"+titleRestrictionFadeoutImage(startDates[i].eventCode,'calendar-js-title-restriction')+"</span></td><td class='eventCodeseperator'><span> | </span></td>"+statusHtml+"</tr>";
											   
											   if(statusText != 'Completed') {
												   repHtml += "<tr><td class='tz sample' colspan='3'>"+startDates[i].dateWithoutTime+" "+startDates[i].startDateTime.toLowerCase()+" "+startDates[i].usertimezonecode+" "+startDates[i].user_tzcode+"</td></tr>"; //71627: My Calendar - Make the following modifications in the My calendar popup
												   if (statusText == 'Not Registered' && startDates[i].sessionCount > 1) {
													   repHtml += "<tr><td class='tz sample' colspan='3'>"+startDates[i].eventEndDate.date +" "+ startDates[i].eventEndDate.time.toLowerCase()+"  </td></tr>";
												   }
											   }
											   if(startDates[i].deliveryType == 'lrn_cls_dty_ilt' && startDates[i].sessiontimezone!=startDates[i].usertimezone){
										repHtml += "<tr><td colspan='4'>"+status+"</td></tr>";
										}
											   
										  
										   } else {
												   if(startDates[i].statustext == 'Completed' || startDates[i].statustext == 'Not Registered') {
												   repHtml += " ";
											   }
											   else {
												  // repHtml += "<table border='0' cellpadding='0' cellspacing='0' class='mycalendar-tooltip'><tbody><tr><td class='mycalendar-class-schedule'>";
													//repHtml += "<tr><td colspan='3'>"+startDates[i].dateWithoutTime+" "+startDates[i].startDateTime.toLowerCase();
												  // repHtml += "<tr><td class='tz' colspan='3'>"+startDates[i].dateWithoutTime+" "+startDates[i].startDateTime+" "+startDates[i].usertimezonecode+" "+startDates[i].user_tzcode+"</td></tr>";
												repHtml += "<tr><td class='tz' colspan='3'>"+startDates[i].dateWithoutTime+" "+startDates[i].startDateTime.toLowerCase()+" "+startDates[i].usertimezonecode+" "+startDates[i].user_tzcode+"</td></tr>";
												   
												   if(startDates[i].mroflag!=null && startDates[i].mroflag!='' && startDates[i].myclass!='Instructor'){
													    if(startDates[i].mroflag == 'Mandatory'){
														     repHtml += "<span title="+Drupal.t('Mandatory')+" class='calendar-mandatory-bg vtip'>M</span>";
														  } else if(startDates[i].mroflag == 'Recommended'){
															 repHtml += "<span title="+Drupal.t('Recommended')+" class='calendar-recommended-bg vtip'>R</span>";   
														  }
												   }
											       repHtml += "</td></tr>";
											   }
										   }
										   

								   }
								  
							//}
							   
						 }	
							$(this).click(function(){
								/* $('.ui-datepicker-set-date').click(function(){
									  $('#catalog-admin-cal-session-details').html("");
									  });*/
								
								if($(this).hasClass("ui-datepicker-today")){
									var currDate = new Date();
									var strCurrDate = currDate.getFullYear() + "/" + (currDate.getMonth()) + "/" + currDate.getDate()+" "+"00:00:00";
									var inc1 = 0;
									for(var i=0;i<startDates.length;i++){
										inc1=inc1+1;					 
									  if(strCurrDate == startDates[i].eventDate){
										if(startDates[i].myclass=='Instructor')
										  statusText="Instructor";
									    else
										  statusText=startDates[i].statustext;
												   	 
										if($.fn.in_array(startDates[i].eventCode,eventsCode) == false) {
											   if(repHtml!=""){
												   repHtml +=  "</tbody></table></td></tr></tbody></table>";
											   }
											   scrollCnt = scrollCnt + 1;
											   eventsCode.push(startDates[i].eventCode);
											   repHtml += "<table border='0' cellpadding='0' cellspacing='0' id='my-calendar-details-tab-id' class='calendar-session-details'><tbody><tr><td width='53'><div class='"+startDates[i].iconTypeClass+"'> </div></td><td><table cellpadding='0' cellspacing='0' class='calendar-session-details-list'><tbody><tr><td colspan='3' class='calendar-session-head' >";
									   repHtml += "<div class='limit-title-row'><span class='limit-title vtip' title='"+htmlEntities(startDates[i].eventTitle)+"'>"+startDates[i].eventTitle+"</span>";
											   if(startDates[i].mroflag!=null && startDates[i].mroflag!='' && startDates[i].myclass!='Instructor'){
												   if(startDates[i].mroflag == 'Mandatory'){
												     repHtml += "<span title="+Drupal.t('Mandatory')+" class='calendar-mandatory-bg vtip'>M</span>";
												   }else if(startDates[i].mroflag == 'Recommended'){
													 repHtml += "<span title="+Drupal.t('Recommended')+" class='calendar-recommended-bg vtip'>R</span>";   
												   }
											   }
											   repHtml += "</td></tr>";
											   var statusHtml = '';
											   if(statusText == 'Registered'){
												   statusHtml += "<td class='mycalendar-class-action registered'>"+Drupal.t("Registered")+"</td>";
											   }else if(statusText == 'Completed'){
												   statusHtml += "<td class='mycalendar-class-action completed'>"+Drupal.t("Completed")+"</td>";
											   }	
											   else if(statusText == "Instructor"){
												   statusHtml += "<td class='mycalendar-class-action instructor'>"+Drupal.t("Instructor")+"</td>";
											   }else{
												   statusHtml += "<td class='mycalendar-class-action not-registered'>"+Drupal.t("Not Registered")+"</td>";
											   }
											   
											   repHtml += "<tr><td class='eventCodetitle'><span class='vtip' title='"+htmlEntities(startDates[i].eventCode)+"'>"+titleRestrictionFadeoutImage(startDates[i].eventCode,'calendar-js-title-restriction')+"</span></td><td class='eventCodeseperator'><span> | </span></td>"+statusHtml+"</tr>";
											   
											   if(statusText != 'Completed') {
											   }
										   } else {
												   if(startDates[i].statustext == 'Completed' || startDates[i].statustext == 'Not Registered') {
												   repHtml += " ";
											   }
											   else {
												  // repHtml += "<table border='0' cellpadding='0' cellspacing='0' class='mycalendar-tooltip'><tbody><tr><td class='mycalendar-class-schedule'>";
										   repHtml += "<tr><td colspan='3'>"+startDates[i].dateWithoutTime+" "+startDates[i].startDateTime.toLowerCase();
												   if(startDates[i].mroflag!=null && startDates[i].mroflag!='' && startDates[i].myclass!='Instructor'){
													   if(startDates[i].mroflag == 'Mandatory'){
														     repHtml += "<span title="+Drupal.t('Mandatory')+" class='calendar-mandatory-bg vtip'>M</span>";
														  }else if(startDates[i].mroflag == 'Recommended'){
															 repHtml += "<span title="+Drupal.t('Recommended')+" class='calendar-recommended-bg vtip'>R</span>";   
														  }
												   }
											       repHtml += "</td></tr>";
													   
											   }
										   }
									  }
									 }
								  }
								 $('#catalog-admin-cal-session-details').html("");
								 var calDestroy = $('.session-details-list').data('JSP');
								 if(calDestroy){
									calDestroy.destroy();
								 }
								 if(repHtml!=""){
									   repHtml +=  "</tbody></table></td></tr></tbody></table>";
								 }
								  $('#catalog-admin-cal-session-details').html("<div class='session-details-list'>"+repHtml+"</div>");						 						 
						 $('#my-calendar-details-tab-id .limit-title').trunk8(trunk8.cal_title);
								 $('#catalog-admin-cal-session-details .session-details-list').jScrollPane();
							//	 $('.jspDrag').css("height","115");
								 vtip();
							});		
							$(".ui-datepicker-today").click();
					});
				 if(($(this).hasClass("ui-datepicker-today"))) { // && scrollCnt1 > 2
					 $('#catalog-admin-cal-session-details .session-details-list').jScrollPane();
				//	 $('.jspDrag').css("height","115");
				 } 
			  }else{
			    $('.mycal').each(function(){		  
				  	var obj = this;
					var sessionDet = '';
					var sessionDet1 = '';
					//var classIds = new Array();
					var eventsCode = new Array();
					var Month = $('#catalog-admin-cal .ui-datepicker-month').val();
					month = parseInt(Month)+1;
					month = ("0"+ month).slice(-2);
					var year = $('#catalog-admin-cal .ui-datepicker-year').val();
					var dateVal = (parseInt($(this).text()) < 10) ? ("0"+$(this).text()) : $(this).text();

					var dateFormatted = year+"/"+month+"/"+dateVal+" "+"00:00:00";
					var inc = 0;
					 var repHtml = "";
					 var sessLen = startDates.length;
					 for(var i=0;i<startDates.length;i++){
						//if($.fn.in_array(startDates[i].classId,classIds) == false){
							 inc=inc+1;					 
							   if(dateFormatted == startDates[i].eventDate){
								   //classIds.push(startDates[i].classId);
								   	   if(startDates[i].myclass=='Instructor')
								   		   statusText="Instructor";
								   	   else
								   		   statusText=startDates[i].statustext;
								   	 
									   if($.fn.in_array(startDates[i].eventCode,eventsCode) == false) {
										   eventsCode.push(startDates[i].eventCode);
										   repHtml += "<table border='0' cellpadding='0' cellspacing='0' class='mycalendar-tooltip'><tr><td class='mycalendar-class-title'>";
										   repHtml += startDates[i].eventTitle+"</td>";
										   if(statusText == 'Registered'){
											   repHtml += "<td class='mycalendar-class-action registered'>"+Drupal.t("Registered")+"</td></tr><tr><td class='mycalendar-class-schedule' colspan='2'>";
										   }else if(statusText == 'Completed'){
											   repHtml += "<td class='mycalendar-class-action completed'>"+Drupal.t("Completed")+"</td></tr><tr><td class='mycalendar-class-schedule' colspan='2'>";
										   }	
										   else if(statusText == "Instructor"){
											   repHtml += "<td class='mycalendar-class-action instructor'>"+Drupal.t("Instructor")+"</td></tr><tr><td class='mycalendar-class-schedule' colspan='2'>";
										   }else{
											   repHtml += "<td class='mycalendar-class-action not-registered'>"+Drupal.t("Not Registered")+"</td></tr><tr><td class='mycalendar-class-schedule' colspan='2'>";
										   }
										   if(statusText != 'Completed') {
									   repHtml += Drupal.t("LBL763")+':'+startDates[i].startDateTime.toLowerCase()+"";
											   if(startDates[i].mroflag!=null && startDates[i].mroflag!='' && startDates[i].myclass!='Instructor')
												   repHtml += " | "+startDates[i].mroflag+"";
										   }
									   }
									   else {
										   //repHtml += "</td>";
										   /* if(statusText == 'Registered'){
											   repHtml += "<tr><td class='mycalendar-class-action registered'>"+startDates[i].statustext+"</td></tr><tr><td class='mycalendar-class-schedule' colspan='2'>";
										   }else if(statusText == 'Completed'){
											   repHtml += "<td class='mycalendar-class-action completed'>"+startDates[i].statustext+"</td></tr><tr><td class='mycalendar-class-schedule' colspan='2'>";
										   }	
										   else if(statusText == "Instructor"){
											   repHtml += "<td class='mycalendar-class-action instructor'>"+statusText+"</td></tr><tr><td class='mycalendar-class-schedule' colspan='2'>";
										   }else{
											   repHtml += "<td class='mycalendar-class-action not-registered'>"+startDates[i].statustext+"</td></tr><tr><td class='mycalendar-class-schedule' colspan='2'>";
										   } */
										   if(startDates[i].statustext == 'Completed' || startDates[i].statustext == 'Not Registered') {
											   repHtml += " ";
										   }
										   else {
											   repHtml += "<table border='0' cellpadding='0' cellspacing='0' class='mycalendar-tooltip'><tr><td class='mycalendar-class-schedule'>";
									   repHtml += Drupal.t("LBL763")+':'+startDates[i].startDateTime.toLowerCase()+"";
											   if(startDates[i].mroflag!=null && startDates[i].mroflag!='' && startDates[i].myclass!='Instructor')
												   repHtml += " | "+startDates[i].mroflag+"";
										   }
									   }
									   repHtml +=  "</td></tr></table>";
									   if(inc < sessLen) {
										   repHtml += "<div class='splitter'>&nbsp;</div>";							
									   } 
								   
							   }
							  
						//}
						   
					 }				
						$(this).qtip({
							 content: '<div id="session_det'+$(this).text()+'" class="mycalqtiptop"></div><div class="mycalqtipmid">'+repHtml+
							 			'</div><div class="mycalqtipbottom"></div>',			
						     show:{
								when:{
									event:'mouseover'
								},
								effect:'slide'
							 },
							 hide: {
								when:{
									event:'mouseout'
								},
								effect:'slide'
							},
							position: { adjust: { x: -250, y: -5 } },
		/*					position: {
							    corner: {
							       target: 'bottomLeft',
							       tooltip: 'topLeft'
							    }
							},
		*/					style: {
								width: 300,
				//				height: 150,
								border: '0px none',
								background: 'none',
								color: '#333333'
							}
						});		
				});
			  }

		   } catch (e) {
		// TODO: handle exception
		   console.log(e, e.stack)
	}
   };
   
  jQuery.fn.in_array=function( arrayElement,arrayName  ){
	   var a=false;
	   for(var i=0;i<arrayName.length;i++){

		   if(arrayElement == arrayName[i]){
			   	a=true;
			   	break;
			   	}
	   }
	return a;
   };
   
jQuery.fn.expertusCalendarBeforeShowDay = function (date,startDates,calType){
	try {
	   	var outtitle="";
		var divId=(calType != undefined && calType == 'isHomePage')?'dv_text_home':'dv_text';
		var courseCount=0;
		var searchdate="";   
		var strLen='60';	
		var text="";
		var title="";
		var dtitle='';
		var crDate=new Date();
		var className='';
		for (var i = 0; i < startDates.length; i++) 
		{	
			var dObj=new Date(startDates[i].eventDate);
				 //title=startDates[i].eventTitle;
			 text = escape(text);
				 dtitle=''; //(title.length>15)?title.substring(0,15)+"...":title;
			 var dId='Act_'+date.getDate()+'_'+date.getMonth();

			 if (date.getMonth() == dObj.getMonth() && date.getDate() == dObj.getDate() && date.getFullYear() == dObj.getFullYear())
			 {
				 if(calType != undefined && calType == 'isHomePage'){
					 outtitle+="<input type='hidden' id=\""+dId+"\" value=\""+text+"\" />";
				 } else {
					 outtitle+="<div><span class=\"textval\" id=\""+dId+"\" style=\"height:auto;text-align:left;width:100%;cursor:pointer\" data=\""+text+"\" onclick=\"javascript:$(this).qtip('hide'); "+startDates[i].eventClick+"\">"+dtitle+"</span></div>";				
				 }
			 }
			  var insClsDetail = startDates[i].insLnrClsDetails; 
		}
		if(outtitle.length>1){	
				var dateVal = (date.getDate() < 10)?("0"+date.getDate().toString()):date.getDate();
				var currDateVal = (crDate.getDate() < 10)?("0"+crDate.getDate().toString()):crDate.getDate();
				var dateFormatted = date.getFullYear()+"/"+("0"+(date.getMonth()+1).toString()).slice(-2) +"/"+dateVal+" "+"00:00:00";
				var currDate = crDate.getFullYear()+"/"+("0"+(crDate.getMonth()+1).toString()).slice(-2)+"/"+currDateVal+" "+"00:00:00";
				var rtn = null;
				var entered = "no";
				
				//The order of color to display on the my calendar. Ticket 0022530.
				if(insClsDetail != null && insClsDetail.length != 0){
					var details= insClsDetail.split('#');
					var clsEndTime = details[0];
					var insClsChk = details[1];
					var isInstructor = details[2];
					
					clsEndTime = clsEndTime.split(',');
					insClsChk = insClsChk.split(',');
					
					var endTotMinsArr = new Array();
					
						 for(var i=0; i<clsEndTime.length;i++){
							 var endHourMins = clsEndTime[i].split(':');
							 endHourMins[0] = (endHourMins[0] == 00) ? 0 : endHourMins[0];
							 endHourMins[1] = (endHourMins[1] == 00) ? 0 : endHourMins[1];
							 var endTotMins = parseInt(endHourMins[0]*60)+parseInt(endHourMins[1]);
							 endTotMinsArr[i] = endTotMins;
						 }
					
					 var todayTime ='';
					 todayTime = (crDate.getHours()*60)+(crDate.getMinutes());
					 
					 var isInsReg ='';
					 for(var i=0;i<endTotMinsArr.length;i++){
						 if(endTotMinsArr[i] >= todayTime){
							 isInsReg = insClsChk[i];
							 break;
						 }
					 }
					 //alert('isInsReg '+isInsReg);
				}

				 for(var i=0;i<startDates.length;i++){
					 if (entered == "yes" && rtn!=null){
						 if(dateFormatted != startDates[i].eventDate)
							 return [false,rtn,'',divId,outtitle];
						 else if(rtn.indexOf("combo-reg")>0 || rtn.indexOf("cell-registered")>0)
							 return [false,rtn,'',divId,outtitle];
						 
						 if(startDates[i].myclass != "Instructor"){
							 continue;
						 }
					 }
					   if(dateFormatted == startDates[i].eventDate){
						   entered = "yes";
						   //if((startDates[i].myclass == "Yes"))
						   if(startDates[i].statustext == "Not Registered"){
							   if((startDates[i].myclass == "Instructor")){
								  // return = [false,'datepicker-withdata mycal cell-instructor textval','',divId,outtitle];
								   if(startDates[i].eventDate == currDate)
									   rtn = 'datepicker-withdata mycal combo-instructor textval';
								   else
									   rtn = 'datepicker-withdata mycal cell-instructor textval';
							   }else{
							   if(startDates[i].eventDate == currDate)							
								  // return = [false,'datepicker-withdata mycal combo-unreg textval','',divId,outtitle];
								   rtn = 'datepicker-withdata mycal combo-unreg textval';
							   else
								  // return = [false,'datepicker-withdata mycal cell-unregistered textval','',divId,outtitle];
								   rtn = 'datepicker-withdata mycal cell-unregistered textval';
							   }
						   }else if(startDates[i].statustext == "Registered"){
							   if(startDates[i].eventDate == currDate){
									   if(isInstructor == true){
										   if(isInsReg == 'INS')
											   rtn = 'datepicker-withdata mycal combo-instructor textval';   
										   else
											   rtn = 'datepicker-withdata mycal combo-reg textval';
									   }else{
											   rtn = 'datepicker-withdata mycal combo-reg textval';
									   }
								}else{
									rtn = 'datepicker-withdata mycal cell-registered textval';
								}
						   }
						   else if(startDates[i].statustext == "Completed"){
							   if((startDates[i].myclass == "Instructor")){
									  // return = [false,'datepicker-withdata mycal cell-instructor textval','',divId,outtitle];
								   if(startDates[i].eventDate == currDate)
									   rtn = 'datepicker-withdata mycal combo-instructor textval';
								   else
									   rtn = 'datepicker-withdata mycal cell-instructor textval';
							   }else{
								   if(startDates[i].eventDate == currDate)							   
									  // return = [false,'datepicker-withdata mycal combo-comp textval','',divId,outtitle];
									   rtn = 'datepicker-withdata mycal combo-comp textval';
								   else
									  // return = [false,'datepicker-withdata mycal cell-completed textval','',divId,outtitle];	
									   rtn = 'datepicker-withdata mycal cell-completed textval';
								}
						   }
					   }
				 }
				 return [false,rtn,'',divId,outtitle];
				//return [false,'datepicker-withdata ui-datepicker-cell-enrolled','', divId, outtitle];
		}else{
			if(calType != undefined && calType == 'isHomePage'){
				return [false,'ui-datepicker-set-date','',divId,''];
			}else {
				return [false,'ui-datepicker-set-date ui-datepicker-cell-enrolled','',divId,''];
			}
		}
		} catch(e) {
			console.log(e);
		}	
   };
  
   
   jQuery.fn.expertusCalendarXmlParser = function (objId, calDefn, year, month, inst){
	   try {
		   loadObj.createLoader('calendar_new');
	   var xstartDates = new Array();
	   var currObj=objId;
	   var t=0;
	   
		   if (year === undefined) {
			   var year =  calDefn.selYear;
		   } 
		
		   if (month === undefined) {
			   var month =  calDefn.selMonth;
		   } else {
			   var month = getFullMonth(month);
		   }
		   
		  var url = '/?q=ajax/calendar-xml-list/' + year +'-' + month + '/month';
		  params = {};
		  $.ajax({
				type: "POST",
				url: url,
				data:  params,
				success: function(result){
					
				
		   
				//$("Items>Item",calDefn.xmlout).each(function(){
				$(result).each(function(index, val){
				
						/*var classId = $("classid",this).text();
				var eventTitle=$("title",this).text();
				var eventCode=$("code",this).text();
				var deliveryType=$("deliverytype",this).text();
				var iconTypeClass=$("classtypeicon",this).text();
				// Modified by Vincent on Oct 30, 2013 for #0028593 (Delivery type check is added)
				var dateWithoutTime = $("datewithouttime",this).text();
				var iltdateWithoutTime = $("iltdatewithouttime",this).text();
				var sessiontimezone = $("session_timezone",this).text();
				var usertimezone =  $("user_timezone",this).text();
				var tz_code =  $("tz_code",this).text();
				var user_tzcode =  $("user_tzcode",this).text();
			  var sessiontimezonecode =  $("session_timezone_code",this).text();
				  var usertimezonecode =  $("usertimezonecode",this).text();
				var startDate=$("eventdate",this).text();
				var iltstartDate = $("ilteventdate",this).text();
				var endDate=$("enddate",this).text();
				var eventDesc=$("cdesc",this).text();
				var eventDate=$("eventdate",this).text();
				var ilteventDate = $("ilteventdate",this).text();
				var mroflag=$("mroflag",this).text();
				var eDate=eventDate.replace(/-/g,'/');
				var x=new Date(eDate);
				var location=$("location",this).text();
				var language=$("language",this).text();
				var startDateTime=$("startdatetime",this).text();
				var iltstartDateTime = $("iltstartdatetime",this).text();
				var endDateTime=$("enddatetime",this).text();
				var iltendDateTime = $("iltenddatetime",this).text();
				var timezone=$("timezone",this).text();
				var status=$("status",this).text();
				var code=$("code",this).text();
				var regstatus = $("regstatus",this).text();
				var statustext = $("statustext",this).text();
				var myclass = $("myclass",this).text();
						var insLnrClsDetails = $("inslnrclsdetails",this).text();*/
						
	//					var classId 				= val.classId;
	//					var eventTitle				= val.eventTitle;
	//					var eventCode				= val.eventCode;
	//					var deliveryType			= val.deliveryType;
	//					var iconTypeClass			= val.iconTypeClass;
	//					// Modified by Vincent on Oct 30, 2013 for #0028593 (Delivery type check is added)
	//					var dateWithoutTime 		= val.dateWithoutTime;
	//					var iltdateWithoutTime 		= val.iltdateWithoutTime;
	//					var sessiontimezone 		= val.sessiontimezone;
	//					var usertimezone 			= val.usertimezone;
	//					var tz_code 				= val.tz_code;
	//					var user_tzcode 			= val.user_tzcode;
	//				  	var sessiontimezonecode 	= val.sessiontimezonecode;
	//					var usertimezonecode 		= val.usertimezonecode;
	//					var startDate				= val.startDate;
	//					var iltstartDate 			= val.iltstartDate;
	//					var eventDesc				= val.eventDesc;
	//					var ilteventDate 			= val.ilteventDate;
	//					var startDateTime			= val.startDateTime;
	//					var iltstartDateTime 		= val.iltstartDateTime;
	//					var endDateTime				= val.endDateTime;
	//					var iltendDateTime 			= val.iltendDateTime;
	//					var code					= val.code;
	//					var regstatus 				= val.regstatus;
	//					var statustext 				= val.statustext;
	//					var myclass 				= val.myclass;
	//					var insLnrClsDetails 		= val.insLnrClsDetails;
	//					var endDate					= val.enddate;  // Not available
	//					var mroflag					= val.mroflag;  // Not available
	//					var location				= val.location; // Not available
	//					var language				= val.language; // Not available
	//					var timezone				= val.timezone; // Not available
	//					var status					= val.status;   // Not available
						
						
						var eventDate				= val.eventDate;
						var eDate					= eventDate.replace(/-/g,'/');
						var x						= new Date(eDate);
						
				var eventClick=calDefn.parent+'.';
				$("Actions>Action",this).each(
					function() {
						var type = $(this).attr('type');
						var actionNm = $(this).attr('name');
						if(actionNm == 'ClickAction'){
							var param = $('Parameters',this).text();
//							param=param.trim();
							eventClick=eventClick+param;
						}
					});
				

						result[index]['eventDate'] = eDate;
						result[index]['eventClick'] = eventClick;
				
	//					var obj={	classId:classId,
	//								eventTitle:eventTitle,
	//								eventCode:eventCode,
	//								iconTypeClass:iconTypeClass,
	//								dateWithoutTime:dateWithoutTime,
	//								iltdateWithoutTime:iltdateWithoutTime,
	//								sessiontimezonecode:sessiontimezonecode,
	//								usertimezonecode:usertimezonecode,
	//								iltstartDateTime:iltstartDateTime,
	//								user_tzcode:user_tzcode,
	//								tz_code:tz_code,
	//								sessiontimezone:sessiontimezone,
	//								usertimezone:usertimezone,
	//								eventDate:eDate,
	//								eventClick:eventClick,
	//								eventDesc:eventDesc,
	//								startDate:startDate,
	//								language:language,
	//								location:location,
	//								startDateTime:startDateTime,
	//								endDateTime:endDateTime,
	//								timezone:timezone,
	//								status:status,
	//								code:code,
	//								deliveryType:deliveryType,
	//								regstatus:regstatus,
	//								statustext:statustext,
	//								mroflag:mroflag,
	//								myclass:myclass,
	//								insLnrClsDetails:insLnrClsDetails
	//					};
	//					
	//					xstartDates[t]=obj;
	//					t++;
	//					
						
		});
		
					startDates = result;
					var dispDay = [SMARTPORTAL.t('Sunday'),SMARTPORTAL.t('Monday'),SMARTPORTAL.t('Tuesday'),SMARTPORTAL.t('Wednesday'),SMARTPORTAL.t('Thursday'),SMARTPORTAL.t('Friday'),SMARTPORTAL.t('Saturday')];
					if(calDefn.calType != undefined && calDefn.calType == 'isHomePage'){
					   dispDay = [SMARTPORTAL.t('Su'),SMARTPORTAL.t('Mo'),SMARTPORTAL.t('Tu'),SMARTPORTAL.t('We'),SMARTPORTAL.t('Th'),SMARTPORTAL.t('Fr'),SMARTPORTAL.t('Sa')];
					}
		
				   this.startDates = startDates;
				  
				   $('#'+objId).datepicker(
					{
						changeMonth: true,
						changeYear : true,
						changeFirstDay:false,
						selectMultiple:true,
						dayNamesMin:dispDay,
						monthNames:[SMARTPORTAL.t('January'), SMARTPORTAL.t('February'), SMARTPORTAL.t('March'), SMARTPORTAL.t('April'), SMARTPORTAL.t('May'), SMARTPORTAL.t('June'), SMARTPORTAL.t('July'), SMARTPORTAL.t('August'), SMARTPORTAL.t('September'), SMARTPORTAL.t('October'), SMARTPORTAL.t('November'), SMARTPORTAL.t('December')],
						showOtherMonths:false,
						dateFormat: 'd/M/Y',
						beforeShowDay:function(date){
							return $.fn.expertusCalendarBeforeShowDay(date,startDates,calDefn.calType);
						},
						onChangeMonthYear:function(year, month, inst) {		
							$.fn.expertusCheckDatePickerLoaded(objId, calDefn, year, month, inst);
						}
		
					});
				   $("#catalog-admin-cal-session-details").html("");
				   $("#catalog-admin-cal-legend").show();
				   $('#'+objId).datepicker("refresh");
				   $('#'+objId+' .datepicker-withdata')
				   .die('click')
				   .live('click', function(e){
					   var dateVal = (parseInt($(this).text()) < 10) ? ("0"+$(this).text()) : $(this).text();
					   $.fn.expertusCalendarShowDayEvents(dateVal, calDefn);
				   })
				   if ($(".ui-datepicker-today").length) {
					   $(".ui-datepicker-today").click();
				   }
				   vtip();
				   loadObj.destroyLoader('calendar_new');
				
			}
			
		  });
		//return xstartDates;
	   } catch(e) {
		   console.log(e);
	   }  
		
	};
	
	
	jQuery.fn.expertusCalendarShowDayEvents = function (dateVal, calDefn){
		try {
			  var scrollCnt = 0;
			  var scrollCnt1 = 0;
			  loadObj.createLoader('catalog-admin-cal-session-details');	
			  $('.ui-datepicker-calendar tbody td').click(function(){
				  	if($(this).hasClass("ui-datepicker-set-date")){ 
						$('#catalog-admin-cal-session-details').html("");
						$(this).css("background","#ffffff");
						$('.ui-datepicker-calendar tbody td').removeClass("selectDateTD");
			  		} else {
			  			$('.ui-datepicker-calendar tbody td').removeClass("selectDateTD");
			  			$(this).addClass("selectDateTD");
			  		}
			  });
			  
				
			  	var obj = this;
				var sessionDet = '';
				var sessionDet1 = '';
				//var classIds = new Array();
				var eventsCode = new Array();
				var Month = $('#catalog-admin-cal .ui-datepicker-month').val();
				month = parseInt(Month)+1;
				month = ("0"+ month).slice(-2);
				var year = $('#catalog-admin-cal .ui-datepicker-year').val();
				
				var dateFormatted = year+"/"+month+"/"+dateVal+" "+"00:00:00";
				var dateFormat = year+"-"+month+"-"+dateVal;
				var url = '/?q=ajax/calendar-xml-list/' + dateFormat + '/day';
				params = {};
				$.ajax({
						type: "POST",
						url: url,
						data:  params,
						success: function(result){
							$(result).each(function(index, val){
									var eventDate				= val.eventDate;
									var eDate					= eventDate.replace(/-/g,'/');
									result[index]['eventDate'] = eDate;
							});
							startDates = result;
							 var inc = 0;
							 var repHtml = "";
							 var statusHtml ="";
							 var sessLen = startDates.length;
							 for(var i=0;i<startDates.length;i++){
								 	   inc=inc+1;		
								 	   if(dateFormatted == startDates[i].eventDate){
										   
										   //classIds.push(startDates[i].classId);
										   	   if(startDates[i].myclass=='Instructor')
										   		   statusText="Instructor";
										   	   else
										   		   statusText=startDates[i].statustext;
										   	   
											   if($.fn.in_array(startDates[i].eventCode,eventsCode) == false) {
												   if(repHtml!=""){
													   repHtml +=  "</tbody></table></td></tr></tbody></table>";
												   }
												   scrollCnt = scrollCnt + 1;
												   eventsCode.push(startDates[i].eventCode);
												   repHtml += "<table border='0' cellpadding='0' cellspacing='0' id='my-calendar-details-tab-id' class='calendar-session-details'><tbody><tr><td><div class='"+startDates[i].iconTypeClass+"'> </div></td><td><table cellpadding='0' cellspacing='0' class='calendar-session-details-list'><tbody><tr><td colspan='3' class='calendar-session-head' >";
												   repHtml += "<div class='limit-title-row'><span class='limit-title vtip' title='"+htmlEntities(startDates[i].eventTitle)+"'>"+startDates[i].eventTitle+"</span></div>";
												   if(startDates[i].mroflag!=null && startDates[i].mroflag!='' && startDates[i].myclass!='Instructor'){
													   if(startDates[i].mroflag == 'Mandatory'){
													     repHtml += "<span title="+Drupal.t('Mandatory')+" class='calendar-mandatory-bg vtip'>M</span>";
													   }else if(startDates[i].mroflag == 'Recommended'){
														 repHtml += "<span title="+Drupal.t('Recommended')+" class='calendar-recommended-bg vtip'>R</span>";   
													   }
												   }
												   repHtml += "</td></tr>";
												   var statusHtml = '';
												   if(statusText == 'Registered'){
													   statusHtml += "<td class='mycalendar-class-action registered'>"+Drupal.t("Registered")+"</td>";
												   }else if(statusText == 'Completed'){
													   statusHtml += "<td class='mycalendar-class-action completed'>"+Drupal.t("Completed")+"</td>";
												   }	
												   else if(statusText == "Instructor"){
													   statusHtml += "<td class='mycalendar-class-action instructor'>"+Drupal.t("Instructor")+"</td>";
												   }else{
													   statusHtml += "<td class='mycalendar-class-action not-registered'>"+Drupal.t("Not Registered")+"</td>";
												   } var qtipop = 'calendar-'+startDates[i].classId;
												   var disp = startDates[i].iltdateWithoutTime+" "+startDates[i].iltstartDateTime+" "+startDates[i].sessiontimezonecode+" "+startDates[i].tz_code;
												   if (statusText == 'Not Registered' && startDates[i].sessionCount > 1) {
														   disp += '&nbsp;' + Drupal.t('LBL621') + '&nbsp;<br />' + startDates[i].eventEnd.iltdateWithoutTime +" "+ startDates[i].eventEnd.iltEndDateTime.toLowerCase()+" "+startDates[i].sessiontimezonecode+" "+startDates[i].tz_code;
												   }
												   var status = qtip_popup_paint(qtipop, disp, '', 1, 'mylearning-calendar'); 
												   repHtml += "<tr><td class='eventCodetitle'><span class='vtip' title='"+htmlEntities(startDates[i].eventCode)+"'>"+titleRestrictionFadeoutImage(startDates[i].eventCode,'calendar-js-title-restriction')+"</span></td><td class='eventCodeseperator'><span> | </span></td>"+statusHtml+"</tr>";
												   
												   // if(statusText != 'Completed') {
													   repHtml += "<tr><td class='tz sample' colspan='3'>"+startDates[i].dateWithoutTime+" "+startDates[i].startDateTime.toLowerCase()+" "+startDates[i].usertimezonecode+" "+startDates[i].user_tzcode+"</td></tr>"; //71627: My Calendar - Make the following modifications in the My calendar popup
													   if (statusText == 'Not Registered' && startDates[i].sessionCount > 1) {
														   repHtml += "<tr><td class='tz sample' colspan='3'>"+startDates[i].eventEnd.dateWithoutTime +" "+ startDates[i].eventEnd.endDateTime.toLowerCase()+" "+startDates[i].usertimezonecode+" "+startDates[i].user_tzcode+ "</td></tr>";
													   }
												   // }
												   if(startDates[i].deliveryType == 'lrn_cls_dty_ilt' && startDates[i].sessiontimezone!=startDates[i].usertimezone){
													   repHtml += "<tr><td colspan='4'>"+status+"</td></tr>";
												   }
												   
											  
											   } else {
													   if(startDates[i].statustext == 'Completed' || startDates[i].statustext == 'Not Registered') {
													   repHtml += " ";
												   }
												   else {
													  // repHtml += "<table border='0' cellpadding='0' cellspacing='0' class='mycalendar-tooltip'><tbody><tr><td class='mycalendar-class-schedule'>";
														//repHtml += "<tr><td colspan='3'>"+startDates[i].dateWithoutTime+" "+startDates[i].startDateTime.toLowerCase();
													  // repHtml += "<tr><td class='tz' colspan='3'>"+startDates[i].dateWithoutTime+" "+startDates[i].startDateTime+" "+startDates[i].usertimezonecode+" "+startDates[i].user_tzcode+"</td></tr>";
													repHtml += "<tr><td class='tz' colspan='3'>"+startDates[i].dateWithoutTime+" "+startDates[i].startDateTime.toLowerCase()+" "+startDates[i].usertimezonecode+" "+startDates[i].user_tzcode+"</td></tr>";
													   
													   if(startDates[i].mroflag!=null && startDates[i].mroflag!='' && startDates[i].myclass!='Instructor'){
														    if(startDates[i].mroflag == 'Mandatory'){
															     repHtml += "<span title="+Drupal.t('Mandatory')+" class='calendar-mandatory-bg vtip'>M</span>";
															  } else if(startDates[i].mroflag == 'Recommended'){
																 repHtml += "<span title="+Drupal.t('Recommended')+" class='calendar-recommended-bg vtip'>R</span>";   
															  }
													   }
												       repHtml += "</td></tr>";
												   }
											   }
											   

									   }
									  
								//}
								   
							 }
							
						
							 $('#catalog-admin-cal-session-details').html("");
							 var calDestroy = $('.session-details-list').data('JSP');
							 if(calDestroy){
								calDestroy.destroy();
							 }
							 if(repHtml!=""){
								   repHtml +=  "</tbody></table></td></tr></tbody></table>";
							 }
							 $('#catalog-admin-cal-session-details').html("<div class='session-details-list'>"+repHtml+"</div>");						 						 
							 $('#my-calendar-details-tab-id .limit-title').trunk8(trunk8.cal_title);
							 $('#catalog-admin-cal-session-details .session-details-list').jScrollPane();
							 //	 $('.jspDrag').css("height","115");
							 vtip();
							
							 //$(".ui-datepicker-today").click();
							 loadObj.destroyLoader('catalog-admin-cal-session-details');	 
						}
				});		
				
					
					
			
		} catch(e){
			console.log(e);
		}
	}
	   	
})(jQuery);