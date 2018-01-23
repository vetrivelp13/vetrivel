(function($) {
	var dWidth='';
	$.widget("ui.admincatalogroster", {
		_init: function() {
			try{
			var self = this;
			this.rosterObj = '$("body").data("admincatalogroster")';
			}catch(e){
				// to do
			}
		},

		launchRoster : function (courseId, classId,rosterTitle){
			try{
			this.uniqueId = courseId+'_'+ classId;
			this.courseId = courseId;
			this.classId  = classId;
			this.rosterTitle = rosterTitle;
			this.renderRosterPopup();
			}catch(e){
				// to do
			}
		},

		renderRosterPopup : function(){
			try{
		    $('#catalog_roster_container').remove();
		    var html = '';
		    var dlgTitle = 'Roster - '+this.rosterTitle;
		    html+='<div id="catalog_roster_container" class="admin-roster-wrapper"></div>';
		    $("body").append(html);
		    $("#catalog_roster_container").dialog({
		        position:[(getWindowWidth()-900)/2,100],
		        bgiframe: true,
		        width:790,
		        //height: 650,
		        resizable:true,
		        modal: true,
		        title: SMARTPORTAL.t(dlgTitle),
		        closeOnEscape: false,
		        draggable:true,
		        overlay:
		         {
		           opacity: 0.5,
		           background: "black"
		         }
		    });	
		    
		    this.createLoader('catalog_roster_container');
		    
			this.regStatusList = this.buildDropDownList('lrn_crs_reg_');
			this.compStatusList = this.buildDropDownList('lrn_crs_cmp_');

			var formContent = this.buildFormContent();
		    document.getElementById('catalog_roster_container').innerHTML = formContent; 
		    //$('#catalog_roster_container').html(formContent);		    

			$( "#registration_status_date" ).datepicker({showOn: "button", buttonImage: "sites/all/themes/core/AdministrationTheme/images/calendar_icon.JPG", buttonImageOnly: true});
			$( "#completion_date" ).datepicker({showOn: "button", buttonImage: "sites/all/themes/core/AdministrationTheme/images/calendar_icon.JPG", buttonImageOnly: true});
			$( "#content_valid_from" ).datepicker({showOn: "button", buttonImage: "sites/all/themes/core/AdministrationTheme/images/calendar_icon.JPG", buttonImageOnly: true});
			$( "#content_valid_to" ).datepicker({showOn: "button", buttonImage: "sites/all/themes/core/AdministrationTheme/images/calendar_icon.JPG", buttonImageOnly: true});

		    this.destroyLoader('catalog_roster_container');
		    
		    $("#catalog_roster_container").show();	
			$('.ui-dialog-titlebar-close').click(function(){
		        $("#catalog_roster_container").remove();
		    });
			
			var userName = null;
			var fullName = null;
			var regStatus = null; 
			var compStatus = null; 
			var score = null;
			var grade = null;
			
			this.rosterSearchResult(userName, fullName, regStatus, compStatus, score, grade);
			}catch(e){
				// to do
			}
		},		
		
		rosterSearchResult : function(userName, fullName, regStatus, compStatus, score, grade){
			//$("#roster-result-container").css('height', '50px');
			try{
			this.createLoader('roster-container-div');
			var obj = this;
			var objStr = '$("#roster-result-container").data("admincatalogroster")';
			var param = userName+"$$$"+fullName+"$$$"+regStatus+"$$$"+compStatus+"$$$"+score+"$$$"+grade;
			$("#roster-result-container").jqGrid({
				url:this.constructUrl("administration/learning/course-class/roster/"+this.courseId+"/"+this.classId+"/"+param),
				datatype: "json",
				mtype: 'GET',
				colNames:[ 'Full Name','Username','Registration Status','Completion Status','Score','Grade'],
				colModel:[ {name:'FullName',index:'FullName', title: true, width:250, 'widgetObj':objStr,formatter:obj.paintRosterResults},
				           {name:'Username',index:'Username', title: true, width:0, 'widgetObj':objStr,formatter:obj.paintRosterResults},
				           {name:'RegistrationStatus',index:'RegistrationStatus', title: true, width:0,'widgetObj':objStr,formatter:obj.paintRosterResults},
				           {name:'CompletionStatus',index:'CompletionStatus', title: true, width:0, 'widgetObj':objStr,formatter:obj.paintRosterResults},
				           {name:'Score',index:'Score', title: true, width:50, 'widgetObj':objStr,formatter:obj.paintRosterResults},
				           {name:'Grade',index:'Grade', title: true, width:50, 'widgetObj':objStr,formatter:obj.paintRosterResults}],
				rowNum:10,
				rowList:[10,25,50],
				pager: '#pager-roster',
				viewrecords:  true,
				multiselect: true,
				emptyrecords: "",
				sortorder: "desc",
				toppager: false,
				height: 'auto',
				width:760,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadui:false,
				gridview : true,
				loadComplete:obj.callbackCatalogRosterLoader
			}).navGrid('#pager-roster',{add:false,edit:false,del:false,search:false,refreshtitle:true});
			
			//$("#roster-result-container").jqGrid('filterToolbar');
			$('#roster-result-container tr:last').css('border-bottom','0px');
			$("#roster-result-container").jqGrid('filterToolbar', {searchOnEnter: false });
			$("#roster-result-container").jqGrid('filterToolbar', {searchOnEnter: false });
			//$("#roster-result-container").setGridParam({rowNum:10}).trigger("reloadGrid");
          //  $("#roster-result-container").jqGrid('filterToolbar',{stringResult: true, searchOnEnter: true, defaultSearch: 'on'});
			}catch(e){
				// to do
			}

		},

		paintRosterResults : function(cellvalue, options, rowObject){
			try{
			var index  = options.colModel.index;
			if(index  == 'Username') {
				return rowObject['Username'] == null ? '' : rowObject['Username'];
			} else if(index  == 'FullName') {
				return rowObject['FullName'] == null ? '' : rowObject['FullName'];
			} else if(index  == 'RegistrationStatus') {
				return rowObject['RegistrationStatus'] == null ? '' : rowObject['RegistrationStatus'];
			} else if(index  == 'CompletionStatus') {
				return rowObject['CompletionStatus'] == null ? '' : rowObject['CompletionStatus'];
			} else if(index  == 'Score') {
				return rowObject['Score'] == null ? '' : rowObject['Score'];
			} else if(index  == 'Grade') {
				return rowObject['Grade'] == null ? '' : rowObject['Grade'];
			}
			}catch(e){
				// to do
			}
		},

		callbackCatalogRosterLoader : function(response){
			//alert(response.events_filter);
			try{
			var filterevents = response.events_filter;
			if($('#catalog_roster_loaded').val() == 0){
				$('#count-statistics').html(response.count_result);
				$('#catalog_roster_loaded').val(1);
			}
			$("body").data("admincatalogroster").destroyLoader('roster-container-div');
			if ($("#roster-result-container").getGridParam("records") <= 0 && filterevents==0) {
				$('#roster-noresult-msg').show();
				$("#catalog_roster_container").dialog({height: 300});
				$('#roster-container').hide();
			} else {
				$('#roster-noresult-msg').hide();
			}
			
			$('#gbox_roster-result-container #prev_pager-roster').click(function(){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('admincatalogroster').createLoader('roster-container-div');
				}
			});

			$('#gbox_roster-result-container #next_pager-roster').click(function(){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('admincatalogroster').createLoader('roster-container-div');
				}
			});

			$('#gbox_roster-result-container .ui-jqgrid-sortable').click(function() {
				$('body').data('admincatalogroster').createLoader('roster-container-div');
			});
			
			$('#gbox_roster-result-container .ui-pg-selbox').bind('change',function() {
				$('body').data('admincatalogroster').createLoader('roster-container-div');
			});			
			$('#roster-result-container tr:last').css('border-bottom','0px');
			}catch(e){
				// to do
			}
		},

		searchRoster : function(){
			try{
			var username_search 			= $('#username_search').val() == '' ? null : $('#username_search').val();
			var fullname_search 			= $('#fullname_search').val() == '' ? null : $('#fullname_search').val();
			var registration_status_search 	= $('#registration_status_search').val() == '' ? null : $('#registration_status_search').val();
			var completion_status_search 	= $('#completion_status_search').val() == '' ? null : $('#completion_status_search').val();
			var score_search 				= $('#score_search').val() == '' ? null : $('#score_search').val();
			var grade_search 				= $('#grade_search').val() == '' ? null : $('#grade_search').val();
			
			this.rosterSearchResult(username_search, fullname_search, registration_status_search, completion_status_search, score_search, grade_search);
			}catch(e){
				// to do
			}
		},
		
		formReset : function(){
			try{
			$(':input','#catalog-roster-form-id')
			 .not(':button, :submit, :reset, :hidden')
			 .val('')
			 .removeAttr('checked')
			 .removeAttr('selected');
			}catch(e){
				// to do
			}
		},
		
		buildFormContent : function(){
			try{
			var content = '';
			//content += '<div class="catalog-label-div">Roster form:</div>';
			content += '<div id="catalog-roster-form">';
			content += '<input type="hidden" id="catalog_roster_loaded" value="0">';
			content += '<form id="catalog-roster-form-id" name="roster-form">';
			content += '<span class="admin-roster-col1"><label>Registration Status:</label><select name="registration_status" class="admin-roster-col1-field" id="registration_status" onchange="$(\'body\').data(\'admincatalogroster\').fillDate(\'lrn_crs_reg_\', this.value)"><option value="">Select</option>'+this.regStatusList+'</select></span>';
			content += '<span class="admin-roster-col2"><label>Registration Date:</label><input readonly="readonly" type="text" name="registration_status_date" id="registration_status_date"/></span><br/>';
			content += '<span class="admin-roster-col1"><label>Completion Status:</label><select name="completion_status" class="admin-roster-col1-field" id="completion_status" onchange="$(\'body\').data(\'admincatalogroster\').fillDate(\'lrn_crs_cmp_\', this.value)"><option value="">Select</option>'+this.compStatusList+'</select></span>';
			content += '<span class="admin-roster-col2"><label>Completion Date:</label><input readonly="readonly" type="text" name="completion_date" id="completion_date"/></span><br/>';
			content += '<span class="admin-roster-col1"><label>Score:</label><input size="20" class="admin-roster-col1-field" type="text" name="score" id="score"/></span>';
			content += '<span class="admin-roster-col2"><label>Content Valid From:</label><input readonly="readonly" type="text" name="content_valid_from" id="content_valid_from"/></span><br/>';
			//content += '<span class="admin-roster-col1"><label>Grade:</label><select name="grade" class="admin-roster-col1-field" id="grade"><option value="">Select</option>'+this.gradeList+'</select></span>';
			content += '<span class="admin-roster-col2"><label>Content Valid To:</label><input readonly="readonly" type="text" name="content_valid_to" id="content_valid_to"/></span><br/>';
			
			// Cancel and Save button
			content += '<div id="addedit-form-cancel-and-save-actions-row">';
			
				// Cancel button
				content += '<div class="addedit-form-cancel-container-actions">';
					//content += '<div class="admin-save-button-left-bg"></div>';
					content += '<input type="button" class="admin-action-button-middle-bg" value="Cancel" onclick="$(\'#catalog_roster_container\').remove();" />';
					//content += '<div class="admin-save-button-right-bg"></div>';

					// Submit button
					content += '<div class="admin-save-button-container">';
					content += '<div class="admin-save-button-left-bg"></div>';
					content += '<input type="button" class="admin-save-button-middle-bg" value="Apply" onclick="$(\'body\').data(\'admincatalogroster\').catalogRosterSubmit();" />';
					content += '<div class="admin-save-button-right-bg"></div>';
					content += '</div>';

				content += '</div>';
				
			content += '</div>';
			
			content += '</form></div><br/>';
			
			// Search content
			/*
			content += '<fieldset><legend>Search</legend>';
			content += '<table>';
			content += '<tr><td>Username</td><td>Full Name</td><td>Registration Status</td><td>Completion Status</td><td>Score</td><td>Grade</td></tr>';
			content += '<tr>';
				content += '<td><input type="text" name="username_search" id="username_search"></td>';
				content += '<td><input type="text" name="fullname_search" id="fullname_search"></td>';
				content += '<td><input type="text" name="registration_status_search" id="registration_status_search" size="20"></td>';
				content += '<td><input type="text" name="completion_status_search" id="completion_status_search" size="20"></td>';
				content += '<td><input type="text" name="score_search" id="score_search" size="20"></td>';
				content += '<td><input type="text" name="grade_search" id="grade_search" size="10"></td>';
			content += '</tr>';
			content += '<tr><td colspan="6" align="center"><input type="button" value="Search" onClick="$(\'body\').data(\'admincatalogroster\').searchRoster();"></td></tr>';
			content += '</table>';
			content += '</fieldset>';
			*/
			
			// Status update content
			content += '<div id="roster-status-update" class="roster-status"></div>';
			
			// No result container
			content += '<div id="roster-noresult-msg">No user(s) has registered for this class.</div>';
			
			// Enrolled User list 
			content += '<div id="roster-container"><div class="catalog-label-div">User(s) list:</div><div id="count-statistics" class="count-statistics"></div><div id="roster-container-div"><table id="roster-result-container"></table><div id="pager-roster"></div></div>';
			content += '</div></div>';
			return content;
			}catch(e){
				// to do
			}
		},
		
		catalogRosterSubmit : function(){
			try{
			this.createLoader('catalog_roster_container');
			var selectedEnrolledIds = $("#roster-result-container").jqGrid('getGridParam','selarrrow');
			if(selectedEnrolledIds == ''){
				this.destroyLoader('catalog_roster_container');
				return false;
			}
			var registrationStatus = $('#registration_status').val(); 
			var registrationStatusDate = $('#registration_status_date').val();
			var completionStatus = $('#completion_status').val();
			var completionDate = $('#completion_date').val();
			var grade = $('#grade').val();
			var score = $('#score').val();
			var contentValidFrom = $('#content_valid_from').val();
			var contentValidTo = $('#content_valid_to').val();
			$('#catalog_roster_loaded').val(0);
			registrationStatusDate = registrationStatusDate.replace('/', '-');
			registrationStatusDate = registrationStatusDate.replace('/', '-');
			
			completionDate = completionDate.replace('/', '-');
			completionDate = completionDate.replace('/', '-');
			
			contentValidFrom = contentValidFrom.replace('/', '-');
			contentValidFrom = contentValidFrom.replace('/', '-');
			
			contentValidTo = contentValidTo.replace('/', '-');
			contentValidTo = contentValidTo.replace('/', '-');
			
			var params = this.courseId+"$$$"+this.classId+'$$$'+selectedEnrolledIds+'$$$'+registrationStatus+'$$$'+registrationStatusDate+'$$$'+completionStatus+'$$$'+completionDate+'$$$'+grade+'$$$'+score+'$$$'+contentValidFrom+'$$$'+contentValidTo;
			
			var contentStart = '<div class="catalog-label-div">Status:</div><table><tr class="header"><td>Username</td><td>Registration Status</td><td>Completion Status</td><td>Score Status</td><td>Grade Status</td></tr>';
			var contentEnd = '</table>';
			var htmlContent = '';
			var rosterURL = decodeURI('/?q=administration/learning/course-class/roster/update/'+params);
			$.ajax({
				url : rosterURL,
				async: false,
				success: function(data) {
					htmlContent = contentStart + $.trim(data) + contentEnd;
					$('#roster-status-update').html('');
					$('#roster-status-update').html(htmlContent);
					$('#roster-status-update').show();
				}
			});

			var userName = null;
			var fullName = null;
			var regStatus = null; 
			var compStatus = null; 
			var score = null;
			var grade = null;
			var rosterGridParam = userName+"$$$"+fullName+"$$$"+regStatus+"$$$"+compStatus+"$$$"+score+"$$$"+grade;
			var rosterGridURL = decodeURI("/?q=administration/learning/course-class/roster/"+this.courseId+"/"+this.classId+"/"+rosterGridParam);
			this.destroyLoader('catalog_roster_container');
			this.createLoader('roster-container-div');
			$('#roster-result-container').setGridParam({url: rosterGridURL});
		    $("#roster-result-container").trigger("reloadGrid",[{page:1}]);

		    this.formReset();
			}catch(e){
				// to do
			}
		},
		
		buildDropDownList : function(code){
			try{
			var dropDownList = '';
			$.ajax({
				url : decodeURI('/?q=administration/learning/course-class/roster/load-drop-down/'+code),
				async: false,
				success: function(data) {
					data = $.trim(data);
					dataWithIndexArray = data.split('###');
					
					if(code == 'lrn_crs_reg_'){
						for(var i=0; i< dataWithIndexArray.length; i++){
							dataArray = dataWithIndexArray[i].split('$$$');
							if(dataArray[0] != 'lrn_crs_reg_ppm' && dataArray[0] != 'lrn_crs_reg_rsv' && dataArray[0] != 'lrn_crs_reg_rsc'){
								dropDownList += "<option value='"+dataArray[0]+"'>"+dataArray[1]+"</option>";
							}
						}
					} else if(code == 'lrn_crs_cmp_'){
						for(var i=0; i< dataWithIndexArray.length; i++){
							dataArray = dataWithIndexArray[i].split('$$$');
							dropDownList += "<option value='"+dataArray[0]+"'>"+dataArray[1]+"</option>";
						}
					} else {
						for(var i=0; i< dataWithIndexArray.length; i++){
							dataArray = dataWithIndexArray[i].split('$$$');
							dropDownList += "<option value='"+dataArray[0]+"'>"+dataArray[1]+"</option>";
						}
					}
				}

			});
			return dropDownList; 
			}catch(e){
				// to do
			}
		},
		
		fillDate : function(code, selectedValue){
			try{
			var dateObj = new Date();
			var date  = (dateObj.getMonth()+1)+'/'+dateObj.getDate()+'/'+dateObj.getFullYear();
			//alert(code + ' ' + date + ' ' + selectedValue);
			if(code == 'lrn_crs_reg_'){
				if(selectedValue == ''){
					$('#registration_status_date').val('');
				} else {
					$('#registration_status_date').val(date);
				}
			} else {
				if(selectedValue == 'lrn_crs_cmp_cmp'){
					$('#completion_date').val(date);
				} else {
					$('#completion_date').val('');
				}
			}
			}catch(e){
				// to do
			}
		}
		
	});
	
	$.extend($.ui.admincatalogroster.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
	
})(jQuery);

$(function(){ $("body").admincatalogroster(); });