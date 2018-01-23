(function($) {
	var dWidth='';
	var crs_cls_listcount = 14;
	$.widget("ui.mulitselectdatagrid", {
		_init: function() {
			try{
			var self = this;
			this.gridObj = '$("body").data("mulitselectdatagrid")';
			this.type = '';
			this.entityId = '';
			this.entityType = '';
			this.gridUrl = '';
			this.sesStartEndDate='';
			this.uniqueId='';
			}catch(e){
				// to do
			}
		},
		/*
		 * This will be called when enrollment auto populate search click,
		 * on user user search result page
		 */
		searchEnrollUserDataGrid : function(userId){
			try{
			var searchKeyword	= $('#user-enrollment-autocomplete-'+userId).val();
			this.entityId 	= userId;
			this.entityType = 'enrollments';
			this.uniqueId 	= this.entityId +'-' + this.entityType;
			$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+ this.uniqueId);
			if(searchKeyword!='' && searchKeyword != Drupal.t('LBL545')){
				searchKeyword = searchKeyword.replace(/\//g, "|");
				var dataGridURL = this.constructUrl("administration/people/user/get-user-enrollments/"+userId+"/"+searchKeyword);
			}else{
				var dataGridURL = this.constructUrl("administration/people/user/get-user-enrollments/"+userId);
			}
		    $('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL});
		    $('#datagrid-container-'+this.uniqueId).trigger("reloadGrid",[{page:1}]);
			}catch(e){
				// to do
			}
		},

		loadUserEnrollmentGrid: function(qtipObj){
			$('#group-control').remove();
			$('.qtip-popup-visible').hide();
			$('#' + qtipObj.popupDispId).qtipPopup({
				wid : qtipObj.wid,
				heg : qtipObj.heg,
				entityId : qtipObj.entityId,
				popupDispId : qtipObj.popupDispId,
				postype : qtipObj.postype,
				poslwid : (qtipObj.poslwid == '' || qtipObj.poslwid == undefined || qtipObj.poslwid == null) ? '' : qtipObj.poslwid,
				posrwid : (qtipObj.posrwid == '' || qtipObj.posrwid == undefined || qtipObj.posrwid == null) ? '' : qtipObj.posrwid,
				disp	: (qtipObj.qdis == '' || qtipObj.qdis == undefined || qtipObj.qdis == null) ? ''	: qtipObj.qdis,
				linkid	: qtipObj.linkid,
			});
			var o =this;
			setTimeout(function(){
				$('#qtip_enrollment_disp_'+qtipObj.entityId+' #paintContentVisiblePopup').wrap('<div id="admin-data-grid">');
				$('#qtip_enrollment_disp_'+qtipObj.entityId+' #paintContentVisiblePopup #paintContentqtip_enrollment_disp_'+qtipObj.entityId).addClass('user-enrollment-div-cls');
				o.loadUserEnrollmentGridData(qtipObj);
			},10);

		},

		/*
		 * This function will be called when admin clicks on 'Enrollments'
		 * at user search result page.
		 * Grid content, which contains class and tp enrollment list for
		 * particular user
		 */
		loadUserEnrollmentGridData : function(qtipObj){
			try{
				var userId =qtipObj.entityId;
				popupId = qtipObj.popupDispId;
				entId = qtipObj.entityId;
				/*$(".user-enrollment-div-cls").hide();*/
				$('#user-enrollment-main-div-'+userId).css('display','block');
				$('#user-enrollment-main-div-'+userId).css("visibility","hidden");
				var objStr = '';
				var obj    = this;
				this.entityId 	= userId;
				this.entityType = 'enrollments';
				this.uniqueId 	= this.entityId +'-' + this.entityType;
				var url = "administration/people/user/get-user-enrollments/"+userId;

			if($("#datagrid-container-"+this.uniqueId).html()==''){
				$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+this.uniqueId);
				$('#loaderdivdatagrid-div-'+this.uniqueId).css('width','100%');
				$('#loaderdivdatagrid-div-'+this.uniqueId+' .loaderimg').css('margin','0 auto');
				$('#user-enrollment-main-div-'+userId).css("visibility","hidden");
			}else{
				$("#user-enrollment-autocomplete-"+userId).val(Drupal.t('LBL545'));
				$("#user-enrollment-autocomplete-"+userId).addClass('input-field-grey');
			}

			//This ajax call will get separate enrollment count list for each status
			countUrl = this.constructUrl("ajax/administration/people/user/get-enrollment-count/" + userId );
			$.ajax({
				type: "POST",
				url: countUrl,
				datatype: "json",
				//async : false,
				data:  '',
				success: function(data){
					var result = jQuery.parseJSON(data);
					$('#user-enrollments-status-count-div-'+userId).html(result.resultCount);
					$('#user-enrollment-main-div-'+userId).css("visibility","visible");
				}
			});
			var htmldata='';
			htmldata +='<div class= "filter-search-start-date-left-bg enroll-search-wrapper-div"></div>';
			htmldata +='<span style="display: block;"  class="narrow-text-container-cls user-enroll-input-auto-cls" id="user-'+qtipObj.entityId+'-orgusers-search-box">';

			htmldata +='<input onblur="textfieldTitleBlur(this,'+"'"+Drupal.t('LBL545')+"'"+');" onfocus="textfieldTitleClick(this, '+"'"+Drupal.t('LBL545')+"'"+');" type="text" autocomplete="off" alt="'+Drupal.t('LBL275')+" "+Drupal.t('LBL304')+'" maxlength="75" size="59" value="'+Drupal.t('LBL545')+'"';
			htmldata +='class="ac_input narrow-search-ac-text-overlap header-search-text-filter input-field-grey user-enroll-input-auto-cls" id="user-enrollment-autocomplete-'+qtipObj.entityId+'">';
			//0037049: Code Re-Factoring 5. People > User > Enrollments popup > While there are no records, the left and right curves of the search tool are missing.
			htmldata +='<a onClick="$(\'body\').data(\'mulitselectdatagrid\').searchEnrollUserDataGrid(\''+qtipObj.entityId+'\');" id="search-user-enrollments-'+qtipObj.entityId+'"  title="'+Drupal.t('LBL275')+" "+Drupal.t('LBL304')+'" class="narrow-text-search" >&nbsp;</a></span>';
			htmldata +='<div class= "filter-search-start-date-right-bg enroll-search-wrapper-div"></div>';

			$("#datagrid-div-inside-grid-"+this.uniqueId).html('');
			htmldata +='<div id="datagrid-div-inside-grid-'+this.uniqueId+'">';
			htmldata += '<table id="datagrid-container-'+this.uniqueId+'" class="datagrid-container-common"></table>';
			htmldata += '<div id="pager-datagrid-'+this.uniqueId+'" class="pager-datagrid-common"></div>';
			htmldata += '<div id="datagrid-div-'+this.uniqueId+'">';
			htmldata +=  '<div class="user-enrollment-bottom-row-cls">';
			htmldata += '<span class="user-enrollment-count-pipe-cls" id="user-enrollments-status-count-div-'+userId+'"></span>';
		  	htmldata +=  '<div id="user-enrollment-container"><div class="white-btn-bg-left"></div><span> <a class="user-enroll-close-link white-btn-bg-middle" onclick="closeQtip('+'\'\' '+',\''+qtipObj.entityId+'\');"    >'+Drupal.t('LBL123')+'</a></span><div class="white-btn-bg-right"></div></div>';
			htmldata +='</div></div></div>';
			$('#'+popupId+' #bubble-face-table .bubble-c #paintContent'+popupId).html(htmldata);
			$('body').data('mulitselectdatagrid').createLoader('datagrid-div-inside-grid-'+this.uniqueId);
		 //data grid content call for user enrollments
			var dateTitle	= Drupal.t('LBL042')+'<p class="usr-enroll-grid-header-grey-cls">(' + Drupal.t('LBL699') + ')</p>';
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			 if(this.currTheme == "expertusoneV2")
			  {
			   if (navigator.userAgent.indexOf("Chrome")>0 || $.browser.msie && $.browser.version >= 8) {
				   var enrollGridWidth=548;
			    }
			   else
			   {
				   var enrollGridWidth=550;
			   }
			  }
			$("#datagrid-container-"+this.uniqueId).jqGrid({
				url: this.constructUrl(url),
				datatype: "json",
				mtype: 'GET',
				colNames:[ Drupal.t('LBL083'), Drupal.t('LBL096'), Drupal.t('LBL036'),dateTitle , Drupal.t('LBL102')],
				colModel:[ {name:'Title',index:'title', title: false, width:140, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false  },
				           {name:'Code',index:'code', title: false, width:100, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false},
				           {name:'Type',index:'delivery_type', title: false, width:105, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false },
				           {name:'Date',index:'Date', title: false, width:115, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false },
				           {name:'Status',index:'Status', title: false, width:85, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false}],
				rowNum:5,
				rowList:[5,10,25,50],
				pager: '#pager-datagrid-'+this.uniqueId,
				viewrecords:  true,
				emptyrecords : "",
				sortorder : "desc",
				toppager: true,
				botpager: false,
				height : 'auto',
				width : enrollGridWidth ,
				loadtext : "",
				recordtext : "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				//pgtext:"",
				loadui : false,
				onSortCol:function (){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-inside-grid-'+userId+'-enrollments');
				},
				loadComplete : this.callbackEnrollUserDataGrid
			}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});

		    /*to bind the user enrollment autopopulate values*/
			$("#user-enrollment-autocomplete-"+userId ).autocomplete(
					"/?q=administration/people/user/user-enrollment-autocomplete/"+userId,{
						//extraParams : {
			            //'criteria_id' :criteriaId
			        	//},
					minChars :3,
					max :50,
					autoFill :true,
					mustMatch :false,
					matchContains :false,
					inputClass :"ac_input",
					loadingClass :"ac_loading"
					},
					"appendTo",
					"#user-enrollment-main-div-"+userId
			);
			/*Search the matched list of enrollments when press enter key*/
			$("#user-enrollment-autocomplete-"+userId).bind('keyup', function(e) {
			    if ( e.keyCode === 13 ) { // 13 is enter key
			    	$("body").data("mulitselectdatagrid").searchEnrollUserDataGrid(userId);
			    }
			});
			//$('#user-enrollment-main-div-'+userId).css("visibility","visible");
			$("#user-enrollment-main-div-"+userId+" #user-"+userId+"-enroll-search-box a.narrow-text-search").css({'position':'relative','z-index':'100'});
			}catch(e){
				// to do
			}
		},

		//Start loadOrgUsersListGrid() # Added by Velu
		loadOrgUsersListGrid : function(orgid){
			try{
			if(document.getElementById('admin-bubble-close')) {
			    $("#admin-bubble-close").click();
			}
			$(".user-enrollment-div-cls").hide();
			$('#org-users-main-div-'+orgid).css('display','block');
			$('#org-users-main-div-'+orgid).css("visibility","hidden");
			$('#datagrid-div-'+orgid+'-orgusers').css('display','none');
			var objStr = '';
			var obj    = this;
			this.entityId 	= orgid;
			this.entityType = 'orgusers';
			this.uniqueId 	= this.entityId +'-' + this.entityType;
			var url = "administration/people/organization/orguserslist/"+orgid;

		    if($("#datagrid-container-"+this.uniqueId).html()==''){
				$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+this.uniqueId);
				$('#loaderdivdatagrid-div-'+this.uniqueId).css('width','100%');
				$('#loaderdivdatagrid-div-'+this.uniqueId+' .loaderimg').css('margin','0 auto');
				$('#org-users-main-div-'+orgid).css("visibility","hidden");
			}else{
				$("#org-users-autocomplete-"+orgid).val(Drupal.t('LBL1230'));
				$("#org-users-autocomplete-"+orgid).addClass('input-field-grey');
				//$('#datagrid-container-'+this.uniqueId).setGridParam({url: this.constructUrl(url)});
				//$('#datagrid-container-'+this.uniqueId).trigger("reloadGrid",[{page:1}]);
			}

		    //HTML data for Users who are all mapped with orgonization
			var htmldata='';
			$("#datagrid-div-inside-grid-"+this.uniqueId).html('');
			htmldata +='<div id="datagrid-div-inside-grid-'+this.uniqueId+'">';
			htmldata += '<table id="datagrid-container-'+this.uniqueId+'" class="datagrid-container-common"></table>';
			htmldata += '<div id="pager-datagrid-'+this.uniqueId+'" class="pager-datagrid-common"></div>';
			htmldata += '<div id="datagrid-div-'+this.uniqueId+'">';
			htmldata +=  '<div class="user-enrollment-bottom-row-cls">';
		  	htmldata +=  '<div id="user-enrollment-container"><div class="white-btn-bg-left"></div><span> <a class="user-enroll-close-link white-btn-bg-middle" onClick="$(\'#org-users-main-div-'+orgid+'\').hide();" >'+Drupal.t('LBL123')+'</a></span><div class="white-btn-bg-right"></div></div>';
			htmldata +='</div></div></div>';
			$("#datagrid-div-inside-grid-"+this.uniqueId).html(htmldata);

			//Data Grid content call for Users who are all mapped with orgonization
			obj.orgGridWidth = 550;
			var col1Width = 80, col2Width = 120, col3Width = 130, col4Width = 90, col5Width = 130;
			if(Drupal.settings.user.language !== undefined && $.inArray(Drupal.settings.user.language, ["pt-pt", "de", "es", "fr", "it", "ru"]) != -1) {
				obj.orgGridWidth = 690;
				col1Width = 75, col2Width = 150, col3Width = 170, col4Width = 140, col5Width = 155;
			}
			$("#datagrid-container-"+this.uniqueId).jqGrid({
				url: this.constructUrl(url),
				datatype: "json",
				mtype: 'GET',
				colNames:[ Drupal.t('LBL107'), Drupal.t('LBL054'), Drupal.t('LBL073'), Drupal.t('LBL173'), Drupal.t('LBL134')],
				colModel:[ {name:'Name',index:'Name', title: false, width:col1Width, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false},
				           {name:'UserName',index:'UserName', title: false, width:col2Width, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false},
				           {name:'JobTitle',index:'JobTitle', title: false, width:col3Width, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false},
				           {name:'UserType',index:'UserType', title: false, width:col4Width, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false},
				           {name:'ManagerName',index:'ManagerName', title: false, width:col5Width, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false}],
				rowNum:5,
				rowList:[5,10,25,50],
				pager: '#pager-datagrid-'+this.uniqueId,
				viewrecords:  true,
				emptyrecords : "",
				sortorder : "asc",
				toppager: true,
				botpager: false,
				height : 140,
				width : obj.orgGridWidth,
				loadtext : "",
				recordtext : "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				//pgtext:"",
				loadui : false,
				onSortCol:function (){
					$('body').data('mulitselectdatagrid').searchOrgUsersDataGrid(orgid);
				},
				loadComplete : this.callbackDataGrid
			}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});

		    /*to bind the user enrollment autopopulate values*/
			$("#org-users-autocomplete-"+orgid ).autocomplete(
					"/?q=administration/people/organization/username-autocomplete/"+orgid,{
						//extraParams : {
			            //'criteria_id' :criteriaId
			        	//},
					minChars :3,
					max :50,
					autoFill :true,
					mustMatch :false,
					matchContains :false,
					inputClass :"ac_input",
					loadingClass :"ac_loading"
					},
					"appendTo",
					"#org-users-main-div-"+orgid
			);
	        /*Search the matched list of users when press enter key*/
			$("#org-users-autocomplete-"+orgid).bind('keyup', function(e) {
			    if ( e.keyCode === 13 ) { // 13 is enter key
			    	$("body").data("mulitselectdatagrid").searchOrgUsersDataGrid(orgid);
			    }

			});
			$('#org-users-main-div-'+orgid).css("visibility","visible");
			$("#org-users-main-div-"+orgid+" #user-"+orgid+"-orgusers-search-box a.narrow-text-search").css({'position':'relative','z-index':'100'});
			//$('#datagrid-container-'+this.uniqueId).css('width','550px');
			//$('.ui-jqgrid-htable').css('width','545px');

			this.currTheme = Drupal.settings.ajaxPageState.theme;
			if(this.currTheme == "expertusoneV2"){
			     if (navigator.userAgent.indexOf("Chrome")>0 || $.browser.msie && $.browser.version >= 8) {
				    $('#datagrid-container-'+this.uniqueId).css('width','550px');
					$('.ui-jqgrid-htable').css('width','550px');
					$('.ui-jqgrid-view').css('min-width','550px');
				   }
			      else{
			      $('#datagrid-container-'+this.uniqueId).css('width','550px');
			       $('.ui-jqgrid-htable').css('width','550px');
			       $('.ui-jqgrid-view').css('min-width','550px');
			      }

			  }
			 else{
			   $('#datagrid-container-'+this.uniqueId).css('width','550px');
			   $('.ui-jqgrid-htable').css('width','550px');
			   }
			}catch(e){
				// to do
			}
		}, //End loadOrgUsersListGrid();

		//Start  searchOrgUsersDataGrid()
		searchOrgUsersDataGrid : function(orgid){
			try{
				var searchKeyword	= $('#org-users-autocomplete-'+orgid).val();
				this.entityId 	= orgid;
				this.entityType = 'orgusers';
				this.uniqueId 	= this.entityId +'-' + this.entityType;
				$('body').data('mulitselectdatagrid').createLoader('datagrid-div-inside-grid-'+this.uniqueId);
				if(searchKeyword!='' && searchKeyword != Drupal.t('LBL1230')){
					searchKeyword = searchKeyword.replace(/\//g, "|");
					var dataGridURL = this.constructUrl("administration/people/organization/orguserslist/"+orgid+"/"+searchKeyword);
				}else{
					var dataGridURL = this.constructUrl("administration/people/organization/orguserslist/"+orgid);
				}
			    $('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL});
			    $('#datagrid-container-'+this.uniqueId).trigger("reloadGrid",[{page:1}]);
			}catch(e){
				// to do
			}
		}, //End searchOrgUsersDataGrid()

		callbackEnrollUserDataGrid : function(data){
			try{
				vtip();
				var uniqueId = data.unique_id;
				var listRows = 5;
				var splitUserId = uniqueId.split("-");
				var nAgt = navigator.userAgent;
				var verOffset ;
				this.currTheme = Drupal.settings.ajaxPageState.theme;
				if ($("#datagrid-container-"+uniqueId).getGridParam("records") <= 0) {
					$(".norecords-msg-edit-page").remove();
					$(".enroll-search-wrapper-div").css('display','none');
					//0037049: Code Re-Factoring 5. People > User > Enrollments popup > While there are no records, the left and right curves of the search tool are missing.
					$(".enroll-search-wrapper-div").next('span').css('display','none');
					//$("#user-"+splitUserId[0]+"-enroll-search-box").css('display','none');
					$(".user-enrollment-count-pipe-cls").css('display','none');
					$('#datagrid-noresult-msg-'+uniqueId).show();
					var norecordsmsg = '<div class="norecords-msg-edit-page">'+Drupal.t('MSG403')+'</div>';
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").before(norecordsmsg);
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").css('display','none');

				        if(this.currTheme == "expertusoneV2")
				        {
				         $("#gview_datagrid-container-"+uniqueId+"").css('border','none');
					    }
				}else{
					$(".norecords-msg-edit-page").remove();
					$(".enroll-search-wrapper-div").css('display','block');
					$("#user-"+splitUserId[0]+"-enroll-search-box").css('display','block');
					$(".enroll-search-wrapper-div").next('span').css('display','block');
					$(".user-enrollment-count-pipe-cls").css('display','block');
					$("#datagrid-container-"+uniqueId+"_toppager").css('display','block');
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").css('display','block');
					 if(this.currTheme == "expertusoneV2")
				        {
						 if (navigator.userAgent.indexOf("Chrome")>0) {
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td').css("padding",'0 0 0 10px');
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:first-child').css("width",'158px').css("padding",'0 0 0 10px');
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(2)').css("width",'106px').css("padding",'0 0 0 10px');
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(4)').css("width",'123px').css("padding",'0 0 0 10px');
							//}
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(3)').css("width",'113px');
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:last-child').css("width",'82px');
						 }else{
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:first-child').css("border",'0');
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(4)').css("width",'95px');
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:last-child').css("width",'90px');
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td').css("padding",'0 0 0 10px');
						if($("#datagrid-container-"+uniqueId).getGridParam("records") == 1){
						    $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:first-child').css("width",'140px').css("padding",'0 0 0 10px');
						}else{
							$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:first-child').css("width",'139px').css("padding",'0 0 0 10px');
						}
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(2)').css("width",'94px');
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(3)').css("width",'98px');
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(4)').css("width",'92px').css("padding",'0 0 0 10px');
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:last-child').css("width",'88px').css("padding",'0 0 0 12px');
				        }
				        }
				}
				//addclass for date soting
				$('#jqgh_datagrid-container-'+uniqueId+'_Date').addClass("enroll-date-sortalign");

				if ($("#datagrid-container-"+uniqueId).getGridParam("records") > listRows) {
					$("#datagrid-container-"+uniqueId+"_toppager").css('visibility','visible');
					$("#datagrid-container-"+uniqueId+"_toppager").css('display','block');
					if ( $.browser.msie && parseInt($.browser.version, 10)=='8') {
						$("#gview_datagrid-container-"+uniqueId).css('top','0');
						$("#gview_datagrid-container-"+uniqueId).css('position','relative');
						$("#datagrid-container-"+uniqueId+"_toppager").css('top','0');
		            }
					if ( $.browser.msie && parseInt($.browser.version, 10)=='8' && this.currTheme == "expertusoneV2") {
						$("#datagrid-container-"+uniqueId+"_toppager").css('top','-36px');
						$("#datagrid-container-"+uniqueId+"_toppager").css('right','1px');
						$("#datagrid-container-"+uniqueId+"_toppager .ui-pg-table").css('right','-2px');
					}
					if ( $.browser.msie && parseInt($.browser.version, 10)=='10' && this.currTheme == "expertusoneV2") {
						$("#gview_datagrid-container-"+uniqueId).css({"top":"-20px","position":"relative","border-width":"1px 0"});
						$("#datagrid-container-"+uniqueId).css({"border-left":"1px solid #e7e7e7","border-right":"1px solid #e7e7e7"});
						$("#gview_datagrid-container-"+uniqueId).find(".ui-jqgrid-hdiv").css({"border-left":"1px solid #e7e7e7","border-right":"1px solid #e7e7e7"});
						$("#datagrid-container-"+uniqueId).css("width",'550px');
						$("#datagrid-container-"+uniqueId+"_toppager").css('top','-9px');
						$("#admin-data-grid .user-enrollment-div-cls .user-enroll-input-auto-cls").css("margin-bottom","0");
					}else if( $.browser.msie && parseInt($.browser.version, 10)=='10' && this.currTheme == "expertusone" || $.browser.msie && parseInt($.browser.version, 10)=='8' && this.currTheme == "expertusone"){
						$("#gview_datagrid-container-"+uniqueId).css({"top":"-20px","position":"relative"});
						$("#datagrid-container-"+uniqueId+"_toppager").css('top','-18px');
					}
					$("#datagrid-container-"+uniqueId+"_toppager").css('width','360px');
				} else {
					$("#datagrid-container-"+uniqueId+"_toppager").css('display','none');
					if ( $.browser.msie && parseInt($.browser.version, 10)=='8') {
						$("#gview_datagrid-container-"+uniqueId).css('top','0px');
						$("#datagrid-div-"+uniqueId+" .user-enrollment-bottom-row-cls").css('margin-top','20px');
					}
				}
				$('body').data('mulitselectdatagrid').destroyLoader('datagrid-div-'+uniqueId);
				$('body').data('mulitselectdatagrid').destroyLoader('datagrid-div-inside-grid-'+uniqueId);

				$('#first_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
					if($(this).hasClass('ui-state-disabled')  == false){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-inside-grid-'+uniqueId);
						$('#loaderdivdatagrid-div-inside-grid-'+uniqueId).css('width','100%');
						$('#loaderdivdatagrid-div-inside-grid-'+uniqueId+' .loaderimg').css('margin','0 auto');
					}
				});

				$('#prev_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
					if($(this).hasClass('ui-state-disabled')  == false){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-inside-grid-'+uniqueId);
						$('#loaderdivdatagrid-div-inside-grid-'+uniqueId).css('width','100%');
						$('#loaderdivdatagrid-div-inside-grid-'+uniqueId+' .loaderimg').css('margin','0 auto');
					}
				});

				$('#next_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
					if($(this).hasClass('ui-state-disabled')  == false){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-inside-grid-'+uniqueId);
						$('#loaderdivdatagrid-div-inside-grid-'+uniqueId).css('width','100%');
						$('#loaderdivdatagrid-div-inside-grid-'+uniqueId+' .loaderimg').css('margin','0 auto');
					}
				});

				$('#last_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
					if($(this).hasClass('ui-state-disabled')  == false){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-inside-grid-'+uniqueId);
						$('#loaderdivdatagrid-div-inside-grid-'+uniqueId).css('width','100%');
						$('#loaderdivdatagrid-div-inside-grid-'+uniqueId+' .loaderimg').css('margin','0 auto');
					}
				});

				// ODD AND EVEN ROW BACKGROUND COLOR
				$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
				$('#datagrid-container-'+uniqueId+' tr.jqgfirstrow').css("background", "none");
				this.currTheme = Drupal.settings.ajaxPageState.theme;
				 if(this.currTheme == "expertusoneV2")
				  {
				     if (navigator.userAgent.indexOf("Chrome")>0 || $.browser.msie && $.browser.version == 8 || $.browser.msie && $.browser.version == 9) {
					    $('#datagrid-container-'+uniqueId).css('width','548px');
						$('.ui-jqgrid-htable').css('width','548px');
					   }
				      else
				     {
				      $('#datagrid-container-'+uniqueId).css('width','550px');
				       $('.ui-jqgrid-htable').css('width','550px');
				      }

				  }
				 else
				   {
				   $('#datagrid-container-'+uniqueId).css('width','550px');
				   $('.ui-jqgrid-htable').css('width','550px');
				   }
				var popupId = 'user-enrollment-main-div-'+splitUserId[0];
				$("#"+popupId).find("#bubble-face-table").css("top","39px").css("z-index","100");
				$("#"+popupId+" .qtip-close-button").css("top","39px").css("z-index","101");
				$('#user-enrollment-main-div-'+splitUserId[0]).css("visibility","hidden");
				var p = $("#"+splitUserId[0]);
				var position = p.position();
				var divHeight = $("#"+popupId).height();
				var parentTopPos = position.top + 200;
				if(parentTopPos > divHeight) {
					//var divTopPos = divHeight + 23;
					$("#"+popupId+" .bottom-qtip-tip-up").remove();
					$("#"+popupId+" .bottom-qtip-tip").remove();
					$("#"+popupId).append("<div class='bottom-qtip-tip active-qtip-div'></div>");
					$('#admin-data-grid .user-enrollment-div-cls .bottom-qtip-tip').css({'z-index':'101','height':'42px','left':'410px','top':'auto','background-position':'-101px -1428px','bottom':'-69px'});
					$("#"+popupId).css("bottom","60px");
				} else {
					$("#"+popupId+" .bottom-qtip-tip").remove();
					$("#"+popupId+" .bottom-qtip-tip-up").remove();
					$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div'></div>");
					$("#"+popupId).find("#bubble-face-table").css("top","39px").css("z-index","100");
					$("#"+popupId+" .qtip-close-button").css("top","39px").css("z-index","101");
					$('#admin-data-grid .user-enrollment-div-cls .bottom-qtip-tip-up').css("right","120px");
				}
				if ($("#datagrid-container-"+uniqueId).getGridParam("records") > listRows) {
				if ((nAgt.indexOf("Chrome"))>=0){
				   $("#"+popupId+" .bottom-qtip-tip").css('top','auto');
				}
				$('#datagrid-container-'+uniqueId+'_toppager').css("width","300px");
				}
				$('#user-enrollment-main-div-'+splitUserId[0]).css("visibility","visible");
				if ((verOffset=nAgt.indexOf("Chrome"))!=-1) {
					$('#gbox_datagrid-container-'+uniqueId+' th').css("padding","0px 0px 0px 0px");
					$('#datagrid-container-'+uniqueId+'_toppager').css("position","absolute").css("top","15px").css("right","-17px");
					$('#datagrid-container-'+uniqueId+'_toppager table').css("right","26px");
				}
				$('body').data('mulitselectdatagrid').destroyLoader('datagrid-div-inside-grid-'+uniqueId);

			}catch(e){
				// to do
			}
		},
		callbackViewEnrollDataGrid : function(data){
			try{
			vtip();
			var uniqueId = data.unique_id;
			var listRows = 5;
			var splitUserId = uniqueId.split("-");
			var nAgt = navigator.userAgent;
			var verOffset ;
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			if ($("#datagrid-container-"+uniqueId).getGridParam("records") <= 0) {
				$(".norecords-msg-edit-page").remove();
				$(".enroll-search-wrapper-div").css('display','none');
				$("#user-"+splitUserId[0]+"-enroll-search-box").css('display','none');
				$(".user-enrollment-count-pipe-cls").css('display','none');
				$('#datagrid-noresult-msg-'+uniqueId).show();
				var norecordsmsg = '<div class="norecords-msg-edit-page">'+Drupal.t('MSG403')+'</div>';
				$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").before(norecordsmsg);
				$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").css('display','none');

			        if(this.currTheme == "expertusoneV2")
			        {
			         $("#gview_datagrid-container-"+uniqueId+"").css('border','none');
				    }
			}else{
				$(".norecords-msg-edit-page").remove();
				$(".enroll-search-wrapper-div").css('display','block');
				$("#user-"+splitUserId[0]+"-enroll-search-box").css('display','block');
				$(".user-enrollment-count-pipe-cls").css('display','block');
				$("#datagrid-container-"+uniqueId+"_toppager").css('display','block');
				$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").css('display','block');
				 if(this.currTheme == "expertusoneV2"){
					 if (navigator.userAgent.indexOf("Chrome")>0) {
						 //$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th').css('padding','0 0 0 10px');
						 if($('#viewcourse-detail-wrapper').length == 1 || $('#viewclass-detail-wrapper').length ==1){
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td').css('padding','0 0 0 5px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th').css('padding','0 0 0 5px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:last-child').css("width",'60px').css('padding','0 0 0 0px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:first-child').css("width",'30%');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(2)').css("width",'30%');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(4)').css("width",'23%');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:last-child').css("width",'60px').css('padding','0 0 0 0px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:first-child').css("width",'30%');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(2)').css("width",'30%');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(4)').css("width", '23%');
						 }
						 $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-bdiv').css('margin-left','15px');
						// $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:last-child').css("width",'50px');
			             $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-htable').css('margin-left','15px');
			             $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:first-child').css("border-right",'1px solid #e5e5e5');
			             $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(2)').css("border-right",'1px solid #e5e5e5');
			             $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(4)').css("border-right",'1px solid #e5e5e5');
			             $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:last-child').css("border-right",'1px solid #e5e5e5');
			             $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(3)').css("border-right",'1px solid #e5e5e5');
			             $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-bdiv table').css('border-left', '1px solid #e5e5e5');
			             $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-bdiv table').css('border-right', '1px solid #e5e5e5');
			             $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-bdiv table').css('border-bottom', '1px solid #e5e5e5');
			             if($('#viewprogram-detail-wrapper').length == 1){
			            	 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:last-child').css("width",'60px').css("padding",'0 0 0 0px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:first-child').css("width",'100px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(2)').css("width",'107px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(3)').css("width",'75px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(4)').css("width",'75px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:last-child').css("width",'60px').css("padding",'0 0 0 0px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:first-child').css("width",'100px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(2)').css("width",'107px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(3)').css("width", '75px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(4)').css("width",'75px');
			             }
					 }else{
						
						 if($('#viewcourse-detail-wrapper').length == 1 || $('#viewclass-detail-wrapper').length ==1){
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td').css('padding','0 0 0 5px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th').css('padding','0 0 0 5px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:last-child').css("width",'60px').css('padding','0 0 0 0px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:first-child').css("width",'30%');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(2)').css("width",'30%');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(4)').css("width",'23%');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:last-child').css("width",'60px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:first-child').css("width",'30%');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(2)').css("width",'30%');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(4)').css("width", '23%');
						 }
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:first-child').css("border-right",'1px solid #e5e5e5');
			             $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(2)').css("border-right",'1px solid #e5e5e5');
			             $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(4)').css("border-right",'1px solid #e5e5e5');
			             $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:last-child').css("border-right",'1px solid #e5e5e5');
			             $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(3)').css("border-right",'1px solid #e5e5e5');
						 if($('#viewprogram-detail-wrapper').length == 1){
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:last-child').css("width",'60px').css("padding",'0 0 0 0px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:first-child').css("width",'90px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(2)').css("width",'107px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(3)').css("width",'75px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(4)').css("width",'70px');
						 }
					     $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-bdiv').css('margin-left','15px');
			             $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-htable').css('margin-left','15px');
			             $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:first-child').css("border-left",'1px solid #e5e5e5');
			             $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-bdiv table').css('border-left', '1px solid #e5e5e5');
			             $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-bdiv table').css('border-right', '1px solid #e5e5e5');
			             $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-bdiv table').css('border-bottom', '1px solid #e5e5e5');
						 if($('#viewprogram-detail-wrapper').length == 1){
				             $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:last-child').css("width",'60px').css("padding",'0 0 0 0px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:first-child').css("width",'90px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(2)').css("width",'107px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(3)').css("width", '75px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(4)').css("width",'70px');
						 }

						 //$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:last-child').css("padding",'0 0 0 0px').css("width",'60px');
				        }
			        }else {
			        	 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:first-child').css("width",'100');
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(2)').css("width",'100px');
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(3)').css("width",'70px');
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:last-child').css("width",'59px');
						 if($("#datagrid-container-"+uniqueId).getGridParam("records") == 1){
							    $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:first-child').css("width",'100px');
							}else{
								$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:first-child').css("width",'100px');
							}
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(2)').css("width",'100px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(3)').css("width",'70px');
							 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:last-child').css("width",'62px');
				             $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-bdiv').css('overflow','hidden');
				             $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-bdiv').css('margin-left','10px');

			        }
			}
			if ($("#datagrid-container-"+uniqueId).getGridParam("records") > listRows) {
				$("#datagrid-container-"+uniqueId+"_toppager").css('visibility','visible');
				$("#datagrid-container-"+uniqueId+"_toppager").css('display','block');
				if ( $.browser.msie && parseInt($.browser.version, 10)=='8' && this.currTheme == "expertusoneV2") {
					$("#datagrid-container-"+uniqueId+"_toppager").css('top','-35px');
					$("#datagrid-container-"+uniqueId+"_toppager").css('right','1px');
				}

			} else {
				$("#gview_datagrid-container-"+uniqueId).css('margin-top','20px');
				$("#datagrid-container-"+uniqueId+"_toppager").css('display','none');
				if ( $.browser.msie && parseInt($.browser.version, 10)=='8') {
					$("#gview_datagrid-container-"+uniqueId).css('top','0px');
				}
			}
			$('body').data('mulitselectdatagrid').destroyLoader('datagrid-div-'+uniqueId);
			$('body').data('mulitselectdatagrid').destroyLoader('datagrid-div-inside-grid-'+uniqueId);

			$('#first_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-inside-grid-'+uniqueId);
					$('#loaderdivdatagrid-div-inside-grid-'+uniqueId).css('width','100%');
					$('#loaderdivdatagrid-div-inside-grid-'+uniqueId+' .loaderimg').css('margin','0 auto');
				}
			});

			$('#prev_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-inside-grid-'+uniqueId);
					$('#loaderdivdatagrid-div-inside-grid-'+uniqueId).css('width','100%');
					$('#loaderdivdatagrid-div-inside-grid-'+uniqueId+' .loaderimg').css('margin','0 auto');
				}
			});

			$('#next_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-inside-grid-'+uniqueId);
					$('#loaderdivdatagrid-div-inside-grid-'+uniqueId).css('width','100%');
					$('#loaderdivdatagrid-div-inside-grid-'+uniqueId+' .loaderimg').css('margin','0 auto');
				}
			});

			$('#last_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-inside-grid-'+uniqueId);
					$('#loaderdivdatagrid-div-inside-grid-'+uniqueId).css('width','100%');
					$('#loaderdivdatagrid-div-inside-grid-'+uniqueId+' .loaderimg').css('margin','0 auto');
				}
			});

			// ODD AND EVEN ROW BACKGROUND COLOR
			$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
			$('#datagrid-container-'+uniqueId+' tr.jqgfirstrow').css("background", "none");
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			var popupId = 'user-enrollment-main-div-'+splitUserId[0];
			$("#"+popupId).find("#bubble-face-table").css("top","39px").css("z-index","100");
			$("#"+popupId+" .qtip-close-button").css("top","39px").css("z-index","101");
			$('#user-enrollment-main-div-'+splitUserId[0]).css("visibility","visible");
			var p = $("#"+splitUserId[0]);
			var position = p.position();
			var divHeight = $("#"+popupId).height();
			var parentTopPos = position.top + 200;
			if(parentTopPos > divHeight) {
				//var divTopPos = divHeight + 23;
				$("#"+popupId+" .bottom-qtip-tip-up").remove();
				$("#"+popupId+" .bottom-qtip-tip").remove();
				$("#"+popupId).append("<div class='bottom-qtip-tip active-qtip-div'></div>");
				$('#admin-data-grid .user-enrollment-div-cls .bottom-qtip-tip').css({'z-index':'101','height':'42px','left':'42px','top':'auto','background-position':'-101px -1428px','bottom':'-69px'});
				$("#"+popupId).css("bottom","60px");
			} else {
				$("#"+popupId+" .bottom-qtip-tip").remove();
				$("#"+popupId+" .bottom-qtip-tip-up").remove();
				$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div'></div>");
				$("#"+popupId).find("#bubble-face-table").css("top","39px").css("z-index","100");
				$("#"+popupId+" .qtip-close-button").css("top","39px").css("z-index","101");
				$('#admin-data-grid .user-enrollment-div-cls .bottom-qtip-tip-up').css("right","120px");
			}
			if ($("#datagrid-container-"+uniqueId).getGridParam("records") > listRows) {
			if ((nAgt.indexOf("Chrome"))>=0){
			   $("#"+popupId+" .bottom-qtip-tip").css('top','auto');
			}
			$('#datagrid-container-'+uniqueId+'_toppager').css("width","450px");
			}
			$('#user-enrollment-main-div-'+splitUserId[0]).css("visibility","visible");
			if ((verOffset=nAgt.indexOf("Chrome"))!=-1) {				
				$('#datagrid-container-'+uniqueId+'_toppager').css("position","relative");
				$('#gbox_datagrid-container-'+uniqueId+' th').css("padding","0px 0px 0px 0px");				
			}
			}catch(e){
				// to do
			}
		},

			loadViewEnrollmentGrid : function(qtipObj,type){
			try{
				$('.qtip-popup-visible').html('').hide();
				$('#' + qtipObj.popupDispId).qtipPopup({
					wid : qtipObj.wid,
					heg : qtipObj.heg,
					entityId : qtipObj.entityId,
					popupDispId : qtipObj.popupDispId,
					postype : qtipObj.postype,
					poslwid : (qtipObj.poslwid == '' || qtipObj.poslwid == undefined || qtipObj.poslwid == null) ? '' : qtipObj.poslwid,
					posrwid : (qtipObj.posrwid == '' || qtipObj.posrwid == undefined || qtipObj.posrwid == null) ? '' : qtipObj.posrwid,
					disp	: (qtipObj.qdis == '' || qtipObj.qdis == undefined || qtipObj.qdis == null) ? ''	: qtipObj.qdis,
					linkid	: qtipObj.linkid,
				});
				popupId = qtipObj.popupDispId;
				entId = qtipObj.entityId;
				var obj    = this;
				setTimeout(function(){
					var objStr = '';

					obj.entityId 	= qtipObj.entityId;
					obj.entityType = 'viewenroll';
					if(typeof(type)==='undefined') type = '';
					var clsId = qtipObj.entityId;
					obj.uniqueId 	= obj.entityId +'-' + obj.entityType;
					if(qtipObj.type == 'cre_sys_obt_trp'){
						var url = "administration/learning/program/view-screen/"+clsId;
						var multiselectOption = false;
					}else{
						var url = "administration/learning/catalog/view-screen/"+clsId;
						var multiselectOption = true;
					}
					var htmldata='';
					//$("#datagrid-div-inside-grid-"+this.uniqueId).html('');
					htmldata +='<div id="datagrid-div-inside-grid-'+obj.uniqueId+'">';
					htmldata += '<table id="datagrid-container-'+obj.uniqueId+'" class="datagrid-container-common"></table>';
					//htmldata += '<div id="pager-datagrid-'+this.uniqueId+'" class="pager-datagrid-common"></div>';
					htmldata += '<div id="datagrid-div-'+obj.uniqueId+'">';
					htmldata +=  '<div class="user-enrollment-bottom-row-cls">';
					htmldata += '<span class="user-enrollment-count-pipe-cls" id="user-enrollments-status-count-div-'+clsId+'"></span>';
				  	htmldata +=  '<div id="user-enrollment-container"><div class="white-btn-bg-left"></div><span> <a class="user-enroll-close-link white-btn-bg-middle" onClick="$(\'#qtip_enrollment_disp_'+clsId+' #visible-popup-'+clsId+'\').hide();" >'+Drupal.t('LBL123')+'</a></span><div class="white-btn-bg-right"></div></div>';
					htmldata +='</div></div></div>';
					//$("#datagrid-div-inside-grid-"+this.uniqueId).html(htmldata);
					$('#'+popupId+' #bubble-face-table .bubble-c #paintContent'+popupId).html(htmldata);
			    //data grid content call for user enrollments
					var dateTitle	= Drupal.t('LBL042')+'<p class="usr-enroll-grid-header-grey-cls">(' + Drupal.t('LBL699') + ')</p>';
					obj.currTheme = Drupal.settings.ajaxPageState.theme;
					 if(obj.currTheme == "expertusoneV2")
					  {
						 if (navigator.userAgent.indexOf("Chrome")>0) {
							 var enrollGridWidth=450;
						 }else {
						   var enrollGridWidth=450;
						 }
					  }else{
						  var enrollGridWidth=430;
					  }

					$("#datagrid-container-"+obj.uniqueId).jqGrid({
						url: obj.constructUrl(url),
						datatype: "json",
						mtype: 'GET',
						colNames:[ Drupal.t('LBL107'), Drupal.t('LBL054'), Drupal.t('LBL3060') , Drupal.t('LBL102'),Drupal.t('LBL668')],
						colModel:[ {name:'Name',index:'full_name', title: false, width:50, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false  },
						           {name:'UserName',index:'user_name', title: false, width:50, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false},
						           {name:'Path',index:'path', title: false, width:50, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, hidden: multiselectOption, resizable:false},
						           {name:'Status',index:'status', title: false, width:40, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, resizable:false },
						           {name:'Score',index:'score', title: false, width:30, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, resizable:false },
						           ],
						rowNum:5,
						rowList:[5,10,25,50],
						pager: '#pager-datagrid-'+obj.uniqueId,
						viewrecords:  true,
						emptyrecords : "",
						sortorder : "desc",
						toppager: true,
						botpager: false,
						height : 150,
						width : enrollGridWidth,
						loadtext : "",
						recordtext : "",
						pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
						//pgtext:"",
						loadui : false,
						onSortCol:function (){
							$('body').data('mulitselectdatagrid').createLoader('datagrid-div-inside-grid-'+clsId+'-enrollments');
						},
						loadComplete : obj.callbackViewEnrollDataGrid
					}).navGrid('#pager-datagrid-'+obj.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				},10);

				}catch(e){
					// to do
				}
			},

		// Mode - View / Edit
		// Type - Equivalence / Pre-requisite
		loadDataGrid : function (mode, type, searchKeyword, entityId, entityType, excludedId, gridObj){
			try{
			var deleteOption = (mode == 'edit' || mode == 'view_only') ? true : false;
			mode = (mode == 'view_only') ? 'view' : mode;
			var gridmode = (mode == 'edit') ? true : false;
			var multiselectOption = (mode == 'edit') ? false : true;
			var objStr = '';
			var obj = this;

			this.mode = mode;
			this.type = type;
			this.entityId = entityId;
			this.entityType = entityType;
			this.uniqueId = type+'-'+entityId+'-'+entityType;
			// Assessment does not have multi-select functionality
			var multiselectCheckbox = ' ';
			if(type != 'assessment' ){
				multiselectCheckbox = '<div class="checkbox-unselected"><input type="checkbox" id="multiselect-selectall-'+this.uniqueId+'" class="multiselect-selectall" onclick ="checkboxSelectedUnselectedCommon(this);"/></div>';
			}
			//var multiselectCheckbox = 'Delete';

			if(this.type == 'equivalence'){
				$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
				$('.catalog-course-basic-addedit-form-container').css('width','625px');
				$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
				$('.catalog-course-basic-addedit-form-container').css('width','625px');
				$('.catalog-course-basic-addedit-form-container').css('min-height','420px');
				$('.admin-course-tab-datagrid-wrapper').css('min-height','420px');
				if($.browser.mozilla){	
					$('#catalog-course-basic-addedit-form-container #admin-data-grid .admin-save-button-container.admin-save-button-container-view-mode').css('margin-top','86px');
				}
				
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					colNames:[ Drupal.t('LBL083'), Drupal.t('LBL096'), '', multiselectCheckbox],
					colModel:[ {name:'Title',index:'title', title: false, width:400, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false  },
					           {name:'Code',index:'code', title: false, width:100, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false},
					           {name:'Action',index:'Remove', title: false, width:20, 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: deleteOption, sortable: false, resizable:false },
					           {name:'MultiselectCheck',index:'MultiselectCheck', title: false, width:17, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, hidden: multiselectOption, resizable:false}],
					rowNum:10,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: "auto",
					width: 610,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});

			}else if(this.type == 'TPAttachCourse'){
				 var str = $('#program_attach_tabs .ui-state-active').attr('id');
				 var currmod = str.split('-');
				 entityId = entityId+'-'+currmod[2];
				 //console.log(strarray[2]);
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					colNames:[ Drupal.t('LBL083'),Drupal.t('LBL096'), 'Action', multiselectCheckbox],
					colModel:[ {name:'Title',index:'title', title: false, width:195, 'widgetObj':objStr, sortable: true, formatter: obj.displayGridValues  },
					           {name:'Code',index:'code', title: false, width:80, 'widgetObj':objStr, sortable: true, formatter: obj.displayGridValues},
					           {name:'Action',index:'Remove', title: false, width:20, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false,  hidden: deleteOption },
					           {name:'MultiselectCheck',index:'MultiselectCheck', title: false, width:17, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, hidden: multiselectOption}],
					rowNum:6,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: 'auto',
					width: 390,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				setQtipPosition();
			}else if(this.type == 'CountrySetting'){
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					colNames:[ Drupal.t('LBL107'),Drupal.t('LBL096'), ''],//Name, Code,
					colModel:[ {name:'Title',index:'name', title: false, width:210, 'widgetObj':objStr, sortable: true, formatter: obj.displayGridValues},
					           {name:'Code',index:'code', title: false, width:128, 'widgetObj':objStr, sortable: true, formatter: obj.displayGridValues},
					           {name:'Enable',index:'ActionList', title: false, width:98, 'widgetObj':objStr, sortable: false, formatter: obj.displayGridValues}],
					rowNum:15,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: 'auto',
					width: 460,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
			}else if(this.type == 'PaymentMethod'){
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					colNames:[ '','',''],
					colModel:[ {name:'Title',index:'CourseTitle', title: false, width:64, 'widgetObj':objStr, sortable: false, formatter: obj.displayGridValues  },
					           {name:'code',index:'CourseCode', title: false, width:496, 'widgetObj':objStr, sortable: false, formatter: obj.displayGridValues},
					           {name:'Enable',index:'ActionList', title: false, width:140, 'widgetObj':objStr, sortable: false, formatter: obj.displayGridValues}],
					rowNum:15,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: 'auto',
					width: 700,
					loadtext: "",
					recordtext: "",
					//pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				setQtipPosition();
			}else if(this.type == 'SurAssAttachQuestion'){
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					colNames:[ Drupal.t('LBL325'), 'Action', multiselectCheckbox],
					colModel:[
					          // {name:'Code',index:'QuestionCode', title: true, width:80, 'widgetObj':objStr, sortable: false, formatter: obj.displayGridValues},
					           {name:'Title',index:'question_txt', title: false, width:195, 'widgetObj':objStr, sortable: true, formatter: obj.displayGridValues  },
					           {name:'Action',index:'Remove', title: false, width:20, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, hidden: deleteOption },
					           {name:'MultiselectCheck',index:'MultiselectCheck', title: false, width:17, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, hidden: multiselectOption}],
					rowNum:6,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: 'auto',
					width: 465,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				setQtipPosition();
			}else if(this.type == 'prerequisite'){				
				$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
				$('.catalog-course-basic-addedit-form-container').css('width','625px');
				$('.catalog-course-basic-addedit-form-container').css('min-height','420px');
				$('.admin-course-tab-datagrid-wrapper').css('min-height','420px');
				if($.browser.mozilla){				
					$('#catalog-course-basic-addedit-form-container #admin-data-grid .admin-save-button-container.admin-save-button-container-view-mode').css('padding-top','82px');
				}
				
				if(entityType == 'cre_sys_obt_trp'){
					typeviewoption= false;
					var titleWidth	= 200;
					var rowTplist 	= 10;
				} else {
					typeviewoption= true;
					var titleWidth	= 400;
					var rowTplist 	= 10;
				}

				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					colNames:[ Drupal.t('LBL083'),Drupal.t('LBL096'), Drupal.t('LBL036'),'', multiselectCheckbox],
					colModel:[ {name:'Title',index:'title', title: false, width:titleWidth, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false  },
					           {name:'Code',index:'code', title: false, width:100, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true,resizable:false },
					           {name:'Type',index:'Type', title: true, width:110, 'widgetObj':objStr, formatter: obj.displayGridValues,hidden: typeviewoption, sortable: true, resizable:false },
					           {name:'Action',index:'Remove', title: false, width:20, 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: deleteOption, sortable: false,resizable:false },
					           {name:'MultiselectCheck',index:'MultiselectCheck', title: true, width:17, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, hidden: multiselectOption,resizable:false}],
					rowNum:rowTplist,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					//sortorder: "desc",
					toppager: true,
					botpager: false,
					height: "auto",
					width: 610,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});

			} else if(this.type == 'survey'){
				var hideStatusOption = (mode == 'edit' || mode == 'view_only') ? true : false;
				var hidePathOption = (mode == 'view' && entityType != 'cre_sys_obt_cls') ? false : true;
				var hideShareOption = (mode == 'view') ? false : true;
				var hideTitleWidth = (mode == 'edit' || mode == 'view_only') ? 260 : 180;
				var hideCodeWidth = (mode == 'edit' || mode == 'view_only') ? 80 : 100;
				$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
				$('.catalog-course-basic-addedit-form-container').css('width','625px');

				if(entityType == 'cre_sys_obt_cls'){
					var rowTplist 	= crs_cls_listcount;
					var optWidth = 100;
				} else {
					var rowTplist 	= 10;
					var optWidth = 130;
				}

				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					colNames:[ Drupal.t('LBL107'), Drupal.t('LBL096'), Drupal.t('LBL661'),Drupal.t('LBL3060'), '','', multiselectCheckbox],
					colModel:[ {name:'Name',index:'title', title: false, width:hideTitleWidth, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true  },
					           {name:'Code',index:'code', title: false, width:hideCodeWidth, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true},
					           {name:'ViewOption',index:'view_option', title: false, width:optWidth, 'widgetObj':objStr, formatter: obj.displayGridValues,hidden: hideStatusOption, sortable: multiselectOption},
					           {name:'Path',index:'path', title: false, width:130, 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: hidePathOption, sortable: true  },
							   {name:'SurveyLink',index:'survey_link', title: false, width:26, 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: hideShareOption, sortable: false },
							   {name:'Action',index:'Remove', title: false, width:20, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, hidden: deleteOption },
					           {name:'MultiselectCheck',index:'MultiselectCheck', title: false, width:20, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, hidden: multiselectOption}],
					rowNum:rowTplist,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: 'auto',
					width: 610,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					afterInsertRow: obj.afterInsertRow,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				setQtipPosition();
			} else if(this.type == 'assessment'){
				//console.log(mode);
				var hidePathOption = (mode == 'view' && entityType != 'cre_sys_obt_cls') ? false : true;
				var hideStatusOption = (mode == 'edit' || mode == 'view_only') ? true : false;
				var hideTitleWidth = (mode == 'edit' || mode == 'view_only') ? 40 : 110;
				var hideCodeWidth = (mode == 'edit' || mode == 'view_only') ? 40 : 120;
				var hideCheckWidth = (mode == 'edit' || mode == 'view_only') ? 8 : 20;
				$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
				$('.catalog-course-basic-addedit-form-container').css('width','625px');

				if(entityType == 'cre_sys_obt_cls'){
					var rowTplist 	= crs_cls_listcount;
				} else {
					var rowTplist 	= 10;
				}
				var preAsstLabel = Drupal.t('LBL1253')+" "+Drupal.t('Assessment');
				if(Drupal.settings.user.language == 'es') {
					preAsstLabel = Drupal.t('LBL1253');
				}
				if(mode!='edit'){
					//console.log("view mode");
					var colNames = [ Drupal.t('LBL107'),Drupal.t('LBL857'), Drupal.t('LBL661'), preAsstLabel,Drupal.t('LBL694'), Drupal.t('LBL3060'),'', ''];
					var colModel = [ {name:'Name',index:'title', title: false, width:hideTitleWidth, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: gridmode  },
					           {name:'Maximum Attempts',index:'no_of_attempts', title: false, width:70, 'widgetObj':objStr, formatter: obj.displayGridValues,hidden: hideStatusOption, editoptions: {size:10, maxlength: 10}, editable:true, sortable: false},
					           {name:'ViewOption',index:'view_option', title: false, width:100, 'widgetObj':objStr, formatter: obj.displayGridValues,hidden: hideStatusOption, sortable: false},
					           {name:'pre_status',index:'pre_status', title: false, width:110, 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: hideStatusOption, sortable: false},
					           {name:'preview_option',index:'preview_option', title: false, width:70, 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: hideStatusOption, sortable: false},
					           {name:'path',index:'path', title: false, width:110, 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: hidePathOption, sortable: false},
					           {name:'module',index:'module', title: false, width:160, 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: true, sortable: true  },
					           {name:'Action',index:'Remove', title: false, width:30, 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: deleteOption, sortable: false },
					           ];

				}else{
					var colNames = [ Drupal.t('LBL107'), Drupal.t('LBL096'), multiselectCheckbox];
					var colModel = [ {name:'Name',index:'title', title: false, width:hideTitleWidth, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: gridmode  },
					           {name:'Code',index:'code', title: false, width:hideCodeWidth, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: gridmode},
					           {name:'MultiselectCheck',index:'MultiselectCheck', title: false, width:hideCheckWidth, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, hidden: multiselectOption}];

				}
				var str = $('#program_attach_tabs .ui-state-active').attr('id');
				if(str != '' && str != 'undefined' && str != undefined){
					 var currmod = str.split('-');
					 entityId = entityId+'-'+currmod[2];
				 }
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					colNames:colNames,
					colModel:colModel,
					rowNum:rowTplist,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortorder: "asc",
					toppager: true,
					botpager: false,
					height: 'auto',
					width: 610,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					afterInsertRow: obj.afterInsertRow,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				setQtipPosition();
			} else if(this.type == 'enrolluser'){
				var obj = this;
				url = this.constructUrl("ajax/administration/getSessionStartEndDates/" + entityId);
				$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
						obj.sesStartEndDate = result;
					}
				});
				$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
				$('.catalog-course-basic-addedit-form-container').css('width','625px');
				if(obj.mode == 'edit'){
					var classname = 'enroll-user-check-all-edit';
					$('.catalog-course-basic-addedit-form-container').removeClass("enroll-user-view-datagrid");
					$('.catalog-course-basic-addedit-form-container').addClass("enroll-user-edit-datagrid");
				}
				else{
					var classname = 'enroll-user-check-all';					
				}
				if(obj.mode == 'view'){
				$('.catalog-course-basic-addedit-form-container').removeClass("enroll-user-edit-datagrid");
				$('.catalog-course-basic-addedit-form-container').addClass("enroll-user-view-datagrid");
				}
				var hideStatusOption = (mode == 'edit') ? true : false;
				var hideOption = (mode == 'edit') ? false : true;
				var searchType = $('#search_all_enroll_type-hidden').val();
				this.currTheme = Drupal.settings.ajaxPageState.theme;
				if(this.currTheme == "expertusoneV2"){
					 if (navigator.userAgent.indexOf("Chrome")>0 || navigator.userAgent.indexOf('Safari')>0) 
						 var gridWidth 		= 610;
					 else
						 var gridWidth 		= 609;
				}else{
					var gridWidth 		= 622;
				}
				var sortName = (mode == 'edit') ? "full_name" : "Date";
				var usernameWidth; 
				if(Drupal.settings.user.language !== undefined && $.inArray(Drupal.settings.user.language, ["fr","it","ru"]) != -1)
					usernameWidth = 107;
				else
					usernameWidth = 100;
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+encodeURIComponent(searchKeyword)+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					postData: {searhType : searchType},
					colNames:[ Drupal.t('LBL107'),Drupal.t('LBL054'),Drupal.t('Manager'),Drupal.t('Organization'), Drupal.t('LBL941'),Drupal.t('DeliveryType'),Drupal.t('LBL042')+'<p class="enroll-date-text">('+Drupal.t('LBL699')+')</p>', Drupal.t('LBL668'),Drupal.t('LBL102'), multiselectCheckbox],
					colModel:[ { name:'FullName',index:'full_name', title: false, width:110,'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues },
					           { name:'Username',index:'user_name', title: false, width:usernameWidth, 'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues},
					           { name:'Manager',index:'manager_name', title: false, width:110,'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues, hidden :hideOption },
					           { name:'Organization',index:'organization_name', title: false, width:110,'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues, hidden :hideOption  },
					           { name:'RegFrom',index:'RegFrom', title: false, width:90, 'widgetObj':objStr, sortable: false,formatter: obj.enrollUserDisplayGridValues, hidden: hideStatusOption },
					           { name:'DeliveryType',index:'DeliveryType', title: false, width:90, 'widgetObj':objStr, sortable: false,formatter: obj.enrollUserDisplayGridValues, hidden: true },
					          /* { name:'Status',index:'Status', title: false, width:105, 'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues, hidden: hideStatusOption },*/
					           { name:'Date',index:'Date', title: false, width:75, 'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues, hidden: hideStatusOption },
					           { name:'Score',index:'score', title: false, width:65	, 'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues, hidden: hideStatusOption },
					           { name:'Status',index:'Status', title: false, width:105, 'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues, hidden: hideStatusOption },
					           { name:'MultiselectCheck',index:'MultiselectCheck', classes:classname, title: false, width:19, 'widgetObj':objStr, sortable: false, formatter: obj.enrollUserDisplayGridValues}],
					rowNum:crs_cls_listcount,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortname: sortName,
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: 'auto',
					width: gridWidth,
					headertitles:true,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				if(mode == "view" && Drupal.settings.ajaxPageState.theme== "expertusoneV2"){

				}
				
              }else if(this.type == 'shareuser'){
			    
			
				console.log("console::#datagrid-container-"+this.uniqueId);
				$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
				$('.catalog-course-basic-addedit-form-container').css('width','625px');
				if(obj.mode == 'edit'){
					var classname = 'enroll-user-check-all-edit';
					$('.catalog-course-basic-addedit-form-container').removeClass("enroll-user-view-datagrid");
					$('.catalog-course-basic-addedit-form-container').addClass("enroll-user-edit-datagrid");
				}
				else{
					var classname = 'enroll-user-check-all';					
				}
				if(obj.mode == 'view'){
				$('.catalog-course-basic-addedit-form-container').removeClass("enroll-user-edit-datagrid");
				$('.catalog-course-basic-addedit-form-container').addClass("enroll-user-view-datagrid");
				}
				var hideStatusOption = (mode == 'edit') ? true : false;
				var hideOption = (mode == 'edit') ? false : true;
				var searchType = $('#search_all_enroll_type-hidden').val();
				this.currTheme = Drupal.settings.ajaxPageState.theme;
				if(this.currTheme == "expertusoneV2"){
					 if (navigator.userAgent.indexOf("Chrome")>0 || navigator.userAgent.indexOf('Safari')>0) 
						 var gridWidth 		= 610;
					 else
						 var gridWidth 		= 609;
				}else{
					var gridWidth 		= 622;
				}
				var sortName = (mode == 'edit') ? "full_name" : "Date";
				var colNames;
				var colModel;
			
	            if(mode!='edit'){

	            	colNames = [ Drupal.t('LBL107'),Drupal.t('Organization'),Drupal.t('Groups'), ''];
	                  colModel = [ { name:'FullName',index:'full_name', title: false, width:120,'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues },
						          // { name:'Username',index:'user_name', title: false, width:180, 'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues},
						           { name:'Organization',index:'organization_name', title: false, width:120,'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues  },
						           { name:'GroupName',index:'name', title: false, width:140, 'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues, sortable: true },
						           {name:'Action',index:'Remove', title: false, width:30, 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: deleteOption, sortable: false },
						           ];
				}else{
					colNames = [ Drupal.t('LBL107'),Drupal.t('Organization'),Drupal.t('Groups'), multiselectCheckbox];
					colModel = [ { name:'FullName',index:'full_name', title: false, width:100,'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues },
					          // { name:'Username',index:'user_name', title: false, width:180, 'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues},
					           { name:'Organization',index:'organization_name', title: false, width:100,'widgetObj':objStr, sortable: true,formatter: obj.enrollUserDisplayGridValues  },
					           { name:'GroupName',index:'name', title: false, width:130, 'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues, sortable: true },
					           {name:'MultiselectCheck',index:'MultiselectCheck', title: false, width:30, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, hidden: multiselectOption}];

				}			
				
	
				
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+encodeURIComponent(searchKeyword)+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					postData: {searhType : searchType},		
					colNames : colNames,
				    colModel : colModel,	
					rowNum:10,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortname: sortName,
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: 'auto',
					width: gridWidth,
					headertitles:true,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				if(mode == "view" && Drupal.settings.ajaxPageState.theme== "expertusoneV2"){

				}
				
			


			} else if(this.type == 'enrolltpuser'){
				//console.log("enrolltpuser >> "+ excludedId);
				//console.log("unique id " + this.uniqueId);
				if(excludedId == 'R'){
				 // console.log($('input[name="selected_enroll_path"]').val())
				  var module_id = $('input[name="selected_enroll_path"]').val();
				  excludedId = module_id+'-R';
				 }
				$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
				$('.catalog-course-basic-addedit-form-container').css('width','625px');
				$('.catalog-course-basic-addedit-form-container').addClass('enrollTPuserParentDiv');
				var hideStatusOption = (mode == 'edit') ? true : false;
				var hideOption = (mode == 'edit') ? false : true;
				var searchType = $('#search_all_enroll_type-hidden').val();
				var sortName = (mode == 'edit') ? "full_name" : "Date";
				var TpusernameWidth; 
				if(Drupal.settings.user.language !== undefined && $.inArray(Drupal.settings.user.language, ["fr","it","ru"]) != -1)
					TpusernameWidth = 146;
				else
					TpusernameWidth = 120;
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+encodeURIComponent(searchKeyword)+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					postData: {searhType : searchType},
					colNames:[  Drupal.t('LBL107'),Drupal.t('LBL054'),Drupal.t('LBL3060'),Drupal.t('Manager'),Drupal.t('Organization'), Drupal.t('LBL042'),Drupal.t('LBL102'), multiselectCheckbox],
					colModel:[ { name:'FullName',index:'full_name', title: false, width:140, 'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues, sortable: true },
					           { name:'UserName',index:'user_name', title: false, width:TpusernameWidth, 'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues , sortable: true },
					           { name:'Path',index:'path', title: false, width:100, 'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues ,  hidden: hideStatusOption },
					           { name:'Manager',index:'manager_name', title: false, width:150,'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues, sortable: true, hidden :hideOption },
					           { name:'Organization',index:'organization_name', title: false, width:150,'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues, sortable: true, hidden :hideOption  },
					           //{ name:'Status',index:'Status', title: false, width:80, 'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues, hidden: hideStatusOption, sortable: true },
					           { name:'Date',index:'Date', title: false, width:100, 'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues, hidden: hideStatusOption, sortable: true },
					           { name:'Status',index:'Status', title: false, width:160, 'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues, hidden: hideStatusOption, sortable: true },
					           { name:'MultiselectCheck',index:'MultiselectCheck', title: false, width:19, 'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues, sortable: false}
					         ],
					rowNum:10,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortname: sortName,
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: 'auto',
					width: 610,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				$('#jqgh_datagrid-container-'+type+'-'+entityId+'-'+entityType+'_RegFrom').removeClass('ui-jqgrid-sortable');

			} else if(this.type == 'enrolltprecertify'){
				//$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
				//$('.catalog-course-basic-addedit-form-container').css('width','625px');
				//$('.catalog-course-basic-addedit-form-container').addClass('enrollTPuserParentDiv');
				var hideStatusOption = (mode == 'edit') ? true : false;
				var hideOption = (mode == 'edit') ? false : true;
				var searchType = $('#search_all_enroll_type-hidden').val();
				var sortName = (mode == 'edit') ? "full_name" : "Date";
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+encodeURIComponent(searchKeyword)+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					postData: {searhType : searchType},
					colNames:[  Drupal.t('LBL3060'),Drupal.t('LBL3089'),Drupal.t('LBL706'),Drupal.t('LBL102')],
					colModel:[ { name:'Path',index:'Path', title: false, width:100, 'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues},
					           { name:'EnrollmentDate',index:'EnrollmentDate', title: false, width:100, 'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues, sortable: true },
					           { name:'CompletionDate',index:'CompletionDate', title: false, width:100, 'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues, sortable: true },
					           { name:'Status',index:'Status', title: false, width:80, 'widgetObj':objStr, formatter: obj.enrollUserDisplayGridValues,sortable: true }
					          
					         ],
					rowNum:5,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortname: sortName,
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: 'auto',
					width: 550,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				$('#jqgh_datagrid-container-'+type+'-'+entityId+'-'+entityType+'_RegFrom').removeClass('ui-jqgrid-sortable');

			} else if(this.type == 'RoleDisplayUsers'){
				$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
				$('.catalog-course-basic-addedit-form-container').css('width','625px');
				var searchType = $('#search_all_addusers_type-hidden').val();
				excludedId = excludedId==""?0:excludedId;
				if(mode == 'viewscreen') {
					var colNames1 = [  Drupal.t('LBL107'),Drupal.t('LBL054'),Drupal.t('LBL102')];
					//colNames:[  'UserName','FullName','Status', ''],
					var colModel1 = [ { name:'FullName',index:'full_name', title: false, width:200, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
					                  { name:'UserName',index:'user_name', title: false, width:140, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
					                  { name:'Status',index:'Status', title: false, width:90, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
					                ];
				}else {
					var colNames1 = [  Drupal.t('LBL107'),Drupal.t('LBL054'),Drupal.t('LBL102'), ''];
					//colNames:[  'UserName','FullName','Status', ''],
					var colModel1 = [ { name:'FullName',index:'full_name', title: false, width:200, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
					                  { name:'UserName',index:'user_name', title: false, width:140, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
					                  { name:'Status',index:'Status', title: false, width:90, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
					           //{ name:'MultiselectCheck',index:'deleteAddedUser', title: false, width:19, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: false},
					                  { name:'Action',index:'Remove', title: false, width:50, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: false, hidden: deleteOption },
					                ];
				}
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("roles/users/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'POST',
					postData: {searhType : searchType},
					colNames:colNames1,
					//colNames:[  'UserName','FullName','Status', ''],
					colModel:colModel1,
					rowNum:5,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: 'auto',
					width: 470,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});

			 }else if(this.type == 'RoleDisplayOwners'){
					$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
					$('.catalog-course-basic-addedit-form-container').css('width','625px');
					var searchType =$('#search_all_user_type-hidden').val();
					excludedId = excludedId==""?0:excludedId;
					if(mode == 'viewscreen') {
						var colNames1 = [  Drupal.t('LBL107'),Drupal.t('LBL054'),Drupal.t('Groups')];
						//colNames:[  'UserName','FullName','Status', ''],
						var colModel1 = [ { name:'FullName',index:'full_name', title: false, width:200, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
						                  { name:'UserName',index:'user_name', title: false, width:140, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
						                  { name:'GroupName',index:'name', title: false, width:150, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
						                  //{ name:'Status',index:'Status', title: false, width:90, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
						                ];
					}else {
						var colNames1 = [  Drupal.t('LBL107'),Drupal.t('LBL054'),Drupal.t('Groups'), ''];
						//colNames:[  'UserName','FullName','Status', ''],
						var colModel1 = [ { name:'FullName',index:'full_name', title: false, width:200, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
						                  { name:'UserName',index:'user_name', title: false, width:140, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
						                  { name:'GroupName',index:'name', title: false, width:150, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
						                  //{ name:'Status',index:'Status', title: false, width:90, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
						           //{ name:'MultiselectCheck',index:'deleteAddedUser', title: false, width:19, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: false},
						                  { name:'Action',index:'Remove', title: false, width:50, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: false, hidden: deleteOption },
						                ];
					}
					$("#datagrid-container-"+this.uniqueId).jqGrid({
						url: this.constructUrl("roles/users/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
						datatype: "json",
						mtype: 'POST',
						postData: {searhType : searchType},
						colNames:colNames1,
						//colNames:[  'UserName','FullName','Status', ''],
						colModel:colModel1,
						rowNum:5,
						rowList:[10,25,50],
						pager: '#pager-datagrid-'+this.uniqueId,
						viewrecords:  true,
						multiselect: false,
						emptyrecords: "",
						sortorder: "desc",
						toppager: true,
						botpager: false,
						height: 'auto',
						width: 470,
						loadtext: "",
						recordtext: "",
						pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
						//pgtext : "",
						loadui:false,
						onSortCol:function (){
							$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
						},
						loadComplete:obj.callbackDataGrid
					}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});

					/*if(Drupal.settings.ajaxPageState.theme== "expertusoneV2")
			        {

					if (navigator.userAgent.indexOf("Chrome")>0) {
					$("#datagrid-container-"+this.uniqueId).find('tr td:nth-child(1)').css("width","194px");
					$("#datagrid-container-"+this.uniqueId).find('tr td:nth-child(2)').css("width","137px");
					$("#datagrid-container-"+this.uniqueId).find('tr td:nth-child(3)').css("width","90px");
					}else{
					$("#datagrid-container-"+this.uniqueId).find('tr td:nth-child(1)').css("width","170px");
					$("#datagrid-container-"+this.uniqueId).find('tr td:nth-child(2)').css("width","117px");
					$("#datagrid-container-"+this.uniqueId).find('tr td:nth-child(3)').css("width","75px");
					}

			        }*/

			 }  else if(this.type == 'grpAddUsers'){
					var hideStatusOption = (mode == 'edit') ? true : false;
					var searchType = $('#search_all_addusers_type-hidden').val();

					$("#datagrid-container-"+this.uniqueId).jqGrid({
						url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+encodeURIComponent(searchKeyword)+"/"+entityId+"/"+entityType+"/"+excludedId),
						datatype: "json",
						mtype: 'GET',
						postData: {searhType : searchType},
						colNames:[  Drupal.t('LBL107'),Drupal.t('LBL054'), multiselectCheckbox],
						colModel:[ { name:'FullName',index:'full_name', title: false, width:150, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
						           { name:'UserName',index:'user_name', title: false, width:150, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
						          // { name:'Status',index:'Status', title: false, width:30, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, hidden: hideStatusOption },
						           { name:'MultiselectCheck',index:'MultiselectCheck', title: false, width:20, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: false},
						          // { name:'Action',index:'Remove', title: false, width:20, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: false, hidden: deleteOption },
						         ],
						rowNum:5,
						rowList:[10,25,50],
						pager: '#pager-datagrid-'+this.uniqueId,
						viewrecords:  true,
						multiselect: false,
						emptyrecords: "",
						sortorder: "desc",
						toppager: true,
						botpager: false,
						height: 'auto',
						width: 469,
						loadtext: "",
						recordtext: "",
						pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
						//pgtext : "",
						loadui:false,
						onSortCol:function (){
							$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
						},
						loadComplete:obj.callbackDataGrid
					}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
					setQtipPosition();

				 } else if(this.type == 'grpAddOwners'){
						//$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
						//$('.catalog-course-basic-addedit-form-container').css('width','625px');
						var hideStatusOption = (mode == 'edit') ? true : false;
						var searchType = $('#search_all_user_type-hidden').val();
						$("#datagrid-container-"+this.uniqueId).jqGrid({
							url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+encodeURIComponent(searchKeyword)+"/"+entityId+"/"+entityType+"/"+excludedId),
							datatype: "json",
							mtype: 'GET',
							postData: {searhType : searchType},
							colNames:[  Drupal.t('LBL107'),Drupal.t('LBL054'),Drupal.t('Groups'), multiselectCheckbox],
							colModel:[ { name:'FullName',index:'full_name', title: false, width:140, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
							           { name:'UserName',index:'user_name', title: false, width:140, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
							           { name:'GroupName',index:'name', title: false, width:140, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true },
							          // { name:'Status',index:'Status', title: false, width:30, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, hidden: hideStatusOption },
							           { name:'MultiselectCheck',index:'MultiselectCheck', title: false, width:30, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: false},
							          // { name:'Action',index:'Remove', title: false, width:20, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: false, hidden: deleteOption },
							         ],
							rowNum:5,
							rowList:[10,25,50],
							pager: '#pager-datagrid-'+this.uniqueId,
							viewrecords:  true,
							multiselect: false,
							emptyrecords: "",
							sortorder: "desc",
							toppager: true,
							botpager: false,
							height: 'auto',
							width: 469,
							loadtext: "",
							recordtext: "",
							pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
							//pgtext : "",
							loadui:false,
							onSortCol:function (){
								$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
							},
							loadComplete:obj.callbackDataGrid
						}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
						setQtipPosition();

				 }  else if(this.type == 'permissions'){
				//var hideStatusOption = (mode == 'edit') ? true : false;
				//var searchType = $('#search_all_keywords_type-hidden').val();

				$("#datagrid-container-keywords-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					colNames:[  Drupal.t('Capabilities'),multiselectCheckbox],
					colModel:[ { name:'Capabilities',index:'Capabilities', title: false, width:115, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: true},
					           { name:'MultiselectCheck',index:'MultiselectCheck', title: false, width:19, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: false},
					         ],
					rowNum:50,
					rowList:[3,6,9,12],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortorder: "asc",
					toppager: false,
					botpager: false,
					height: 110,
					width: 350,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
			 }else if(this.type == 'addkeywords'){
				//var hideStatusOption = (mode == 'edit') ? true : false;
				//var searchType = $('#search_all_keywords_type-hidden').val();

				$("#datagrid-container-keywords-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					colNames:[  Drupal.t('LBL983'),Drupal.t('LBL229'),''],
					colModel:[ { name:'Keywords',index:'keywords', title: false, width:120, 'widgetObj':objStr, sortable: true, formatter: obj.enrollUserDisplayGridValues },
					           { name:'KeywordDescription',index:'keyworddescription', title: false, width:145, 'widgetObj':objStr, sortable: true, formatter: obj.enrollUserDisplayGridValues },
					           { name:'Add to Template',index:'AddToTemplate', title: false, width:60, 'widgetObj':objStr, sortable: false,formatter: obj.enrollUserDisplayGridValues }
					         ],
					rowNum:50,
					rowList:[6,12,18],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					//sortorder: false,
					toppager: false,
					botpager: false,
					height: "auto",
					width: 440,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});

			} else if(this.type == 'content'){
				$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
				$('.catalog-course-basic-addedit-form-container').css('width','625px');
				var seqDragWdh = (mode == 'edit') ? 30 : 50;
				var colnames, colmodel;
				if(mode=='edit'){
					colnames = [ Drupal.t('LBL083'),
					           Drupal.t('LBL036'),
					           Drupal.t('LBL854'),
					           '<span class="jq-grid-no-padding">'+Drupal.t('LBL857')+'</span>',
					           '<span class="jq-grid-no-padding">'+Drupal.t('LBL604') + '</span><span class="time-zone-text"> (' + Drupal.t('LBL605') + ')</span>',
					           multiselectCheckbox
					         ];
					colmodel = [
					           { name:'Title',index:'code', title: false, width:170, 'widgetObj':objStr, formatter: obj.displayGridValues,  sortable: true },
					           { name:'Type',index:'type_name', title: false, width:95, 'widgetObj':objStr, formatter: obj.displayGridValues,  sortable: true },
					           { name:'Lessons',index:'total_lesson', title: false, width:95, 'widgetObj':objStr, formatter: obj.displayGridValues,  sortable: true },
					           { name:'Maximum Attempts',index:'max_attempts', title: false, width:100, 'widgetObj':objStr, formatter: obj.displayGridValues, editoptions: {size:10, maxlength: 10}, editable:true, sortable: false},
					           { name:'Validity Days',index:'validity_days', title: false, width:120, 'widgetObj':objStr, formatter: obj.displayGridValues, editoptions: {size:10, maxlength: 10}, editable:true, sortable: false },
					           { name:'MultiselectCheck',index:'MultiselectCheck', title: true, width:30, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, hidden: multiselectOption},
					         ];
				}else{
					colnames=[ Drupal.t('LBL083'),
					           Drupal.t('LBL036'),
					           Drupal.t('LBL854'),
					           Drupal.t('LBL857'),
					           Drupal.t('LBL604') + '<span class="time-zone-text"> (' + Drupal.t('LBL605') + ')</span>',
					           ''
					         ];
					colmodel=[
					           { name:'Title',index:'code', title: false, width:170, 'widgetObj':objStr, formatter: obj.displayGridValues,  sortable: false },
					           { name:'Type',index:'type_name', title: false, width:95, 'widgetObj':objStr, formatter: obj.displayGridValues,  sortable: false },
					           { name:'Lessons',index:'total_lesson', title: false, width:95, 'widgetObj':objStr, formatter: obj.displayGridValues,  sortable: false },
					           { name:'MaximumAttempts',index:'max_attempts', title: false, width:100, 'widgetObj':objStr, formatter: obj.displayGridValues, editoptions: {size:10, maxlength: 10}, editable:true, sortable: false},
					           { name:'ValidityDays',index:'validity_days', title: false, width:120, 'widgetObj':objStr, formatter: obj.displayGridValues, editoptions: {size:10, maxlength: 10}, editable:true, sortable: false },
					           { name:'Action',index:'Remove', title: true, width:30, 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: deleteOption, sortable: false }
					         ];
				}
				var hideStatusOption = (mode == 'edit') ? true : false;
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+encodeURIComponent(searchKeyword)+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					colNames: colnames,
					colModel: colmodel,
					onSelectRow: function(id){
						if(id && id!==lastsel2){
							jQuery('#rowed5').jqGrid('restoreRow',lastsel2);
							jQuery('#rowed5').jqGrid('editRow',id,true);
							lastsel2=id;
						}
					},
					rowNum:crs_cls_listcount,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: 'auto',
					width: 610,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
			} else if(this.type == 'session'){
				var TypeHidden = ($('#add-session-delivery-type'+this.uniqueId).val() == "lrn_cls_dty_ilt") ? true : false;
				var InstructorHidden = ($('#add-session-delivery-type'+this.uniqueId).val() == "lrn_cls_dty_ilt") ? false : true;
				/*$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
				$('.catalog-course-basic-addedit-form-container').css('width','625px');*/
				var colNames,colModel;
				if($('#add-session-delivery-type'+this.uniqueId).val() == "lrn_cls_dty_ilt"){
					var widtharray= new Array(400,330,120,75,80,185,50,50);
					colNames=[  Drupal.t('LBL036'),Drupal.t('LBL295'),Drupal.t('LBL648'),Drupal.t('LBL649'),Drupal.t('LBL621'),Drupal.t('Instructor'),'',''];
					colModel=[
				          	   {name:'Type',index:'meeting_type_name', title: false, width:widtharray[0], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, resizable:false ,hidden : TypeHidden },
					           {name:'Title',index:'title', title: false, width:widtharray[1], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false  },
					           {name:'StartsOn',index:'start_date', title: false, width:widtharray[2], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false  },
					           {name:'From',index:'start_time', title: false, width:widtharray[3], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false  },
					           {name:'To',index:'end_time', title: false, width:widtharray[4], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false  },
					           {name:'Instructor',index:'full_name', title: false, width:widtharray[5], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false, hidden : InstructorHidden  },
					           {name:'Edit',index:'Edit', title: false, width:widtharray[6], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, resizable:false },
					           {name:'Action',index:'Remove', title: false, width:widtharray[7], 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: false, sortable: false, resizable:false }];

				}else{
					var widtharray= new Array(250,250,150,90,110,185,50,50,20);
					colNames=[  Drupal.t('LBL036'),Drupal.t('LBL295'),Drupal.t('LBL648'),Drupal.t('LBL649'),Drupal.t('LBL621'),Drupal.t('Instructor'),'','',''];
					colModel=[
				          	   {name:'Type',index:'meeting_type_name', title: false, width:widtharray[0], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false ,hidden : TypeHidden },
					           {name:'Title',index:'title', title: false, width:widtharray[1], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false  },
					           {name:'StartsOn',index:'start_date', title: false, width:widtharray[2], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false  },
					           {name:'From',index:'start_time', title: false, width:widtharray[3], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false  },
					           {name:'To',index:'end_time', title: false, width:widtharray[4], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, resizable:false  },
					           {name:'Instructor',index:'full_name', title: false, width:widtharray[5], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, resizable:false, hidden : InstructorHidden  },
					           {name:'More',index:'More', title: false, width:widtharray[6], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, resizable:false ,hidden : TypeHidden },
					           {name:'Edit',index:'Edit', title: false, width:widtharray[7], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, resizable:false },
					           {name:'Action',index:'Remove', title: false, width:widtharray[8], 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: false, sortable: false, resizable:false }];

				}
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					colNames: colNames,
					colModel: colModel,
					rowNum:12,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortname: "start_date",
					sortorder: "asc",
					toppager: true,
					botpager: false,
					height: "auto",
					width: 610,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext:"",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				setQtipPosition();
			}else if(this.type == 'trainings'){
				$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
				$('.catalog-course-basic-addedit-form-container').css('width','625px');
				var searchType = $('#search_all_trainings_type-hidden').val();

				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId),
					datatype: "json",
					mtype: 'GET',
					postData: {searhType : searchType},
					colNames:[ Drupal.t('LBL096'), Drupal.t('LBL083'), '', multiselectCheckbox],
					colModel:[ {name:'Code',index:'TrainingCode', title: false, width:100, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, resizable:false},
					           {name:'Title',index:'TrainingTitle', title: false, width:400, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, resizable:false  },
					           {name:'Action',index:'Remove', title: false, width:20, 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: deleteOption, sortable: false, resizable:false },
					           {name:'MultiselectCheck',index:'MultiselectCheck', title: false, width:17, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, hidden: multiselectOption, resizable:false}],
					rowNum:10,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+this.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: "auto",
					width: 610,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext : "",
					loadui:false,
					onSortCol:function (){
						$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:obj.callbackDataGrid
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});

			}
			 else if(this.type == 'ReportSchedulesList') {
				   var col1Width = 120, col2Width = 178, col3Width = 160, col4Width = 110;
				   	$("#datagrid-container-"+this.uniqueId).jqGrid({
						url: this.constructUrl("administration/report-schedule/datagrid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType),
						datatype: "json",
						mtype: 'GET',
						postData: {searhType : searchType},
						colNames:[  Drupal.t('LBL1207'), Drupal.t('LBL1239'), Drupal.t('LBL1224'), Drupal.t('LBL108')],
						//colNames:[  'Schedule','Next run', 'Last Run', 'Action'],
						colModel:[ { name:'Schedule',index:'Schedule', title: false, width:col1Width, 'widgetObj':objStr, formatter: obj.displayGridValues,sortable: true, resizable:false },
						           { name:'Next Run',index:'NextRun', title: false, width:col2Width, 'widgetObj':objStr, formatter: obj.displayGridValues,sortable: false, resizable:false},
						           { name:'Last Run',index:'LastRun', title: false, width:col3Width, 'widgetObj':objStr, formatter: obj.displayGridValues,sortable: false, resizable:false},
						           { name:'Action',index:'Action', title: false, width:col4Width, 'widgetObj':objStr, formatter: obj.displayGridValues,sortable: false, resizable:false},

						         ],

						rowNum:8,
						rowList:[8],
						pager: '#pager-datagrid-'+this.uniqueId,
						viewrecords:  true,
						multiselect: false,
						emptyrecords: "",
						sortorder: "desc",
						toppager: true,
						botpager: false,
						height: 'auto',
						width: 550,
						loadtext: "",
						recordtext: "",
						pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
						//pgtext : "",
						loadui:false,
						loadComplete:obj.callbackDataGrid

					}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:false 	});
				 }
			    else{

				var gridDispId = gridObj.gridDispId;

				$("#"+gridDispId).jqGrid({
					url:this.constructUrl(gridObj.url),
					datatype: "json",
					mtype: 'GET',
					colNames : gridObj.colNames,
					colModel : gridObj.colModel,
					rowNum:10,
					rowList:[10,25,50],
					pager: gridObj.pager,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortorder: "desc",
					toppager: true,
					botpager: false,
					height: 'auto',
					width: gridObj.width,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					loadui:false,
					loadComplete:obj.callbackDataGrid
				}).navGrid(gridObj.pager,{add:false,edit:false,del:false,search:false,refreshtitle:true});

			}
			}catch(e){
				// to do
			}
		},

		storeEnrollmentValues : function(entityId, entityType, uniqueId){
		 try{
			var selectedId = '';
			var existingValList = '';
			var selectedStatusValue = '';
			var selectedCompletedDate = '';
			var selectedScoreValue = '';
			var attachEnrollValue = '';

			var entityList = entityId+'-'+entityType;

			var currentSelectedValues = '';
			var selectedCompletedDate = '';
			var currentListIdsArray = new Array();
			var c = 0;
			// Collect all the selected Ids Score and Sequence
			$('#datagrid-container-'+uniqueId+' .multiselect-singlecheck').each(function(){
				selectedId = $(this).val();
				currentListIdsArray[c] = selectedId;
				if($(this).attr('checked') == true){
					//select text from exempted markup or plain html
					if($('#enrolled_selected_'+selectedId+'-'+entityList).find('.enrolled-exempted-status .enrolled-exempted-status-link').length > 0) {
						selectedStatusValue = $('#enrolled_selected_'+selectedId+'-'+entityList).find('.enrolled-exempted-status .enrolled-exempted-status-link').text();
					} else {
						selectedStatusValue = $('#enrolled_selected_'+selectedId+'-'+entityList).html();
					}
					if(selectedStatusValue != undefined){
						selectedCompletedDate = $('#completion_sel_date_'+selectedId+'-'+entityList).val();
						scoreVal = $('#score_'+selectedId+'-'+entityList).val();
						if(scoreVal == 'undefined') {
						  selectedScoreValue = '';
						}
						else {
						  selectedScoreValue = scoreVal;
						}						
						attachEnrollValue = selectedId+'##'+selectedStatusValue+'##'+selectedCompletedDate+'##'+selectedScoreValue;
						if(currentSelectedValues == '' ){
							currentSelectedValues = attachEnrollValue;
						} else {
							currentSelectedValues = currentSelectedValues + ',' +attachEnrollValue;
						}
					}
				}
				c++;
			});
			// Collect all the existing Ids except the current selected Ids
			var existingList = $('input[name="hidden_valuelist_'+uniqueId+'"]').val();
			if(existingList != ''){
				var existingListArray = existingList.split(',');
				var selectedValDetailsArr = new Array();
				var existingListDetails = '';
				selectedId = '';
				selectedStatusValue = '';

				for(var i=0; i< existingListArray.length; i++){
					existingListDetails = existingListArray[i];
					selectedValDetailsArr = existingListDetails.split('##');
					selectedId 				= selectedValDetailsArr[0];
					selectedStatusValue 	= selectedValDetailsArr[1];
					selectedCompletedDate 	= selectedValDetailsArr[2];
					selectedScoreValue 		= selectedValDetailsArr[3];
					if(selectedScoreValue == 'undefined') {
					  selectedScoreValue = '';
					}
                    else {
					  selectedScoreValue = selectedValDetailsArr[3];
					}					
					if($.inArray(selectedId, currentListIdsArray) == -1){
						attachEnrollValue = selectedId+'##'+selectedStatusValue+'##'+selectedCompletedDate+'##'+selectedScoreValue;
						if(currentSelectedValues == ''){
							currentSelectedValues = attachEnrollValue;
						} else {
							currentSelectedValues = currentSelectedValues + ',' +attachEnrollValue;
						}
					}
				}
			}
			// Append the selected Ids and existing Ids
			var allValues =  '';
			if(existingValList != '' && currentSelectedValues != ''){
				allValues = currentSelectedValues + ',' +existingValList;
			} else if(currentSelectedValues != '') {
				allValues = currentSelectedValues;
			} else if(existingValList != '') {
				allValues = existingValList;
			}
			$('input[name="hidden_valuelist_'+uniqueId+'"]').val(allValues);
		 }catch(e){
				// to do
		  }
		},

		retainEnrollmentValues : function(entityId, entityType, uniqueId){
			try{
			var existingList = $('input[name="hidden_valuelist_'+uniqueId+'"]').val();
			if(existingList != '' && existingList != undefined){
				var existingListArray = existingList.split(',');
				var existingListLen = existingListArray.length;
				var entityList = entityId+'-'+entityType;
				var uniqueId2 = '';
				var defaultStatus = '';
				var selectedId = '';
				for( var i = 0; i < existingListLen; i++) {
					existingListDetails = existingListArray[i];
					selectedValDetailsArr = existingListDetails.split('##');
					selectedId 				= selectedValDetailsArr[0];
					selectedStatusValue 	= selectedValDetailsArr[1];
					selectedCompletedDate 	= selectedValDetailsArr[2];
					selectedScoreValue 		= selectedValDetailsArr[3];
					if(selectedScoreValue == 'undefined') {
					  selectedScoreValue = '';
					}
					else {
					  selectedScoreValue 		=  selectedValDetailsArr[3];
					}					
					uniqueId2 = selectedId+'-'+entityList;

					if($('#enrolled_selected-'+uniqueId2).html() != 'undefined'){
						$('#enrolled_selected_'+uniqueId2).html(selectedStatusValue);
						$('#score_'+uniqueId2).val(selectedScoreValue);
						defaultStatus = $('#enrolled_default_status_'+uniqueId2).val();
						if(selectedStatusValue == 'Completed'){

							if(defaultStatus != 'Completed'){
								$('#default_status_date_'+uniqueId2).hide();
								$('#completion_sel_date_'+uniqueId2).show();
								$('#completion_sel_date_'+uniqueId2).datepicker({ defaultDate: 1, dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true,buttonText: Drupal.t("LBL218")});
							}
							$('#score_'+uniqueId2).removeAttr('disabled');
							$('#score_'+uniqueId2).css('backgroundColor','');

							$('#completion_sel_date_'+uniqueId2).val(selectedCompletedDate);
						} else {
							$('#default_status_date_'+uniqueId2).show();
							$('#completion_sel_date_'+uniqueId2).hide();
							$('#completion_sel_date_'+uniqueId2).datepicker( "destroy" );
							$('#score_'+uniqueId2).attr('disabled', 'disabled');
							$('#score_'+uniqueId2).css('backgroundColor','#CCCCCC');
						}
					}
				}
			}
			}catch(e){
				// to do
			}
		},

		countryAction : function(countryCode,countryId){
			try{
			var obj = this;
			$('body').data('mulitselectdatagrid').createLoader('admin-commerce-country-setting-plan');
			url = obj.constructUrl("administration/commerce/setting/country/action/"+countryCode+"/"+countryId);
			$.ajax({
				type: "POST",
				url: url,
				data:  '',
				success: function(result){
          result = $.trim(result);
          result = result.split('|');
            $("#"+countryCode+"-"+countryId).html(result[1]);
            $("body").data("mulitselectdatagrid").destroyLoader("admin-commerce-country-setting-plan");
          }

		    });
			}catch(e){
				// to do
			}
		},

		storeSurveyAssessmentValues : function(entityType, uniqueId){
			try{
			var selectedId = '';
			var existingValList = '';
			var selectedSequenceValue = '';
			var selectedScoreValue = '';
			var attachQuestionValue = '';

			var currentSelectedValues = '';
			var currentListIdsArray = new Array();
			var c = 0;
			// Collect all the selected Ids Score and Sequence
			$('#datagrid-container-'+uniqueId+' .multiselect-singlecheck').each(function(){
				selectedId = $(this).val();
				currentListIdsArray[c] = selectedId;
				if($(this).attr('checked') == true){
					selectedSequenceValue = $('#attachQuestion-sequenceNo-'+selectedId).val();
					selectedMandatoryValue = $('#attachQuestion-mandatoryOptional-'+selectedId).val();
					if(entityType == 'sry_det_typ_ass'){
						selectedScoreValue = $('#attachQuestion-score-'+selectedId).val();
					}
					if(selectedSequenceValue != undefined){

						if(entityType == 'sry_det_typ_ass'){
							attachQuestionValue = selectedId+'##'+selectedSequenceValue+'##'+selectedMandatoryValue+'##'+selectedScoreValue;
						} else {
							attachQuestionValue = selectedId+'##'+selectedSequenceValue+'##'+selectedMandatoryValue;
						}

						if(currentSelectedValues == ''){
							currentSelectedValues = attachQuestionValue;
						} else {
							currentSelectedValues = currentSelectedValues + ',' +attachQuestionValue;
						}
					}
				}
				c++;
			});
			// Collect all the existing Ids except the current selected Ids
			var existingList = $('input[name="hidden_valuelist_'+uniqueId+'"]').val();
			if(existingList != ''){
				var existingListArray = existingList.split(',');
				var selectedValDetailsArr = new Array();
				var existingListDetails = '';
				selectedId = '';
				selectedSequenceValue = '';
				selectedScoreValue = '';
				for(var i=0; i< existingListArray.length; i++){
					existingListDetails = existingListArray[i];
					selectedValDetailsArr = existingListDetails.split('##');
					selectedId 				= selectedValDetailsArr[0];
					selectedSequenceValue 	= selectedValDetailsArr[1];
					selectedMandatoryValue 	= selectedValDetailsArr[2];
					if(entityType == 'sry_det_typ_ass'){
						selectedScoreValue 	   	= selectedValDetailsArr[3];
					}
					if($.inArray(selectedId, currentListIdsArray) == -1){
						if(entityType == 'sry_det_typ_ass'){
							attachQuestionValue = selectedId+'##'+selectedSequenceValue+'##'+selectedMandatoryValue+'##'+selectedScoreValue;
						} else {
							attachQuestionValue = selectedId+'##'+selectedSequenceValue+'##'+selectedMandatoryValue;
						}

						if(existingValList == ''){
							existingValList = attachQuestionValue;
						} else {
							existingValList = existingValList + ',' +attachQuestionValue;
						}
					}
				}
			}
			// Append the selected Ids and existing Ids
			var allValues =  '';
			if(existingValList != '' && currentSelectedValues != ''){
				allValues = currentSelectedValues + ',' +existingValList;
			} else if(currentSelectedValues != '') {
				allValues = currentSelectedValues;
			} else if(existingValList != '') {
				allValues = existingValList;
			}
			$('input[name="hidden_valuelist_'+uniqueId+'"]').val(allValues);
			}catch(e){
				// to do
			}
		},

		retainSurveyAssessmentValues : function(entityType, uniqueId){
			try{
			var existingList = $('input[name="hidden_valuelist_'+uniqueId+'"]').val();
			if(existingList != ''){
				var existingListArray = existingList.split(',');
				var existingListLen = existingListArray.length;
				var selectedId = '';
				for( var i = 0; i < existingListLen; i++) {
					existingListDetails = existingListArray[i];
					selectedValDetailsArr = existingListDetails.split('##');
					selectedId 				= selectedValDetailsArr[0];
					selectedSequenceValue 	= selectedValDetailsArr[1];
					selectedMandatoryValue 	= selectedValDetailsArr[2];
					if(entityType == 'sry_det_typ_ass'){
						selectedScoreValue 	   	= selectedValDetailsArr[3];
					}
					if($('#attachQuestion-sequenceNo-'+selectedId).val() != 'undefined'){
						$('#attachQuestion-sequenceNo-'+selectedId).val(selectedSequenceValue);
						$('#attachQuestion-mandatoryOptional-'+selectedId+' option[value="'+selectedMandatoryValue+'"]').attr('selected', 'selected');
						if(entityType == 'sry_det_typ_ass'){
							$('#attachQuestion-score-'+selectedId).val(selectedScoreValue);
						}
					}
				}
			}
			}catch(e){
				// to do
			}
		},

		callbackDataGrid : function(data){
			try{
			vtip();
			$('#attachQuestion-mandatoryOptional-1 option[value="1"]').attr('selected', 'selected');
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			var nAgt = navigator.userAgent;
			var verOffset;
			var type = $('body').data('mulitselectdatagrid').type;
			var mode = $('body').data('mulitselectdatagrid').mode;
			var entityId = $('body').data('mulitselectdatagrid').entityId;
			var uniqueId = data.unique_id;
			if(type == 'enrolltprecertify'){
				$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
			}
			
			if(type == 'enrolluser' || type == 'enrolltpuser' || type =='shareuser'){
				var enrolluserValue = $('#enrolluser-autocomplete').val();
				var enrolltpuserValue = $('#enrolltpuser-autocomplete').val();
				if(type == 'enrolluser' && (enrolluserValue != Drupal.t('LBL181') && enrolluserValue != Drupal.t('LBL036') +' '+ Drupal.t('LBL107')
						&& enrolluserValue != Drupal.t('LBL036') +' '+Drupal.t('LBL137')
						&& enrolluserValue != Drupal.t('LBL036') +' '+Drupal.t('LBL133')+' '+ Drupal.t('LBL107')
						&& enrolluserValue != Drupal.t('LBL036') +' '+Drupal.t('LBL134')
						&& enrolluserValue != Drupal.t('LBL036') +' '+ Drupal.t('LBL102')
						&& enrolluserValue != Drupal.t('LBL036') +' '+ Drupal.t('LBL3060')
						&& enrolluserValue != Drupal.t('LBL036') +' '+ Drupal.t('LBL173')
						&& enrolluserValue != Drupal.t('LBL036') +' '+ Drupal.t('LBL054')
						&& enrolluserValue != Drupal.t('LBL036') +' '+ Drupal.t('Organization')
						&& enrolluserValue != Drupal.t('LBL036') +' '+ Drupal.t('LBL133')
						&& enrolluserValue != Drupal.t('LBL036') +' '+ Drupal.t('Manager')
						&& enrolluserValue != Drupal.t('LBL036') +' '+ Drupal.t('Group')
						&& enrolluserValue != Drupal.t('LBL1270'))) {
					$('#enrolluser-autocomplete').removeClass('input-field-grey');
				}
				if(type == 'enrolltpuser' && (enrolltpuserValue != Drupal.t('LBL181') && enrolltpuserValue != Drupal.t('LBL036') +' '+ Drupal.t('LBL107')
						&& enrolltpuserValue != Drupal.t('LBL036') +' '+Drupal.t('LBL137')
						&& enrolltpuserValue != Drupal.t('LBL036') +' '+Drupal.t('LBL133')+' '+ Drupal.t('LBL107')
						&& enrolltpuserValue != Drupal.t('LBL036') +' '+Drupal.t('LBL134')
						&& enrolltpuserValue != Drupal.t('LBL036') +' '+ Drupal.t('LBL102')
						&& enrolltpuserValue != Drupal.t('LBL036') +' '+ Drupal.t('LBL3060')
						&& enrolltpuserValue != Drupal.t('LBL036') +' '+ Drupal.t('LBL173')
						&& enrolltpuserValue != Drupal.t('LBL036') +' '+ Drupal.t('LBL054')
						&& enrolltpuserValue != Drupal.t('LBL036') +' '+ Drupal.t('Organization')
						&& enrolltpuserValue != Drupal.t('LBL036') +' '+ Drupal.t('LBL133')
						&& enrolltpuserValue != Drupal.t('LBL036') +' '+ Drupal.t('Manager')
						&& enrolltpuserValue != Drupal.t('LBL036') +' '+ Drupal.t('Group')
						&& enrolltpuserValue != Drupal.t('LBL1270'))) {
					$('#enrolltpuser-autocomplete').removeClass('input-field-grey');
				}
				if(data.rows != undefined){
					var statisticsCount = data.rows[0].statistic_count;
					$('#statistics-count-'+uniqueId).html(statisticsCount);
				}
				$('body').data('mulitselectdatagrid').retainEnrollmentValues($('body').data('mulitselectdatagrid').entityId, $('body').data('mulitselectdatagrid').entityType, uniqueId);
				if ((type == 'shareuser' || type == 'enrolluser') && (verOffset=nAgt.indexOf("Chrome"))!=-1 || type == 'enrolltpuser' && (verOffset=nAgt.indexOf("Chrome"))!=-1) {
					if(mode == 'view'){
			 		$('#datagrid-container-'+uniqueId+'_MultiselectCheck').find('.multiselect-selectall').css("margin-left","0px");
					}
				}
				//addclass for date soting
				if(mode=="view"){
					$('#jqgh_datagrid-container-'+uniqueId+'_Date').addClass("enroll-date-sortalign");
				//	$("#datagrid-container-"+uniqueId+"_toppager .ui-pg-table").css('top','-2px')
					// removes hand symbol for non sortable column
					$('#jqgh_datagrid-container-'+uniqueId+'_RegFrom').removeClass('ui-jqgrid-sortable');
				}
				checkPaginationWidth();
			}
			if(type == 'addkeywords'){
				$('#admin-add-course-training-plan').find('.ui-jqgrid-bdiv').addClass("add-key-word-list");
				$('#datagrid-container-keywords-'+uniqueId+' tr:even').css("background", "#f7f7f7");
				$('#datagrid-container-keywords-'+uniqueId+' tr.jqgfirstrow').css("background", "none");
				$('#admin-add-course-training-plan .add-key-word-list').jScrollPane({});
			}
			var orgUsersId = uniqueId.split('-');
			if(data.type == 'orgusers'){
				$('#datagrid-div-'+orgUsersId[0]+'-orgusers').css('display','block');
				var popupId1 = 'org-users-main-div-'+orgUsersId[0];
				$("#"+popupId1).find("#bubble-face-table").css("top","39px").css("z-index","100");
				$("#"+popupId1+" .qtip-close-button").css("top","39px").css("z-index","101");
				$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
				$('#datagrid-container-'+uniqueId+' tr.jqgfirstrow').css("background", "none");
				$("#datagrid-container-"+uniqueId+"_toppager").css("width","296px");
				var orgGridWidth = $('body').data('mulitselectdatagrid').orgGridWidth;
//			    var offsetWidth = $('#org-users-main-div-'+entityId).find('#bubble-face-table td.bubble-cl').width() * 2;
			    var orgQtipWidth = orgGridWidth + 40;
			    $('#org-users-main-div-'+entityId).find('#bubble-face-table').width(orgQtipWidth);
			}


			if(type == 'attachQuestion') {
				var selectedIds 	= data.selectedIds;
				// datagrid-div-attachQuestion-7-sry_det_typ_ass
				var container_div 	= 'jqgh_datagrid-container';
				var paint_div		= 'datagrid-div-'+uniqueId;

				/*if(data.pg_mode == 'addmore') {
					$("#"+paint_div).css('padding-left','12px');
				}*/

				if($("#"+container_div+"-"+uniqueId+'_GroupName') && (document.getElementById("mandat_GroupName") == null)) {
					$("#"+container_div+"-"+uniqueId+'_GroupName').append('<span id="mandat_GroupName" class="addedit-mandatory">*</span>');
				}

				if($("#"+container_div+"-"+uniqueId+'_SequenceNo') && (document.getElementById("mandat_SequenceNo") == null)) {
					$("#jqgh_datagrid-container-"+uniqueId+'_SequenceNo').append('<span id="mandat_SequenceNo" class="addedit-mandatory">*</span>');
				}

				if($("#"+container_div+"-"+uniqueId+'_Score') && (document.getElementById("mandat_Score") == null)) {
					$("#jqgh_datagrid-container-"+uniqueId+'_Score').append('<span id="mandat_Score" class="addedit-mandatory">*</span>');
				}

				if(selectedIds) {
					var selectedId = selectedIds.split(",");

					$('input[name="hidden_idlist_'+uniqueId+'"]').val(selectedIds);
					var n = 0;
					for(n=0;n<selectedId.length;n++){
						 // jqg_datagrid-container-attachQuestion-1-sry_det_typ_ass_2
						// attachQuestion-1-sry_det_typ_ass
						var ckboxId = 'jqg_datagrid-container-'+uniqueId+'_'+selectedId[n];
						$("#"+ckboxId).attr("checked",true);
					}
				}

				// To retain values of Score, Sequence and Mandatory for Survey and Assessment
				var entityType = $('input[name="hidden_entity_type_'+uniqueId+'"]').val();
				$('body').data('mulitselectdatagrid').retainSurveyAssessmentValues(entityType, uniqueId);

			}

            
			$('body').data('mulitselectdatagrid').retainCheckboxSelected(uniqueId);

			if(type == 'assessment'){ // Disabling the multicheckbox selection for assessment
				$('#multiselect-selectall-'+uniqueId).hide();
				$('#datagrid-container-'+uniqueId+' .checkbox-unselected').removeClass('checkbox-unselected');
				if(mode == 'edit')
					$('#jqgh_datagrid-container-'+uniqueId+'_ViewOption').removeClass('ui-jqgrid-sortable');
			}
			if(type == 'survey' && mode == 'edit') {
				$('#jqgh_datagrid-container-'+uniqueId+'_ViewOption').removeClass('ui-jqgrid-sortable');
			}
			if ((type == 'shareuser' || type == 'enrolluser') && mode == 'edit' && this.currTheme != 'expertusoneV2' && $.browser.msie && parseInt($.browser.version, 10)=='8') {
				$('#jqgh_datagrid-container-'+uniqueId+'_MultiselectCheck').find('.multiselect-selectall').css("padding-left","2px");
			}
			/*if(type =='enrolluser'&& mode == 'edit' && this.currTheme == 'expertusoneV2'){
			console.log("enrolluser"+" Mode:"+ mode);
				$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(1)').css("width","112px");
			}*/
			if(type =='content' && mode == 'view'){
				sequenceDragAndDrop('datagrid-container-'+uniqueId, 'attach_class_content');

			}else if(type =='content' && mode == 'edit') {
			}
			if(type =='content' &&  Drupal.settings.ajaxPageState.theme == "expertusoneV2") {

			}
			if(Drupal.settings.ajaxPageState.theme != "expertusoneV2"){
				if(type =='content' &&  mode == 'view' && $.browser.msie && parseInt($.browser.version, 10)=='8') {}
			}
				if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
				     if (type == 'assessment' &&  mode == 'view' && $.browser.msie && parseInt($.browser.version, 10)=='8') {
				    	 $('#gview_datagrid-container-'+uniqueId+' table th:nth-child(5)').css('width','113px');
				    	 $('#gview_datagrid-container-'+uniqueId+' table td:first-child').css('width','158px');
						 $('#gview_datagrid-container-'+uniqueId+' table td:nth-child(3)').css('width','80px');
				     }
				}
				if((type=='RoleDisplayUsers' || type=='RoleDisplayOwners') && mode== 'viewscreen'){
				 if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2' && navigator.userAgent.indexOf("Chrome")>0){
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-bdiv tr td:eq(0)').css('width','211px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-bdiv tr td:eq(1)').css('width','150px');
				  }
				}
				if (type == 'assessment' && navigator.userAgent.indexOf("Chrome")>0 || type == 'assessment' && navigator.userAgent.indexOf('Safari')>0 ){
					if (this.currTheme == 'expertusoneV2'){
					   if(mode == 'edit'){
						   $('#gview_datagrid-container-'+uniqueId+' table th:first-child').css('width','277px');
						   $('#gview_datagrid-container-'+uniqueId+' table th:nth-child(2)').css('width','276px');
						   $('#gview_datagrid-container-'+uniqueId+' table th:last-child').css('width','55px');
					   }else if(mode == 'view'){
						   console.log("assessment view");
						  /* $('#gview_datagrid-container-'+uniqueId+' table th:first-child').css('width','174px');
						   $('#gview_datagrid-container-'+uniqueId+' table th:nth-child(2)').css('width','98px');
						   $('#gview_datagrid-container-'+uniqueId+' table th:nth-child(3)').css('width','115px');
						   $('#gview_datagrid-container-'+uniqueId+' table th:nth-child(4)').css('width','110px');
						   $('#gview_datagrid-container-'+uniqueId+' table th:nth-child(5)').css('width','113px');
						   $('#gview_datagrid-container-'+uniqueId+' table th:nth-child(6)').css('width','27px');
						   $('#gview_datagrid-container-'+uniqueId+' table th:nth-child(7)').css('width','20px');*/
						   					   

						 /*  $('#gview_datagrid-container-'+uniqueId+' table td:first-child').css('width','174px');
						   $('#gview_datagrid-container-'+uniqueId+' table td:nth-child(2)').css('width','98px');
						   $('#gview_datagrid-container-'+uniqueId+' table td:nth-child(3)').css('width','115px');
						   /*$('#gview_datagrid-container-'+uniqueId+' table td:nth-child(3) span').css('width','75px');*/
						 /*  $('#gview_datagrid-container-'+uniqueId+' table td:nth-child(4)').css('width','110px');
						   $('#gview_datagrid-container-'+uniqueId+' table td:nth-child(5)').css('width','113px');
						   $('#gview_datagrid-container-'+uniqueId+' table td:nth-child(6)').css('width','27px');
						   $('#gview_datagrid-container-'+uniqueId+' table td:nth-child(7)').css('width','20px');*/
						   
						   $('#gview_datagrid-container-'+uniqueId+' table td:nth-child(4)').css('padding-left','10px');
						   $('#gview_datagrid-container-'+uniqueId+' table td:nth-child(5)').css('padding-left','10px');
					  }else{
						  console.log("assessment else");
						   $('#gview_datagrid-container-'+uniqueId+' table th:first-child').css('width','223px');
						   $('#gview_datagrid-container-'+uniqueId+' table th:nth-child(2)').css('width','80px');
						   $('#gview_datagrid-container-'+uniqueId+' table th:nth-child(3)').css('width','106px');
						   $('#gview_datagrid-container-'+uniqueId+' table th:nth-child(4)').css('width','93px');
						   $('#gview_datagrid-container-'+uniqueId+' table th:nth-child(5)').css('width','102px');
					  }
					}

				}

				if (type == 'survey' && navigator.userAgent.indexOf("Chrome")>0){
					//console.log('survey view');
					if (this.currTheme == 'expertusone' && mode == 'view'){
						//console.log('survey view');
						$('#gview_datagrid-container-'+uniqueId+' table th:first-child').css('width','331px');
					}
					if (this.currTheme == 'expertusoneV2' && mode == 'edit'){
						//console('survey view tp vadivel');
						//$('#program-tp-basic-addedit-form-container').find('#admin-data-grid .ui-jqgrid-bdiv .ui-jqgrid-btable tr td:first-child').css('width','411px');
					}

					if (this.currTheme == 'expertusoneV2' && mode == 'view'){
						$(".admin-datagrid-pagination.survey-attach-grid-wrapper").find(".ui-jqgrid-hdiv .ui-jqgrid-hbox .ui-jqgrid-labels th").css("padding","0 4px 0 0");
					}

				}

			if(type =='content' &&  Drupal.settings.ajaxPageState.theme == "expertusone") {
				if(navigator.userAgent.indexOf("Chrome")>0){
				$('#gview_datagrid-container-'+uniqueId+' table td:last-child').css('padding-left','8px');
				}
			}
			if(type == 'session'){
				$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
				$('#datagrid-container-'+uniqueId+' tr td').css({'overflow':'visible','line-height':'33px'});
				$('#datagrid-container-'+uniqueId+' tr td:last-child').css('padding-left','0');
			}
			if(type == 'ReportSchedulesList'){
			  $('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
			  if(this.currTheme == 'expertusoneV2'){
				  $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-bdiv table').css('border-left', '1px solid #e5e5e5');
	              $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-bdiv table').css('border-right', '1px solid #e5e5e5');
	              $('#gview_datagrid-container-' + uniqueId + ' .ui-jqgrid-bdiv table').css('border-bottom', '1px solid #e5e5e5');
			  }
			  
			}  
			if(type == 'addProductOrder'){
				$("#gview_datagrid-container-"+uniqueId).css('top','-31px');
				$("#gview_datagrid-container-"+uniqueId).css('position','relative');
			    $('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
			    $("#datagrid-container-"+uniqueId).find("tr:first-child td:first-child").css('padding-left','2px');
			    $loaderId = (type == 'addUserOrder') ? 'gbox_datagrid-container-add-user-admin-order' : 'admin_product_addorder_container';
			    $('body').data('mulitselectdatagrid').destroyLoader($loaderId); // Added for destroy loader when click on sorting
			    if ($("#datagrid-container-"+uniqueId).getGridParam("records") <= 0) {
			    	$('#gview_datagrid-container-'+uniqueId+ ' .ui-jqgrid-bdiv').css('border-width','0');
			    }else{
			      	$('#gview_datagrid-container-'+uniqueId+ ' .ui-jqgrid-bdiv').css('border-width','0 1px 1px');
			    }
			   //multicurrency related work
				if(document.getElementById('edit-admin-currency-dropdown-select_titletext')){
					if($.cookie('admin_shop_cart_currency_code') !='' && $.cookie('admin_shop_cart_currency_code') !=null && $.cookie('admin_shop_cart_currency_code')!=undefined)
						$('#edit-admin-currency-dropdown-select_titletext span').html($.cookie('admin_shop_cart_currency_code')+' '+$.cookie('admin_shop_cart_currency_sym'));
				}
			}

			if ($("#datagrid-container-"+uniqueId).getGridParam("records") <= 0) {
				if(type == "enrolluser" || data.type == 'orgusers' || type == "enrolltpuser" || type == 'shareuser'){
					$(".enrolluser-class-grid-wrapper #exportcontainer").hide();
					$(".enrolltp-grid-wrapper #exportcontainer").hide();
					$(".exportcontainer").hide();
					$("#user-"+orgUsersId[0]+"-orgusers-search-box").css('display','none');
					$("#gview_datagrid-container-"+uniqueId).css('border','none');
				}
				var alreadyEnrolled = data.msg;
				if(alreadyEnrolled !=''){
					var Msg = alreadyEnrolled;
				}else{
					var Msg = Drupal.t('MSG403');
				}
				$('#datagrid-noresult-msg-'+uniqueId).show();
				$('#datagrid-noresult-msg-edit-'+uniqueId).show();
				$('#datagrid-add-search-button-'+uniqueId).hide();
				$('#sharelink_disp_div').hide();
				$(".norecords-msg-edit-page").remove();
				if(type == 'enrolluser' || type == 'enrolltpuser' || type == 'prerequisite' || type == 'equivalence'|| type=='survey' || type=='assessment'){
					$('#datagrid-div-'+uniqueId).hide();
				}
				 if( Msg == Drupal.t('MSG403') ) {
					$('.admin-datagrid-save-cancel-button-align-bottom').hide();
				 }
                                  if(type == 'shareuser'){
				      if( Msg == Drupal.t('MSG403') ) {
						$('.admin-datagrid-save-cancel-button-align-bottom').show();
						 }
				 }				if((type == 'shareuser' || type == 'enrolluser' || type == 'enrolltpuser' || type == 'CountrySetting'|| type=='RoleDisplayUsers' || type=='RoleDisplayOwners' || type=='grpAddOwners' || type=='grpAddUsers' || type == 'prerequisite' || type == 'equivalence'|| type=='survey' || type=='assessment'|| type == "trainings" || type == 'content') && mode == 'edit'){
					$('#statistics-count-'+uniqueId).html('');
					var norecordsmsg = '<div class="norecords-msg-edit-page">'+Msg+'</div>';
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").before(norecordsmsg);
					$("#datagrid-container-"+uniqueId+"_toppager").css('display','none');
					$("#datagrid-div-"+uniqueId).css('display','block');
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").css('display','none');
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").addClass("no-item-found-class");
					if((type == 'enrolluser' || type == 'shareuser') && ( $.browser.msie && parseInt($.browser.version, 10)=='8')){
						$("#gview_datagrid-container-"+uniqueId+" table th").css('padding-right','0px');
						//$("#gview_datagrid-container-"+uniqueId+" table th div").css('padding-left','5px');
					}
						if((type == 'enrolluser' || type == 'shareuser') && ($.browser.msie && $.browser.version == 8) && this.currTheme == "expertusoneV2"){
							$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(0)').css('width','132px');
						}
				}else if(type == 'session'){
					$('#catalog-class-basic-addedit-form-action-disp-'+entityId+' ul li:eq(1) input').click();
				}else if(type == 'trainings' && $('#search_all_trainings_type-hidden').val() == "any"){
					$('#discount-basic-addedit-action-disp ul li:eq(1) input').click();
				}else if(mode == 'view'  && type != "trainings"){
					if(type =='RoleDisplayUsers' || type =='RoleDisplayOwners' || type == 'shareuser'){ // Condition added for this ticket #0040915
						Msg = Drupal.t('MSG403');
						var norecordsmsg = '<div class="norecords-msg-edit-page">'+Msg+'</div>';
						$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").before(norecordsmsg);
						$("#datagrid-container-"+uniqueId+"_toppager").css('display','none');
						$("#datagrid-div-"+uniqueId).css('display','block');
						$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").css('display','none');
						$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").addClass("no-item-found-class");
						var skip = 'yes';
					}else{
						$('#datagrid-div-'+uniqueId).hide();
					}
				} else {
					if(type == 'ContentMoveUsers') {
						var norecordsmsg = '<div class="norecords-msg-edit-page">'+Drupal.t('MSG605')+'</div>';
					} else if(type == 'SurAssAttachQuestion'){
						var norecordsmsg = '<div class="norecords-msg-edit-page">'+data.msg+'</div>';
					} else {
						var norecordsmsg = '<div class="norecords-msg-edit-page">'+Drupal.t('MSG403')+'</div>';
					}
					if(type == "trainings"){
						$('#datagrid-add-search-button-'+uniqueId).show();
					}
					//$("#datagrid-container-"+uniqueId).html(norecordsmsg);
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").before(norecordsmsg);
					$("#datagrid-container-"+uniqueId+"_toppager").css('display','none');
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").css('display','none');
					if(type == 'ContentMoveUsers'){
						$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").css('visibility','hidden');
						$("#admin-move-users-content .transfer-users-grp").css('display','none');
					}
				}
			}else{
				if( Msg != Drupal.t('MSG403') ){
					$('.admin-datagrid-save-cancel-button-align-bottom').show();
				}
				if(type == "enrolluser" || data.type == 'orgusers'  || type == 'shareuser'){
					$(".enrolluser-class-grid-wrapper #exportcontainer").show();
					$(".exportcontainer").show();
					$("#user-"+orgUsersId[0]+"-orgusers-search-box").css('display','block');
					$(".filter-search-start-date-left-bg").css('display','block');
					$(".filter-search-start-date-right-bg").css('display','block');
					//$("#gview_datagrid-container-"+uniqueId).css('border','1px');
				}
				var tmpMode = mode!='view' ? 'edit' : 'view';
				if(tmpMode == 'edit' || type == 'enrolluser'  || type == 'shareuser' || type == 'enrolltpuser' || type =="trainings" || type == 'CountrySetting' || type=='RoleDisplayUsers' || type=='RoleDisplayOwners' || type=='grpAddUsers'|| type=='grpAddOwners' || type == 'prerequisite' || type == 'equivalence'|| type=='survey' || type=='assessment'){
					$(".norecords-msg-edit-page").remove();
					$("#datagrid-container-"+uniqueId+"_toppager").css('display','block');
					$("#admin-move-users-content .transfer-users-grp").css('display','block');
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").css('visibility','visible');
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").css('display','block');
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").removeClass("no-item-found-class");
					$("#datagrid-container-"+uniqueId+"_toppager_center").css('width','190px');
				}
				if(tmpMode == 'edit' || type == 'enrolltpuser' ) {
					if($.browser.msie && $.browser.version == 9 && this.currTheme != 'expertusoneV2'){
					$("#datagrid-container-"+uniqueId+"_toppager_center").css('width','170px');
				   }
				}
				if(type == 'CountrySetting'){
					if($.browser.msie && $.browser.version == 9 && this.currTheme != 'expertusoneV2'){
						$("#datagrid-container-"+uniqueId+"_toppager_center").css('width','190px');
					 }
				}
				if((type == 'enrolluser' || type == 'shareuser' ) && this.currTheme == 'expertusone' ){
				$("#datagrid-container-"+uniqueId+"_toppager_center").find(".ui-pg-table").css("right","14px");
				}
				if ((type == 'enrolluser' || type == 'shareuser' ) && $.browser.msie && parseInt($.browser.version, 10)=='8') {
				   // $('#gview_datagrid-container-'+uniqueId+' table th div').css('padding-left','3px');
				    $('#gview_datagrid-container-'+uniqueId+' table th:last-child div').css('padding-left','1px');
				    $('#gview_datagrid-container-'+uniqueId+' table th').css('padding-left','0px');
				}
				if(type == 'enrolltprecertify'){
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").css('display','block');
				}
			}
			//0049123
		    if((type == 'enrolluser' || type == 'shareuser' ) && this.currTheme == 'expertusoneV2' ){
				   if(mode == 'view'){
					   $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:first-child').css("width",'105px');
				   }
				   if(mode == 'view' && navigator.userAgent.indexOf("Chrome")>0 || mode == 'view' && navigator.userAgent.indexOf('Safari')>0){
					   $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:first-child').css("width",'113px');
			
				   }
				   if(mode == 'edit'){
					 //  $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:nth-child(2)').css("width",'133px');
				   }
			}

    		if(type == 'enrolluser' || type == 'enrolltpuser' || type == 'enrolltprecertify'){
    			//console.log("enrolltprecertify type 00:: "+ type);
    			var pagecnt = (type == 'enrolltprecertify') ? 5 : 10;
    			var enrType = (type == 'enrolluser') ? 'enrolluser' : 'enrolltpuser';
				if($("#datagrid-container-"+uniqueId).getGridParam("records") > pagecnt){
					$("#"+enrType+"-grid-wrapper-"+uniqueId+ " #exportcontainer").removeClass('exportcontainer');
					$("#"+enrType+"-grid-wrapper-"+uniqueId+ " #exportcontainer").addClass('exportcontainer-page');
					if(type == 'enrolltprecertify'){
					    if(document.getElementById('page-pipe-recertify') == null){
						 $("#"+enrType+"-grid-wrapper-"+uniqueId+ " #exportcontainer").append('<span id = "page-pipe-recertify" class="page-pipeline">|</span>');
						 if($("#admin-data-grid #program-tprecertify-basic-addedit-form-container .ui-pg-table .ui-pg-table").is(":visible")){
								  var pageConWidth=$("#admin-data-grid #program-tprecertify-basic-addedit-form-container .ui-pg-table .ui-pg-table").width();
								   $("#program-tprecertify-basic-addedit-form-container #exportcontainer").css("right",pageConWidth+15);
							}
						 }
					}else{
					    if(document.getElementById('page-pipe') == null)
					     $("#"+enrType+"-grid-wrapper-"+uniqueId+ " #exportcontainer").append('<span id = "page-pipe" class="page-pipeline">|</span>');
					}
						
					if(this.currTheme != 'expertusoneV2' && $.browser.msie && $.browser.version == 9 ){
						$("#"+enrType+"-grid-wrapper-"+uniqueId+ " #exportcontainer").css('right','193px');
					}
				}else{ // Remove the pipeline if the No Records in Enrollment #0039985
					$("#"+enrType+"-grid-wrapper-"+uniqueId+ " #exportcontainer").find("#page-pipe").remove();
					$("#"+enrType+"-grid-wrapper-"+uniqueId+ " #exportcontainer").find("#page-pipe-recertify").remove();
					$("#"+enrType+"-grid-wrapper-"+uniqueId+ " #exportcontainer").removeClass('exportcontainer-page');
				    $("#"+enrType+"-grid-wrapper-"+uniqueId+ " #exportcontainer").addClass('exportcontainer');
				}
			}

			if(data.type == 'orgusers'){
				if($("#datagrid-container-"+uniqueId).getGridParam("records") > 5){
					$("#datagrid-container-"+uniqueId+"_toppager_center .search-pagination-right").find("span:first-child").remove();
					$("#page-pipe").css("display","block");
					var dymPipWidth=$("#datagrid-container-"+uniqueId+"_toppager_center .ui-pg-table").width();
					//console.log(dymPipWidth);
					if(document.getElementById('page-pipe') == null)
						$("#datagrid-div-"+uniqueId+' #exportcontainer').after('<span id = "page-pipe" class="page-pipeline">|</span>');
					if(this.currTheme == 'expertusoneV2'){
						if($.browser.msie && $.browser.version == 9){
					     $('#page-pipe').css({'top':'27px','right':dymPipWidth+18});
						}else{
						 $('#page-pipe').css({'top':'25px','right':dymPipWidth+18});
						}

					}else{
					  if($.browser.msie && $.browser.version == 9){
					   $('#page-pipe').css({'top':'26px','right':dymPipWidth+18});
					      }else{
						   $('#page-pipe').css({'top':'26px','right':dymPipWidth+18});
					     }


					}

					if($.browser.msie && parseInt($.browser.version, 10)=='10' && this.currTheme == "expertusoneV2") {
						$("#gview_datagrid-container-"+uniqueId).css({"top":"-30px","position":"relative","border-width":"0"});
						$("#gview_datagrid-container-"+uniqueId).find(".ui-jqgrid-hdiv").css({"border-right":"1px solid #e7e7e7"});
						$("#datagrid-container-"+uniqueId).css("width",'551px');
						$("#datagrid-container-"+uniqueId+"_toppager").css('top','-10px');
					}else if($.browser.msie && parseInt($.browser.version, 10)=='10' && this.currTheme == "expertusone" || $.browser.msie && parseInt($.browser.version, 10)=='8' && this.currTheme == "expertusone") {
						$("#gview_datagrid-container-"+uniqueId).css({"top":"-30px","position":"relative"});
						$("#datagrid-container-"+uniqueId+"_toppager").css('top','-10px');
					}
				}else {$("#page-pipe").css("display","none");}
				 if($("#datagrid-container-"+uniqueId).getGridParam("records") <=0){
					$(".filter-search-start-date-left-bg").css('display','none');
					$(".filter-search-start-date-right-bg").css('display','none');
					}
			}
			if(type == 'ReportSchedulesList') {
				if($("#datagrid-container-"+uniqueId).getGridParam("records") > 8){
					$("#schedule-exportcontainer").removeClass('schedule-exportcontainer');
					$("#schedule-exportcontainer").addClass('schedule-exportcontainer-page');
					$("#datagrid-container-"+uniqueId+"_toppager_center .search-pagination-right").find("span:first-child").remove();
					if(document.getElementById('page-pipe') == null)
						$('#schedule-exportcontainer').after('<span id = "page-pipe" class="page-pipeline">|</span>');
				}
				$("#schedule-exportcontainer").css('display','block');
			}
			if(type == 'TPAttachCourse' || type == 'ContentMoveUsers') {
				listRows = 6;
			}else if(type == 'SurAssAttachQuestion'){
				listRows = 6;
			}else if(type == 'prerequisite' || type == 'trainings' || type == 'equivalence' || type=='enrolltpuser' || type=='survey' || type=='assessment' || type=='content' || type=='enrolluser'  || type == 'shareuser' ) {
				var lnrType = $('body').data('mulitselectdatagrid').entityType;
				listRows = (lnrType == 'cre_sys_obt_trp' ||  lnrType == 'cre_sys_obt_crt' || lnrType == 'cre_sys_obt_trn' || lnrType == 'cre_sys_obt_cur') ? 10 : 8;
				if((lnrType == 'cre_sys_obt_cls') && (type=='survey' || type=='content' || type=='enrolluser')){
				   listRows = crs_cls_listcount ;
				}
			}else if(type == 'addProductOrder'){
				listRows = 10;
			}else{
				listRows = 5;
			}
			if(mode=='edit'){
				$("#datagrid-div-"+uniqueId).find( '.ui-jqgrid-htable').find('th').children().css('margin-top','0px');
				if(type=='RoleDisplayUsers' || type=='RoleDisplayOwners' || type=='grpAddUsers' || type=='grpAddOwners'){
					if(type=='grpAddUsers' || type=='grpAddOwners' || type=='RoleDisplayOwners')
						$("#gview_datagrid-container-"+uniqueId).find( '.ui-jqgrid-hdiv').css('width','469px');
					else
						$("#gview_datagrid-container-"+uniqueId).find( '.ui-jqgrid-hdiv').css('width','470px');
					$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
					$('#datagrid-container-'+uniqueId+' tr.jqgfirstrow td').css("padding", "0 2px");
					$("#datagrid-container-"+uniqueId+"_toppager_center .search-pagination-right").find("span:first-child").remove();
					$("#datagrid-container-"+uniqueId+"_toppager").find('.ui-pg-table').css('width', 'auto');
				}
				if(type == "ReportSchedulesList"){
					listRows = 8;
					$("#gview_datagrid-container-"+uniqueId).css('margin-left','5px');
				}
			}else if(mode=='view'){

				if(type == "session"){
					listRows = 12;
					$("#gview_datagrid-container-"+uniqueId).find( '.ui-jqgrid-bdiv').css('margin-top','0px');
					$("#gview_datagrid-container-"+uniqueId).find( '.ui-jqgrid-bdiv').css('overflow','visible');
				}
				if(type == "session" && Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
					if($('#add-session-delivery-type'+uniqueId).val() == "lrn_cls_dty_ilt" && navigator.userAgent.indexOf("Chrome")>0){
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-htable tr th:eq(1)').css('width','215px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-htable tr th:eq(2)').css('width','89px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-bdiv tr td:eq(1)').css('width','222px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-bdiv tr td:eq(2)').css('width','89px');
					}
				}
				$("#gview_datagrid-container-"+uniqueId).addClass('datagrid-type-'+type);
				if(type=='RoleDisplayUsers' || type=='RoleDisplayOwners' || type=='grpAddUsers' || type=='grpAddOwners'){
					if(skip !='yes' || skip == undefined || skip==''){ // Condition added for this ticket #0040915
					  $("#gview_datagrid-container-"+uniqueId).find( '.ui-jqgrid-hdiv').css('display','block');
					}
					$("#gview_datagrid-container-"+uniqueId).find( '.ui-jqgrid-hdiv').css('width','470px');
					$("#datagrid-div-"+uniqueId).find( '.ui-jqgrid-htable').find('th').children().css('height','27px');
					$("#datagrid-div-"+uniqueId).find( '.ui-jqgrid-htable').find('th').children().css('text-align','left');
					$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
					$("#datagrid-container-"+uniqueId+"_toppager_center .search-pagination-right").find("span:first-child").remove();
					$("#datagrid-container-"+uniqueId+"_toppager").find('.ui-pg-table').css('width', 'auto');
				}
			}
			 if(type == "prerequisite" || type == 'equivalence' || type == 'enrolluser'  || type == 'shareuser'  || type == 'enrolltpuser' || type == 'trainings' ||  type=='survey' || type=='assessment' || type=='content' ) {
					if ( $.browser.msie && parseInt($.browser.version, 10)=='8' ||  $.browser.msie && parseInt($.browser.version, 10)=='9' || $.browser.msie && parseInt($.browser.version, 10)=='10') {
					 $("#gview_datagrid-container-"+uniqueId).find('.ui-jqgrid-bdiv').css('height','auto');
					}
				 }
			 if(type == "prerequisite" || type == 'equivalence'){
				 if ( $.browser.msie && parseInt($.browser.version, 10)=='8' ||  $.browser.msie && parseInt($.browser.version, 10)=='9' || $.browser.msie && parseInt($.browser.version, 10)=='10') {
					 $('#gview_datagrid-container-'+uniqueId).css('min-height','385px');
				 }
			 }
			if ($("#datagrid-container-"+uniqueId).getGridParam("records") > listRows) {
				$("#datagrid-container-"+uniqueId+"_toppager").css('visibility','visible');
				$("#datagrid-container-"+uniqueId+"_toppager").css('display','block');
				if(type == "prerequisite"){
					if ( $.browser.msie && parseInt($.browser.version, 10)=='9') {
						$("#datagrid-container-"+uniqueId+"_toppager .ui-pg-table").css('width','80px');
					}
				 }
				if(mode=='edit' && type != 'TPAttachCourse' && type != 'SurAssAttachQuestion' && type!='addkeywords' && type!= 'CountrySetting' && type!='grpAddUsers' && type!='grpAddOwners' && type!='trainings' && type!='addUserOrder' && type!='addProductOrder' && type!='ReportSchedulesList'){
					$("#gview_datagrid-container-"+uniqueId).css('top','-31px');
					$("#gview_datagrid-container-"+uniqueId).css('position','relative');
				} else if(mode=='view' && type=="enrolluser" || type=="enrolltpuser"  || type == 'shareuser' ) {
					$("#gview_datagrid-container-"+uniqueId).css('top','-31px');
					$("#gview_datagrid-container-"+uniqueId).css('position','relative');
				}if(mode=='view' && type == "session"){
					$("#datagrid-container-"+uniqueId+"_toppager").css('position','absolute');
					this.currTheme = Drupal.settings.ajaxPageState.theme;
					if(this.currTheme!='expertusoneV2')
						$("#gview_datagrid-container-"+uniqueId).find( '.ui-jqgrid-hdiv').css('width','554px');
				}
				//Discount screen ui Fixes
				if(type == "trainings"){
					if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
					$("#gview_datagrid-container-"+uniqueId).find( '.ui-jqgrid-toppager').css({"position":"absolute","width":"612px","top":"8px"});
					}else{
                  	$("#gview_datagrid-container-"+uniqueId).find( '.ui-jqgrid-toppager').css({"position":"absolute","width":"612px","top":"0"});
				    }
				}
				if(type == "session" && Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
					$("#gview_datagrid-container-"+uniqueId).find( '.ui-jqgrid-toppager').css({"position":"relative","margin-bottom":"0px","left":"3px"});
				}
				if(/*type=='RoleDisplayUsers' || */type=='grpAddUsers' || type=='grpAddOwners'){
					$("#datagrid-container-"+uniqueId+"_toppager").css('position','absolute');
					if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
						var leftPx = '350px',normWidth = '145px';
					}else{
						if(type=='grpAddUsers' || type=='grpAddOwners')
							var leftPx = '290px',normWidth = '194px';
						else
							var leftPx = '288px',normWidth = '196px';
					}
					$("#datagrid-container-"+uniqueId+"_toppager").css('left',leftPx);
					if(type=='grpAddUsers' || type=='grpAddOwners'){
						if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
							var leftPx = '350px',normWidth = '145px';
							if ($.browser.msie && parseInt($.browser.version) == 10) {
								var leftPx = '340px',normWidth = '150px';
							}

							$("#datagrid-container-"+uniqueId+"_toppager").css('top','-45px');
							$("#datagrid-container-"+uniqueId+"_toppager").css('left',leftPx);
						}
						else
						{
							$("#datagrid-container-"+uniqueId+"_toppager").css('top','-44px');
						}
					}else{
//						$("#datagrid-container-"+uniqueId+"_toppager").css('top','-46px');
					}
					var totalGridPage = $("#datagrid-container-"+uniqueId+"_toppager .search-pagination-right span").html();
					//totalGridPage = '50000';
					totalGridPage = totalGridPage.replace(' ','');
					//console.log(totalGridPage);
					var tNum = parseInt(totalGridPage);
					if(tNum > 99){
						var lesslength = totalGridPage.length - 2;
						totalGridPage = totalGridPage.substr(0, totalGridPage.length -lesslength)+'..';
						//console.log(totalGridPage)
						$("#datagrid-container-"+uniqueId+"_toppager .search-pagination-right span").html(totalGridPage);
						$("#datagrid-container-"+uniqueId+"_toppager .search-pagination-right span").attr('title',tNum);
						$("#datagrid-container-"+uniqueId+"_toppager .search-pagination-right span").attr('class','vtip');
						var ua = window.navigator.userAgent;
			            var msie = ua.indexOf("MSIE ");
			            if (msie > 0){
			            	if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
				            	$('#exportcontainer').css('right','125px');
				            	$("#datagrid-container-"+uniqueId+"_toppager").css('left','345px');
								$("#datagrid-container-"+uniqueId+"_toppager .ui-pg-table").css('width','auto');
			            	}
						}
			            vtip();
					}
					$("#datagrid-container-"+uniqueId+"_toppager").css('width',normWidth);
					$("#datagrid-container-"+uniqueId+"_toppager_center").css('width', normWidth);
					$("#datagrid-container-"+uniqueId+"_toppager .ui-pg-table").css('right','auto');
					if(type=='grpAddUsers' || type=='grpAddOwners'){
						if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
							$("#datagrid-container-"+uniqueId+"_toppager .ui-pg-table").css('right','7px');
						}
					}
					$("#datagrid-container-"+uniqueId+"_toppager_center .ui-icon-seek-prev").css('height','25px');
					// Export option in Group Users for the group user  #0039985
					if($("#datagrid-container-"+uniqueId).getGridParam("records") > 5 && (type =='RoleDisplayUsers' || type =='RoleDisplayOwners')){
						$('#exportcontainer').find("span.exportcontainer-page-pipe-delimter").remove();
						$("#exportcontainer").removeClass('exportcontainer');
						$("#exportcontainer").addClass('exportcontainer-page');
						if(document.getElementById('page-pipe') == null)
							$('#exportcontainer').append('<span id = "page-pipe" class="page-pipeline">|</span>');
					}
				}
				if((type=='RoleDisplayUsers' || type=='RoleDisplayOwners') && mode== 'viewscreen'){
					$("#datagrid-container-"+uniqueId+"_toppager").find('.ui-pg-table').css('width', 'auto');
					$("#datagrid-container-"+uniqueId+"_toppager_center .search-pagination-right").find("span:first-child").remove();
					var totalGridPage = $("#datagrid-container-"+uniqueId+"_toppager .search-pagination-right span").html();
					totalGridPage = totalGridPage.replace(' ','');
					var tNum = parseInt(totalGridPage);
					if(tNum > 99){
						var lesslength = totalGridPage.length - 2;
						totalGridPage = totalGridPage.substr(0, totalGridPage.length -lesslength)+'..';
						$("#datagrid-container-"+uniqueId+"_toppager .search-pagination-right span").html(totalGridPage);
						$("#datagrid-container-"+uniqueId+"_toppager .search-pagination-right span").attr('title',tNum);
						$("#datagrid-container-"+uniqueId+"_toppager .search-pagination-right span").attr('class','vtip');
					}
					vtip();
				}
				if (type=="SurAssAttachQuestion" || type=="TPAttachCourse") {
					$("#datagrid-container-"+uniqueId+"_toppager_center .search-pagination-right").find("span:first-child").remove();
				}
					if(data.type == 'orgusers'){
						var expIconPos=$(".orgusers").find("#page-pipe").css("right");
						//console.log(expIconPos);
						if(this.currTheme == 'expertusoneV2'){
							$('.orgusers .exportcontainer').css({'position':'absolute','top':'56px','right':expIconPos});
						}else{
							$('.orgusers .exportcontainer').css({'position':'absolute','top':'56px','right':expIconPos});
						}
					}
			} else {
				if (type=="prerequisite" || type=="equivalence" || type=="enrolltpuser" || type=="enrolluser"  || type == 'shareuser'  || type=='survey' || type=='assessment' || type=='content' || type=='RoleDisplayUsers' || type=='RoleDisplayOwners' || type=='grpAddUsers' || type=='grpAddOwners' || type == "ReportSchedulesList") {
						$("#gview_datagrid-container-"+uniqueId).css('top','0px');
						$("#gview_datagrid-container-"+uniqueId).css('position','relative');
				}
				if(type == "trainings"){
					$("#gview_datagrid-container-"+uniqueId).css('margin-top','0px');
				}
				if (type == "ReportSchedulesList") {
				  $('#gview_datagrid-container-' + uniqueId).css('margin-top', '33px');
				}
				if (type=='content') {
					$("#gview_datagrid-container-"+uniqueId).addClass('multiSelectIEfix');
				}
				if(type == 'addProductOrder'){
					$("#gview_datagrid-container-"+uniqueId).css('top','-31px');
					$("#gview_datagrid-container-"+uniqueId).css('position','relative');
				}
				if (type=="SurAssAttachQuestion" || type=="TPAttachCourse") {
					if($("#datagrid-container-"+uniqueId).getGridParam("records") == 0) {
						$('#admin-add-questions-survey-assessment').find('.admin_add_multi_search_container').hide();
						$('#admin-add-course-training-plan').find('.admin_add_multi_search_container').hide();
						$('#admin-add-questions-survey-assessment').find('.norecords-msg-edit-page').addClass('admin-display-norecords-text-box');
						$('#admin-add-course-training-plan').find('.norecords-msg-edit-page').addClass('admin-display-norecords-text-box');
						if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
						  $('#admin-add-questions-survey-assessment .admin-attach-survey-assessment-question-tab-datagrid-wrapper .ui-jqgrid-bdiv').css('display','none');
						}
					}
					$("#datagrid-container-"+uniqueId+"_toppager").css('visibility','hidden');
				}
				else {
					$("#datagrid-container-"+uniqueId+"_toppager").css('display','none');
				}
				if(data.type == 'orgusers'){
					if(this.currTheme == 'expertusoneV2'){
						$('.orgusers .exportcontainer').css({'position':'absolute','top':'50px','right':'7px'});
					}else{
						$('.orgusers .exportcontainer').css({'position':'absolute','top':'58px','right':'5px'});
					}

				}
				if( type =='RoleDisplayUsers' || type =='RoleDisplayOwners'){ // Remove the pipeline if the No Records in Group user #0039985
					$("#page-pipe").remove();
					$("#exportcontainer").removeClass('exportcontainer-page');
				    $("#exportcontainer").addClass('exportcontainer');
				}
			}
			$('body').data('mulitselectdatagrid').destroyLoader('datagrid-div-'+uniqueId);

			$('#first_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId);
					if(type == 'attachQuestion') {
						var entityType = $('input[name="hidden_entity_type_'+uniqueId+'"]').val();
						$('body').data('mulitselectdatagrid').storeSurveyAssessmentValues(entityType, uniqueId);
					}
					if(type == 'enrolluser'  || type == 'shareuser'  || type == 'enrolltpuser') {
						$('body').data('mulitselectdatagrid').storeEnrollmentValues($('body').data('mulitselectdatagrid').entityId, $('body').data('mulitselectdatagrid').entityType, uniqueId);
					}
				}
			});

			$('#prev_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId);
					if(type == 'attachQuestion') {
						var entityType = $('input[name="hidden_entity_type_'+uniqueId+'"]').val();
						$('body').data('mulitselectdatagrid').storeSurveyAssessmentValues(entityType, uniqueId);
					}
					if(type == 'enrolluser'  || type == 'shareuser' || type == 'enrolltpuser') {
						$('body').data('mulitselectdatagrid').storeEnrollmentValues($('body').data('mulitselectdatagrid').entityId, $('body').data('mulitselectdatagrid').entityType, uniqueId);
					}
				}
			});

			$('#next_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId);
					if(type == 'attachQuestion') {
						var entityType = $('input[name="hidden_entity_type_'+uniqueId+'"]').val();
						$('body').data('mulitselectdatagrid').storeSurveyAssessmentValues(entityType, uniqueId);
					}
					if(type == 'enrolluser'  || type == 'shareuser'  || type == 'enrolltpuser') {
						$('body').data('mulitselectdatagrid').storeEnrollmentValues($('body').data('mulitselectdatagrid').entityId, $('body').data('mulitselectdatagrid').entityType, uniqueId);
					}
				}
			});

			$('#last_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId);
					if(type == 'attachQuestion') {
						var entityType = $('input[name="hidden_entity_type_'+uniqueId+'"]').val();
						$('body').data('mulitselectdatagrid').storeSurveyAssessmentValues(entityType, uniqueId);
					}
					if(type == 'enrolluser'  || type == 'shareuser' || type == 'enrolltpuser') {
						$('body').data('mulitselectdatagrid').storeEnrollmentValues($('body').data('mulitselectdatagrid').entityId, $('body').data('mulitselectdatagrid').entityType, uniqueId);
					}
				}
			});


			$('#datagrid-container-'+uniqueId+' tr').click(function(event){
				event.stopPropagation();
			});
			// Select all check and unselect all check funtionality
			$('#multiselect-selectall-'+uniqueId).click(function(event){
				event.stopPropagation();
				var existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
				if(existingList != '' || existingList != null || existingList != undefined)
				var existingListArray = existingList.split(',');
				var entityType = $('body').data('mulitselectdatagrid').entityType;
				if($(this).attr('checked') == true){
					$('#datagrid-container-'+uniqueId+' tr').each(function(){
						var selectedId = $(this).attr('id');
						var selectedId2 = $(this).attr('id');
						if((type == 'assessment' || type == 'survey') && selectedId != ''){
							var uniqueId2 = selectedId+'-'+entityId+'-'+entityType;
							var viewopt = $('#enrolled_selected_'+uniqueId2).html();
							selectedId = (viewopt == Drupal.t('LBL662') || viewopt == 'Horizontal') ? selectedId+'-H' : selectedId+'-V';	//'Horizontal'
						}
						if(selectedId != '' && $.inArray(selectedId, existingListArray) == -1 && $('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).attr('disabled') != true){
							existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
							if(existingList == ''){
								$('input[name="hidden_idlist_'+uniqueId+'"]').val(selectedId);
							} else {
								$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingList + ',' +selectedId);
							}
							$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId2).attr('checked', true);
							$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId2).parent().removeClass('checkbox-unselected');
							$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId2).parent().addClass('checkbox-selected');
						}
					});
				} else {
					var removeList = new Array();
					var c = 0;
					$('#datagrid-container-'+uniqueId+' tr').each(function(){
						var selectedId = $(this).attr('id');
						$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).attr('checked', false);
						$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).parent().removeClass('checkbox-selected');
						$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).parent().addClass('checkbox-unselected');
						if(type == 'assessment' || type == 'survey') {
						  removeList[c] = (viewopt == Drupal.t('LBL662') || viewopt == 'Horizontal') ? selectedId+'-H' : selectedId+'-V';	//'Horizontal'
						}
						else {
						  removeList[c] = selectedId;
						}
						c++;

					});
					$('input[name="hidden_idlist_'+uniqueId+'"]').val('');
					for(var i=0; i< existingListArray.length; i++){
						var selectedId = existingListArray[i];
						if(type == 'assessment' || type == 'survey'){
							var viewopt = $('#enrolled_selected_'+uniqueId).html();
							selectedId = selectedId;
						}
						if($.inArray(selectedId, removeList) == -1 ){
							existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
							if(existingList == ''){
								$('input[name="hidden_idlist_'+uniqueId+'"]').val(selectedId);
							} else {
								$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingList + ',' +selectedId);
							}
						}
					}

				}

				if(type == 'enrolluser'  || type == 'shareuser' || type == 'enrolltpuser'){
					$('body').data('mulitselectdatagrid').enabledDisableSaveItems(uniqueId, 1);
				}

			});

			// Alternate option for multi-select to align right
			$('#datagrid-container-'+uniqueId+' .multiselect-singlecheck').click(function(){
				if(type == 'assessment'){// Disabling the multicheckbox selection
					var len = $('#datagrid-container-'+uniqueId+' input[type=checkbox]').length;
					if(len > 0) {
						$('input[name="hidden_idlist_'+uniqueId+'"]').val('');
						var pval = $(this).is(':checked');
						//$('#datagrid-container-'+uniqueId+' .multiselect-singlecheck').removeAttr('checked');
						var inobj = this;

						setTimeout(function(){
							$('#datagrid-container-'+uniqueId+' input[type=checkbox]').each(function(){
								if(len==1){
									if($(inobj).attr('id') == $(this).attr('id')){
										if(pval==false){
											$(this).removeAttr('checked');
										}else{
											$(this).attr('checked','checked');
										}
									}else if($(inobj).attr('id') != $(this).attr('id') && pval==true){
										$(this).attr('checked','checked');
									}
								}else{
									$(inobj).attr('checked','checked');
								}
								var givenId = $(this).attr('id');
								if($(this).is(':checked')){
									if(givenId.indexOf("pre_status") > 0) {
										$(this).parent().attr('title',Drupal.t('LBL1253')+" "+Drupal.t('Assessment'));
									}
									if(givenId.indexOf("preview_option") > 0){
										$(this).parent().attr('title','');
									}
								}else{
									$(this).removeAttr('checked');
									if(givenId.indexOf("pre_status") > 0) {
										$(this).parent().attr('title',Drupal.t('LBL871')+" "+Drupal.t('Assessment'));
									}
									if(givenId.indexOf("preview_option") > 0){
										$(this).parent().attr('title','');
									}
								}
							})
						},20)
					}
				}


				var selectedId = $(this).val();
				var entityType = $('body').data('mulitselectdatagrid').entityType;
				if(type == 'assessment' || type == 'survey'){
					var uniqueId2 = selectedId+'-'+entityId+'-'+entityType;
					var viewopt1 = $('#enrolled_selected_'+uniqueId2).html();
					selectedId = (viewopt1 == Drupal.t('LBL662') || viewopt1 == 'Horizontal') ? selectedId+'-H' : selectedId+'-V';	//'Horizontal'
				}

				 if(type == 'assessment' && $(this).attr('type') == 'radio'){
                     var existingList = '';
				 }else{
                     var existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
				 }
				var existingListArray = existingList.split(',');
				if($(this).attr('checked') == true){
					if(selectedId != '' && $.inArray(selectedId, existingListArray) == -1 ){
							if(existingList == ''){
								$('input[name="hidden_idlist_'+uniqueId+'"]').val(selectedId);
							} else {
								$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingList + ',' +selectedId);
							}
					}
				} else {
					if(existingList != ''){
						var existingListArray = existingList.split(',');
						var existingListLen = existingListArray.length;
						$('input[name="hidden_idlist_'+uniqueId+'"]').val('');
						for( var i = 0; i < existingListLen; i++) {
							if(selectedId != existingListArray[i]){
								existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
								if(existingList == ''){
									$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingListArray[i]);
								} else {
									$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingList + ',' +existingListArray[i]);
								}
							}
						}
					}
				}
				// If all checkbox selected - multiselect-all should be checked
				$('body').data('mulitselectdatagrid').retainSelectAllCheckbox(uniqueId);

				if(type == 'enrolluser'  || type == 'shareuser' || type == 'enrolltpuser'){
					$('body').data('mulitselectdatagrid').enabledDisableSaveItems(uniqueId, 1);
				}

			});

			// Can be used if we have Multiselect option as true - Checkbox with left aligned
			/**/
			// To select all
			$('#cb_datagrid-container-'+uniqueId).click(function(){
				var existingList = '';
				$('input[name="hidden_idlist_'+uniqueId+'"]').val('');
				if($(this).attr('checked') == true){
					$('#datagrid-container-'+uniqueId+' tr').each(function(){
						var selectedId = $(this).attr('id');
						existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
						if(existingList == ''){
							$('input[name="hidden_idlist_'+uniqueId+'"]').val(selectedId);
						} else {
							$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingList + ',' +selectedId);
						}
					});
				}
			});
			/**/

			// Can be used if we have Multiselect option as true - Checkbox with left aligned
			/* */
			// Storing selected Ids and removing unselected Ids
			$('#datagrid-container-'+uniqueId+' .cbox').click(function(){

				if(type == 'assessment'){ // Disabling the multicheckbox selection
						$('input[name="hidden_idlist_'+uniqueId+'"]').val('');
						$('#datagrid-container-'+uniqueId+' .cbox').attr('checked', false);
						$(this).attr('checked', true);
				}

				var selectedId = $(this).parents('tr').attr('id');
				var existingList = '';

				if($(this).attr('checked') == true){
					existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
					if(existingList == ''){
						$('input[name="hidden_idlist_'+uniqueId+'"]').val(selectedId);
					} else {
						$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingList + ',' +selectedId);
					}
				} else {

					existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
					if(existingList != ''){
						var existingListArray = existingList.split(',');
						var existingListLen = existingListArray.length;
						$('input[name="hidden_idlist_'+uniqueId+'"]').val('');
						for( var i = 0; i < existingListLen; i++) {
							if(selectedId != existingListArray[i]){
								existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
								if(existingList == ''){
									$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingListArray[i]);
								} else {
									$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingList + ',' +existingListArray[i]);
								}
							}
						}
					}
				}
			});

			if(type == 'enrolluser'  || type == 'shareuser' ){
				// Enroll user status section for class
				$('#catalog-class-basic-addedit-form-container .enrolled-selection').click(function(){
					$('#catalog-class-basic-addedit-form-container .enrolled-status ul.sub-menu').hide();
					$('.qtip-popup-exempted').css('display', 'none');
					if($(this).next('.sub-menu').css('display') == 'block'){
						$(this).next('.sub-menu').css('display', 'none');
					} else {
						$(this).next('.sub-menu').css('display', 'block');
					}
				});

				$('#catalog-class-basic-addedit-form-container .enrolled-status .sub-menu li').click(function(){
					//var obj	= this;
					var data = $(this).attr('data');
					var id = $(this).parents('tr').attr('id');
					var dataArray = data.split('-');
					var status = dataArray[0];
					var entityId = dataArray[2];
					var entityType = dataArray[3];
					var rowNumber =dataArray[4];

					if($('#multiselect-singlecheck-enrolluser-'+entityId+'-'+entityType+'_'+id).attr("checked") == false){
						$('#multiselect-singlecheck-enrolluser-'+entityId+'-'+entityType+'_'+id).attr("checked", false)[0].click();
					}
					var defaultStatus = $('#enrolled_default_status_'+id+'-'+entityId+'-'+entityType).val();
					if(status == defaultStatus){
						$('#multiselect-singlecheck-enrolluser-'+entityId+'-'+entityType+'_'+id).attr("checked", true)[0].click();
					}

					var uniqueId2 = id+'-'+entityId+'-'+entityType;
					if(status == 'Completed'){
						var sesStartEndDate = $('body').data('mulitselectdatagrid').sesStartEndDate;
						var sesStartsOnArray = sesStartEndDate.split('#');
						var sesStartsOn = sesStartsOnArray[0];
						var sesEndsOn = sesStartsOnArray[1];
                        var clsType = sesStartsOnArray[2];
                        //For IE8 issue ticket: 0024236
                        if(clsType == 'ilt_or_vc'){
                            var sesStarts =  sesStartsOn.split(',');
                            var sesEnds =  sesEndsOn.split(',');
                            var sessMinDate = new Date(sesStarts[0],sesStarts[1]-1,sesStarts[2]);
                            var sessMaxDate = new Date(sesEnds[0],sesEnds[1]-1,sesEnds[2]);
                        }else{
                           // var sessMinDate = new Date(); #0040096
                            var sessMaxDate = new Date();
                        }

						$('#default_status_date_'+uniqueId2).hide();
						$('#completion_sel_date_'+uniqueId2).show();
						$('#score_'+uniqueId2).removeAttr('disabled');
						$('#score_'+uniqueId2).css('backgroundColor','');
						//$('#completion_sel_date_'+uniqueId2).datepicker({ defaultDate: 1, dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
						this.currTheme = Drupal.settings.ajaxPageState.theme;
						if(clsType == 'ilt_or_vc'){
							if(this.currTheme == 'expertusoneV2'){
								$('#completion_sel_date_'+uniqueId2).datepicker({ minDate: sessMinDate, dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG" , buttonImageOnly: true,buttonText: Drupal.t("LBL218") });
							}else{
								$('#completion_sel_date_'+uniqueId2).datepicker({ minDate: sessMinDate, dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true,buttonText: Drupal.t("LBL218") });
							}
						}else{ // For Video and WBT Class Admin Roster Can Give a Any Prev Or Future Dates . #0040096
							if(this.currTheme == 'expertusoneV2'){
								$('#completion_sel_date_'+uniqueId2).datepicker({ defaultDate: 1, dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true, buttonText: Drupal.t("LBL218")  });
							}else{
								$('#completion_sel_date_'+uniqueId2).datepicker({ defaultDate: 1, dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true, buttonText: Drupal.t("LBL218")  });
							}
						}
						//If Select Complete Status link the last Session date is display in the text box. Ref Ticket: 0023408
						$('#completion_sel_date_'+uniqueId2).datepicker('setDate', sessMaxDate);
					} else {
						$('#default_status_date_'+uniqueId2).show();
						$('#completion_sel_date_'+uniqueId2).hide();
						$('#completion_sel_date_'+uniqueId2).datepicker( "destroy" );
						$('#score_'+uniqueId2).attr('disabled', 'disabled');
						$('#score_'+uniqueId2).css('backgroundColor','#CCCCCC');
					}

					var existStsVal = $('#enrolled_status_'+id+'-'+entityId+'-'+entityType).val();
					if(status == 'Waived' || (defaultStatus == Drupal.t('Waived') && (status == 'Enrolled' || status == 'In progress'))) {
						var drstatus = Drupal.t(status);
						var waivedlinkHTML = '';
						if(rowNumber > 5) {
							var qtipOptWaivedObj  = "{'entityId' : '"+ id + "'"+
							", 'entityType' : '"+entityType +"'" +
							", 'url' : 'administration/enrollment/exempted/single/" + entityId + "/" + id + "/class/0'" +
							", 'popupDispId' : 'enrolled-exempted-"+id + "'"+
							", 'catalogVisibleId' : 'qtipAttachIdqtip_exempted_single_disp'" +
							", 'wid' : 400"+", 'heg' : 150" +", 'postype' : 'bottomright'" +",'poslwid' : 70 "+", 'qdis' : 'ctool'" +
							", 'linkid' : 'visible-exempted-"+ id+"','dispDown' : ''"+", 'enrId' : '"+entityId +"'}";
							
						}else {
							var qtipOptWaivedObj  = "{'entityId' : '"+ id + "'"+
							", 'entityType' : '"+entityType +"'" +
							", 'url' : 'administration/enrollment/exempted/single/" + entityId + "/" + id + "/class/0'" +
							", 'popupDispId' : 'enrolled-exempted-"+id + "'"+
							", 'catalogVisibleId' : 'qtipAttachIdqtip_pwdstrength_visible_disp_" + id + "'" +
							", 'wid' : 400"+", 'heg' : 150" +", 'postype' : 'bottomright'" +",'poslwid' : 70 "+", 'qdis' : 'ctool'" +
							", 'linkid' : 'visible-exempted-"+ id+"','dispDown' : 'Y'"+", 'enrId' : '"+entityId +"'}";
						}
						waivedlinkHTML = 
							'<span class="enrolled-exempted-container">' +
							'<span id="enrolled-exempted-' + id + '" class="enrolled-exempted-status">' +
							'<a id="visible-exempted-'+id+'" onclick = "exemptedVisibility(' + qtipOptWaivedObj + ');" class="enrolled-exempted-status-link">' +
							drstatus +
							'</a>' +
							'<span id="visible-popup-' + id + '" class="qtip-popup-exempted" style="display:none; position:absolute; left:0px; top:0px;" ></span>'+
							'</span>' +
							'</span>';
						$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).removeClass('vtip');
						$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).html(waivedlinkHTML);
						$('#enrolled_status_'+id+'-'+entityId+'-'+entityType).val(status);
						$('#visible-exempted-'+id).click();
						$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).removeAttr('title');
						$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).removeClass('vtip');
					}else{
						$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).removeClass('vtip');
						$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).html(Drupal.t(status));
						$('#enrolled_status_'+id+'-'+entityId+'-'+entityType).val(status);
					}
					
					$(this).parent().css('display', 'none');

					$('body').data('mulitselectdatagrid').enabledDisableSaveItems(uniqueId, 0);

				});

				$('.enroll-user-competion-score').keyup(function(){
					var uniqueId1 = $(this).attr('data1');
					var uniqueId2 = $(this).attr('data2');
					if($('#enrolled_default_status_'+uniqueId2).val() == 'Completed'){
						if($(this).val() == ''){
							$('#multiselect-singlecheck-'+uniqueId1).attr("checked", true)[0].click();
						} else {
							$('#multiselect-singlecheck-'+uniqueId1).attr("checked", false)[0].click();
						}
					}
					return false;
				});

				//CLASS ENROLLMENT STATUS LINK ALIGNMENT ISSUE FIXE
				if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").css('overflow','visible');
				}else{
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").css('overflow','inherit');
				}

			} else if(type == 'enrolltpuser'){

				// Enroll user status section for class
				$('#program-tp-basic-addedit-form-container .enrolled-selection').click(function(){
					$('#program-tp-basic-addedit-form-container .enrolled-status ul.sub-menu').hide();
					$('.qtip-popup-exempted').css('display', 'none');
					if($(this).next('.sub-menu').css('display') == 'block'){
						$(this).next('.sub-menu').css('display', 'none');
					} else {
						$(this).next('.sub-menu').css('display', 'block');
					}
				});

				$('#program-tp-basic-addedit-form-container .enrolled-status .sub-menu li').click(function(){
					var data = $(this).attr('data');
					var id = $(this).parents('tr').attr('id');
					var dataArray = data.split('-');
					var status = dataArray[0];
					var entityId = dataArray[2];
					var entityType = dataArray[3];
					var rowNumber =dataArray[4];

					if($('#multiselect-singlecheck-enrolltpuser-'+entityId+'-'+entityType+'_'+id).attr("checked") == false){
						$('#multiselect-singlecheck-enrolltpuser-'+entityId+'-'+entityType+'_'+id).attr("checked", false)[0].click();
					}
					var defaultStatus = $('#enrolled_default_status_'+id+'-'+entityId+'-'+entityType).val();
					if(status == defaultStatus){
						$('#multiselect-singlecheck-enrolltpuser-'+entityId+'-'+entityType+'_'+id).attr("checked", true)[0].click();
					}

					var uniqueId2 = id+'-'+entityId+'-'+entityType;
					if(status == 'Completed'){
						$('#default_status_date_'+uniqueId2).hide();
						$('#completion_sel_date_'+uniqueId2).show();
						this.currTheme = Drupal.settings.ajaxPageState.theme;
						if(this.currTheme == 'expertusoneV2'){
							$('#completion_sel_date_'+uniqueId2).datepicker({ defaultDate: 1, dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true, buttonText: Drupal.t("LBL218")  });
						}else{
							$('#completion_sel_date_'+uniqueId2).datepicker({ defaultDate: 1, dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true, buttonText: Drupal.t("LBL218")  });
						}
						$('#completion_sel_date_'+uniqueId2).datepicker('setDate', new Date());
					} else {
						$('#default_status_date_'+uniqueId2).show();
						$('#completion_sel_date_'+uniqueId2).hide();
						$('#completion_sel_date_'+uniqueId2).datepicker( "destroy" );
					}

					var existStsVal = $('#enrolled_status_'+id+'-'+entityId+'-'+entityType).val();
					if(status == 'Waived' || (defaultStatus == Drupal.t('Waived') && (status == 'Enrolled' || status == 'In progress'))) {
						var drstatus = Drupal.t(status);
						var waivedlinkHTML = '';
						
						if(rowNumber > 5) {
							var qtipOptWaivedObj  = "{'entityId' : '"+ id + "'"+
							", 'entityType' : '"+entityType +"'" +
							", 'url' : 'administration/enrollment/exempted/single/" +entityId + "/"+ id + "/tp/0'" +
							", 'popupDispId' : 'enrolled-exempted-"+id + "'"+
							", 'catalogVisibleId' : 'qtipAttachIdqtip_exempted_single_disp'" +
							", 'wid' : 400"+", 'heg' : 200" +", 'postype' : 'bottomright'" +",'poslwid' : 70 "+", 'qdis' : 'ctool'" +
							", 'linkid' : 'visible-exempted-"+ id+"','dispDown' : ''"+", 'enrId' : '"+entityId +"'}";
						}else {
							var qtipOptWaivedObj  = "{'entityId' : '"+ id + "'"+
							", 'entityType' : '"+entityType +"'" +
							", 'url' : 'administration/enrollment/exempted/single/" +entityId + "/"+ id + "/tp/0'" +
							", 'popupDispId' : 'enrolled-exempted-"+id + "'"+
							", 'catalogVisibleId' : 'qtipAttachIdqtip_pwdstrength_visible_disp_" + id + "'" +
							", 'wid' : 400"+", 'heg' : 200" +", 'postype' : 'bottomright'" +",'poslwid' : 70 "+", 'qdis' : 'ctool'" +
							", 'linkid' : 'visible-exempted-"+ id+"','dispDown' : 'Y'"+", 'enrId' : '"+entityId +"'}";
							
						}
						waivedlinkHTML = 
							'<span class="enrolled-exempted-container">' +
							'<span id="enrolled-exempted-' + id + '" class="enrolled-exempted-status">' +
							'<a id="visible-exempted-'+id+'" class="enrolled-exempted-status-link" onclick = "exemptedVisibility(' + qtipOptWaivedObj + ');">' +
							drstatus +
							'</a>' +
							'<span id="visible-popup-' + id + '" class="qtip-popup-exempted" style="display:none; position:absolute; left:0px; top:0px;" ></span>'+
							'</span>' +
							'</span>';
						
						$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).removeClass('vtip');
						$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).html(waivedlinkHTML);
						$('#enrolled_status_'+id+'-'+entityId+'-'+entityType).val(status);
						$('#visible-exempted-'+id).click();
					}else{
						$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).html(Drupal.t(status));
						$('#enrolled_status_'+id+'-'+entityId+'-'+entityType).val(status);
					}
					
					$(this).parent().css('display', 'none');

					$('body').data('mulitselectdatagrid').enabledDisableSaveItems(uniqueId, 0);

				});

				$('.enroll-user-competion-score').keyup(function(){
					var uniqueId1 = $(this).attr('data1');
					var uniqueId2 = $(this).attr('data2');
					if($('#enrolled_default_status_'+uniqueId2).val() == 'Completed'){
						if($(this).val() == ''){
							$('#multiselect-singlecheck-'+uniqueId1).attr("checked", true)[0].click();
						} else {
							$('#multiselect-singlecheck-'+uniqueId1).attr("checked", false)[0].click();
						}
					}
					return false;
				});

				//TRAINING PROGRAM ENROLLMENT STATUS LINK ALIGNMENT ISSUE FIXE
				$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").css('overflow','inherit');

				$('body').data('mulitselectdatagrid').enabledDisableSaveItems(uniqueId, 0);

			}else if(type == 'assessment' || type == 'survey'){
				var Etype = $('body').data('mulitselectdatagrid').entityType;
				if(Etype == 'cre_sys_obt_cls'){
					$('#catalog-class-basic-addedit-form-container .enrolled-selection').click(function(){
						$('#catalog-class-basic-addedit-form-container .enrolled-status ul.sub-menu').hide();
							$(this).next('.sub-menu').toggle();
							//$('#catalog-class-basic-addedit-form-container .enrolled-status ul.sub-menu').css('right','147px');
							//$('#catalog-class-basic-addedit-form-container .survey-attach-grid-wrapper .enrolled-status ul.sub-menu').css('right','68px');
							if(navigator.userAgent.indexOf("Chrome")>0){
								//$('#catalog-class-basic-addedit-form-container .assess-attach-grid-wrapper .enrolled-status ul.sub-menu').css('right','160px');
							}else {
								//$('#catalog-class-basic-addedit-form-container .assess-attach-grid-wrapper .enrolled-status ul.sub-menu').css('right','137px');
							}
					});

					$('#catalog-class-basic-addedit-form-container .enrolled-status .sub-menu li').click(function(){
						var data = $(this).attr('data');
						var id = $(this).parents('tr').attr('id');
						var dataArray = data.split('-');
						var status = dataArray[0];
						var entityId = dataArray[2];
						var entityType = dataArray[3];
						var uniqueId2 = id+'-'+entityId+'-'+entityType;
						var uniqueId = id+'-'+type+'-'+entityId+'-'+entityType;		 // 230-assessment-322-cre_sys_obt_cls-
						//$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).html(Drupal.t(status));
						//$('#enrolled_status_'+id+'-'+entityId+'-'+entityType).val(status);
						var viewopt1 = $('#enrolled_selected_'+uniqueId2).html();
						$(this).parent().css('display', 'none');
						if(status == 'Horizontal' || status == Drupal.t('LBL662')){
							//$('#Horizontal-'+uniqueId2).css('display','none');
							//$('#Vertical-'+uniqueId2).css('display','block');
							//$('#'+uniqueId+'-Horizontal').css('display','none');
							$('#'+uniqueId+'-Vertical').css('display','block');
							$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).html(Drupal.t('LBL662'));
							$('#enrolled_status_'+id+'-'+entityId+'-'+entityType).val(Drupal.t('LBL663'));
							$(this).html(Drupal.t('LBL663'));
							$(this).attr('data', 'Vertical' +'-'+dataArray[1]+'-'+dataArray[2]+'-'+dataArray[3] ); // Vertical-230-322-cre_sys_obt_cls
							$(this).attr('id', uniqueId + '-' + 'Vertical');	//Drupal.t('LBL663')
						}else{
							//$('#'+uniqueId+'-Vertical').css('display','none');
							$('#'+uniqueId+'-Horizontal').css('display','block');
							$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).html(Drupal.t('LBL663'))
							$('#enrolled_status_'+id+'-'+entityId+'-'+entityType).val(Drupal.t('LBL662'));
							$(this).html(Drupal.t('LBL662'));
							$(this).attr('data', 'Horizontal' +'-'+dataArray[1]+'-'+dataArray[2]+'-'+dataArray[3] );
							$(this).attr('id', uniqueId + '-' + 'Horizontal');	//Drupal.t('LBL662')
						}

					});
					
				}else{
					$('#program-tp-basic-addedit-form-container .enrolled-selection').click(function(){
						$('#program-tp-basic-addedit-form-container .enrolled-status ul.sub-menu').hide();
							$(this).next('.sub-menu').toggle();
					});

					$('#program-tp-basic-addedit-form-container .enrolled-status .sub-menu li').click(function(){
						var data = $(this).attr('data');
						var id = $(this).parents('tr').attr('id');
						var dataArray = data.split('-');
						var status = dataArray[0];
						var entityId = dataArray[2];
						var entityType = dataArray[3];
						var uniqueId2 = id+'-'+entityId+'-'+entityType;
						var uniqueId = id+'-'+type+'-'+entityId+'-'+entityType;
						$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).html(Drupal.t(status == 'Vertical' ? 'LBL663' : 'LBL662'));
						$('#enrolled_status_'+id+'-'+entityId+'-'+entityType).val(status);
						$(this).parent().css('display', 'none');
						var viewopt1 = $('#enrolled_selected_'+uniqueId2).html();
						if(viewopt1 == Drupal.t('LBL662') || viewopt1 == 'Horizontal'){	//'Horizontal'
							$('#'+id+'-'+uniqueId2+'-Horizontal').css('display','none');
							$('#'+id+'-'+uniqueId2+'-Vertical').css('display','block');
							$(this).html(Drupal.t('LBL663'));
							$(this).attr('data', 'Vertical' +'-'+dataArray[1]+'-'+dataArray[2]+'-'+dataArray[3] ); // Vertical-230-322-cre_sys_obt_cls	//Drupal.t('LBL663')
							$(this).attr('id', uniqueId + '-' + 'Vertical');	//Drupal.t('LBL663')
						}else{
							$('#'+id+'-'+uniqueId2+'-Vertical').css('display','none');
							$('#'+id+'-'+uniqueId2+'-Horizontal').css('display','block');
							$(this).html(Drupal.t('LBL662'));
							$(this).attr('data', 'Horizontal' +'-'+dataArray[1]+'-'+dataArray[2]+'-'+dataArray[3] ); // Vertical-230-322-cre_sys_obt_cls	//Drupal.t('LBL662')
							$(this).attr('id', uniqueId + '-' +'Horizontal');	//Drupal.t('LBL662')
						}

					});
				}
				if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").css('overflow','visible');
				}else{
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").css('overflow','inherit');
				}
			}
			if(type == 'TPAttachCourse' || type == 'SurAssAttachQuestion') {
				//$('#gview_datagrid-container-'+uniqueId).css('height','250px');
				$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
				setQtipPosition(); // Fix for issue #0015361: UI Issues in Survey/Assessment
			}
			if(type=='grpAddUsers' || type=='grpAddOwners'){
				setQtipPosition();
			}

			/* ODD AND EVEN ROW BACKGROUND COLOR */
			if(type == 'prerequisite' || type == 'equivalence' || type == 'trainings') {
				/* Apply background color to data grid rows*/
				if($('body').data('mulitselectdatagrid').entityType == 'cre_sys_obt_trp') {
					$('#gview_datagrid-container-'+uniqueId).css('height','368px');
					$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
					$('#datagrid-container-'+uniqueId+' tr.jqgfirstrow').css("background", "none");
					$('#datagrid-container-'+uniqueId).css('width','610px');
					$('.ui-jqgrid-htable').css('width','610px');
				}else {
					$('#gview_datagrid-container-'+uniqueId).css('height','310px');
					$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
					$('#datagrid-container-'+uniqueId+' tr.jqgfirstrow').css("background", "none");
					$('#datagrid-container-'+uniqueId).css('width','610px');
					$('.ui-jqgrid-htable').css('width','610px');
					if($.browser.msie && parseInt($.browser.version, 10)>='8'){
					$('#gview_datagrid-container-'+uniqueId).css('height','auto');
					}
				}
				$('#pg_datagrid-container-'+uniqueId+"_toppager table ").css('width','auto');
			}
			else if(type == 'survey' || type == 'assessment' || type == 'enrolluser' || type == 'enrolltpuser' || type == 'content') {
				/* Apply background color to data grid rows*/
				if($('body').data('mulitselectdatagrid').entityType == 'cre_sys_obt_cls'){
				  var mHeight = (type == 'content' || type == 'survey' || type == 'assessment' && mode == 'view') ? '460px' : '350px';
				}else{
				  var mHeight = (type == 'content' || type == 'survey' || type == 'assessment' && mode == 'view') ? '385px' : '350px';
				}
				if($("#datagrid-container-"+uniqueId).getGridParam("records") > listRows) {
					//$('#gview_datagrid-container-'+uniqueId).css('height','362px');
					//$('#bubble-face-table #gview_datagrid-container-'+uniqueId).css('min-height','330px');
					$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
					$('#datagrid-container-'+uniqueId+' tr.jqgfirstrow').css("background", "none");
					$('#datagrid-container-'+uniqueId).css('width','610px');
					$('.ui-jqgrid-htable').css('width','610px');
				} else {
					$('#gview_datagrid-container-'+uniqueId).css('min-height',mHeight);
					$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
					$('#datagrid-container-'+uniqueId+' tr.jqgfirstrow').css("background", "none");
					$('#datagrid-container-'+uniqueId).css('width','610px');
					$('.ui-jqgrid-htable').css('width','610px');
				}


				if(type == 'enrolluser'  || type == 'shareuser' ){
					this.currTheme = Drupal.settings.ajaxPageState.theme;
					if(this.currTheme=='expertusoneV2'){
						$('#gview_datagrid-container-'+uniqueId).css('min-height','361px');
						$('#gview_datagrid-container-'+uniqueId).css('width','612px');
						$('#gview_datagrid-container-'+uniqueId).css('overflow','visible');
						//$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-bdiv tr td:eq(0)').css('width','113px');

					}
					if(type == 'enrolluser'){
					   $('#gview_datagrid-container-'+uniqueId).css('height','445px');
					}
					if($('#enrolluser-grid-wrapper-'+uniqueId).find('.error').css('display')=='block'){
						var dhight = $('#enrolluser-grid-wrapper-'+uniqueId).height();
						if(dhight > 400){
							var ehight = $('#enrolluser-grid-wrapper-'+uniqueId).find('.error').height();
							var nhight = dhight+ehight
							setTimeout(function() {
							}, 10000);
						}
					}
				}
				if(type == 'enrolluser' || type == 'enrolltpuser'  || type == 'shareuser' ){
				if ($("#datagrid-container-"+uniqueId).getGridParam("records") <= 0) {
				 $(".norecords-msg-edit-page").remove();
					Msg = Drupal.t('MSG403');
					var norecordsmsg = '<div class="norecords-msg-edit-page">'+Msg+'</div>';
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").before(norecordsmsg);
					$("#datagrid-container-"+uniqueId+"_toppager").css('display','none');
					$("#datagrid-div-"+uniqueId).css('display','block');
					$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").css('display','none');
					//$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").addClass("no-item-found-class");
					$('#statistics-count-'+uniqueId).hide();
					var skip = 'yes';
				}
			  }
			}
			
			if(type == 'trainings' && $.browser.msie && parseInt($.browser.version, 10)=='9'){
				if(mode == "view" && Drupal.settings.ajaxPageState.theme == "expertusone"){
				$('#pg_datagrid-container-'+uniqueId+"_toppager .ui-pg-table .ui-pg-table ").css({"width":"140px","right":"-18px"});
				}else{
				$('#pg_datagrid-container-'+uniqueId+"_toppager .ui-pg-table .ui-pg-table ").css({"width":"150px","right":"-1px"});
				}
			}
			if(type == 'session' && $.browser.msie && parseInt($.browser.version, 10)=='8'){
				if($('#add-session-delivery-type'+this.uniqueId).val() != "lrn_cls_dty_ilt"){
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-htable tr th:eq(6)').css('width','35px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-htable tr th:eq(1)').css('width','219px');
				}
				if($('#add-session-delivery-type'+uniqueId).val() == "lrn_cls_dty_vcl"){
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-htable tr th:eq(1)').css('width','186px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(0)').css('width','125px');
				  if($.browser.msie && parseInt($.browser.version, 10)=='8'){
					  $('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(0)').css('width','118px');
				  }
				}
			}
			if($('#add-session-delivery-type'+uniqueId).val() == "lrn_cls_dty_vcl" && navigator.userAgent.indexOf("Chrome")>0){
			   $('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-htable tr th:eq(1)').css('width','185px');
			}
			/*Session alignment issue for All Browsers*/
			if(type == "session" && Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
				if($('#add-session-delivery-type'+uniqueId).val() == "lrn_cls_dty_vcl" && navigator.userAgent.indexOf("Chrome")>0){
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-htable tr th:eq(0)').css('width','142px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-htable tr th:eq(1)').css('width','140px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-htable tr th:eq(6)').css('width','81px');

					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(0)').css('width','142px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(1)').css('width','147px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(2)').css('width','90px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(3)').css('width','55px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(6)').css('width','40px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(7)').css('width','40px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(8)').css('width','30px');
					/*double click event for chrome*/
					jQuery('.session-added-edit-link-text').bind('dblclick',function() {
					 $('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:nth-child(8) > div').css('width','35px');
					  /*remove double select table , or any element*/
					    $('.add_session_popup').bind('selectstart', function(event) {
					  	   clearSelection();
					    });
					    $('.add_session_popup').trigger( "selectstart" )
					      function clearSelection() {
						    if(document.selection && document.selection.empty) {
						        document.selection.empty();
						  	    } else if(window.getSelection) {
						        var sel = window.getSelection();
						        sel.removeAllRanges();
						        }
						  }
				      });
				}
				else if($('#add-session-delivery-type'+uniqueId).val() == "lrn_cls_dty_vcl" && $.browser.mozilla && $.browser.version > '2' || $('#add-session-delivery-type'+uniqueId).val() == "lrn_cls_dty_vcl" && $.browser.msie && $.browser.version > '7'){
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-htable tr th:eq(1)').css('width','142px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-htable tr th:eq(1)').css('width','148px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-htable tr th:eq(4)').css('width','45px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-htable tr th:eq(6)').css('width','49px');

					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(0)').css('width','142px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(1)').css('width','151px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(2)').css('width','88px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(4)').css('width','50px');
					$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:eq(8)').css('width','25px');
				}

				if($('#add-session-delivery-type'+uniqueId).val() == "lrn_cls_dty_ilt" && navigator.userAgent.indexOf("Chrome")>0){
					/*double click event for chrome*/
					jQuery('.session-added-edit-link-text').bind('dblclick',function() {
					 $('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-btable tr td:nth-child(7) > div').css('width','35px');
					  /*remove double select table , or any element*/
					    $('.add_session_popup').bind('selectstart', function(event) {
					  	   clearSelection();
					    });
					    $('.add_session_popup').trigger( "selectstart" )
					      function clearSelection() {
						    if(document.selection && document.selection.empty) {
						        document.selection.empty();
						  	    } else if(window.getSelection) {
						        var sel = window.getSelection();
						        sel.removeAllRanges();
						        }
						  }
				      });
				}
				//multi instructor related work.
				$('.add_session_popup').click(function(event) {
					if($(event.target).is('.multiselect-custom-dropdown')  || ($(event.target).is('input:checkbox:not(.weekday-checkbox-input)') || $(event.target).is('.title-lengthy-text')) || ($(event.target).parents('.multiselect-custom-dropdown-results').length && $('.multiselect-custom-dropdown-results').is(':visible'))) {
						//change for 42102: Unable to move to any other modules while clicking any modules link from the user profile page
						return false;
					}
					else if (!$(event.target).parents('.multiselect-custom-dropdown,.multiselect-custom-dropdown-results').length) {
						$('.multiselect-custom-dropdown-results').hide();
//						var ins_list = $('#load_multiselect_session_instructor').val();
//						if($('#change_instructor').val() != undefined && $('#change_instructor').val() != null && $('#change_instructor').val() == 2){
//							$('#change_instructor').val(1);
//							selectDropdownOnclick();
//						}
					}
				});
			}
			if(type == 'ContentMoveUsers'){
				$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
				$('#datagrid-container-'+uniqueId+' tr.jqgfirstrow').css("background", "none");
				$("#data-table-page-view").css("display","none");
				$("#datagrid-div-"+uniqueId).css({"top":"-16px","position":"relative"});
				var recordCount = $('#datagrid-container-'+uniqueId).jqGrid('getGridParam', 'records');
					$("#gview_datagrid-container-"+uniqueId).css('position','static');
				if (recordCount == 0) {
						$("#datagrid-div-"+uniqueId).css({"top":"0"});
						$('#admin-move-users-content').find('.norecords-msg-edit-page').addClass('admin-display-norecords-text-box');
			       } else if (recordCount <= 6) {
			    	   	$("#gview_datagrid-container-"+uniqueId).css({'position':'relative','top':'20px'});
				}
				if($.browser.msie && $.browser.version == 9 && Drupal.settings.ajaxPageState.theme != 'expertusoneV2'){
					$("#datagrid-container-"+uniqueId+"_toppager_center").css('width','190px');
				   }
			}
			if(type == 'CountrySetting') {
				//$(".search-pagination-right").find("span:first-child").remove();
				if($("#datagrid-container-"+uniqueId).getGridParam("records") > listRows) {
					$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
					$('#datagrid-container-'+uniqueId+' tr.jqgfirstrow').css("background", "none");
				}else{
					$('#datagrid-container-'+uniqueId+' tr:even').css("background", "#f7f7f7");
					$('#datagrid-container-'+uniqueId+' tr.jqgfirstrow').css("background", "none");
				}
				var recordCount = $('#datagrid-container-'+uniqueId).jqGrid('getGridParam', 'records');
				 if (recordCount < 14) {
		    	   	$("#gview_datagrid-container-"+uniqueId).css({'position':'relative','top':'27px'});
			   }else{
			  	 $("#gview_datagrid-container-"+uniqueId).css({"top":"0px","position":"relative"});
			   }
				 $("#datagrid-container-"+uniqueId+"_toppager").css('width','462px');
				 $("#gview_datagrid-container-"+uniqueId).find( '.ui-jqgrid-htable').css('width','460px');
				 Drupal.ajax.prototype.commands.CtoolsModalAdjust();
				 $("#datagrid-container-"+uniqueId+"_toppager_center .search-pagination-right").find("span:first-child").remove();
				 $(".search-pagination-right").find("span:first-child").remove();
			}

			if(type == 'PaymentMethod'){
				var paymentcount = $("#datagrid-container-"+uniqueId).getGridParam("records");
				$(".ui-jqgrid-bdiv").css({'overflow':'visible'});
			  $('#datagrid-container-'+uniqueId+' tr').css({'border-bottom':'1px dotted #CCCCCC'});
				$('#datagrid-container-'+uniqueId+' tr td').css({'overflow':'visible','padding-left':'0px'});
				$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").css('display','none');
				$('#datagrid-container-'+uniqueId+' tr:eq('+paymentcount+')').css({'border-bottom':'none'});
				$('#datagrid-container-'+uniqueId+' tr.jqgfirstrow').css("border", "none");
				Drupal.ajax.prototype.commands.CtoolsModalAdjust();
			}
			if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2' && type != 'SurAssAttachQuestion'){

				$('#datagrid-div-'+uniqueId+' .ui-jqgrid-htable tr th:last-child').css("border-right", "0px none");
				$('#datagrid-div-'+uniqueId+' .ui-jqgrid-btable tr td:first-child').css({"font-family":"openSansRegular, Arial","color":"#474747"});
			}
			if(type=="TPAttachCourse"){
				if($.browser.msie && $.browser.version == 8){
					$("#datagrid-div-"+uniqueId+" .ui-jqgrid-bdiv").css('max-height','175px');
					setQtipPosition();
				}
			}
			if(data.type == 'orgusers'){
				var popupId = 'org-users-main-div-'+data.orgid;
				var p = $("#"+data.orgid);
				var position = p.position();
				var divHeight = $("#"+popupId).height();
				var parentTopPos = position.top + 200;
				if(parentTopPos > divHeight) {
					$('#admin-data-grid .orgusers .user-enrollment-div-cls .bottom-qtip-tip').css({'z-index':'101','height':'42px','left':'80px','top':'auto','background-position':'-101px -1428px','bottom':'-69px'});
					$("#"+popupId).css("bottom","60px");
				} else {
					$('#admin-data-grid .orgusers .user-enrollment-div-cls .bottom-qtip-tip').css({'z-index':'101','height':'42px','left':'80px','top':'9px','background-position':'-6px -1410px'});
				}
				$('body').data('mulitselectdatagrid').destroyLoader('datagrid-div-inside-grid-'+uniqueId);

			}
			if(type=="ReportSchedulesList" &&  this.currTheme == 'expertusoneV2'){
				$('#datagrid-div-'+uniqueId+' .ui-jqgrid-htable tr th:last-child').css("border-right", "1px solid #E5E5E5");
			}
			if(type=="ReportSchedulesList" &&  this.currTheme == 'expertusoneV2') {
			 
			}
			if(type=="ReportSchedulesList"){
				$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv").find('tr td:first-child').css("font-family",'openSansRegular, Arial');
				 Drupal.attachBehaviors();
				 $('body').data('mulitselectdatagrid').destroyLoader('datagrid-div-'+uniqueId);
			}
			if((type=="survey" && mode == 'edit') &&  this.currTheme == 'expertusoneV2') {
				var Etype = $('body').data('mulitselectdatagrid').entityType;

				 if (navigator.userAgent.indexOf("Chrome")>0) {
				  if(Etype!='cre_sys_obt_cls'){
					 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(2)').css("width",'160px');
					 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:last-child').css("width",'34px');
				 }
				}
			} else if((type=="survey" && mode == 'view') &&  this.currTheme == 'expertusoneV2') {
				var Etype = $('body').data('mulitselectdatagrid').entityType;
			  }else if((type=="survey" && mode == 'edit') &&  this.currTheme != 'expertusoneV2') {
				var Etype = $('body').data('mulitselectdatagrid').entityType;
				if ( $.browser.msie && parseInt($.browser.version, 10)=='8') {
					 if(Etype =='cre_sys_obt_cls'){
						 $("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-hdiv").find('th:nth-child(1)').css("width",'422px');
					 }
				}
			}
			//Hide the drop down option values when clicking on the body of the jqgrid
			$('.ui-jqgrid-btable td').bind('click',function(event) {
				if(event.target.id != 'admin-dropdown-arrow'){
					$('#select-list-dropdown-list dropdown-link-font ').hide();
				}
			});

			//Hide the drop down option values when clicking on the header of the jqgrid
			$('.ui-jqgrid-htable th').bind('click',function(event) {
				if(event.target.id != 'admin-dropdown-arrow'){
					$('#select-list-dropdown-list dropdown-link-font ').hide();
				}
			});
			// Added by Vincent on 02, Jan 2014 for #0029687: Refresh on Admin Enrollments page
			var allPage= EXPERTUS_SMARTPORTAL_AbstractManager.getCookie('current_page');
			if(allPage!=''){
			    crPage = allPage.split('~');
			    var chPage= $("#datagrid-container-"+uniqueId).getGridParam("page");
			    crPage[2] = chPage+"#0";
			    var updatePage = crPage[0]+"~"+crPage[1]+"~"+crPage[2];
				document.cookie="current_page="+updatePage+"; path=/";
			}
			
			if($("#admin-data-grid .ui-pg-table .ui-pg-table").is(":visible")){
			$('#catalog-class-basic-addedit-form-container').addClass('with_pagination');
			}else{
			$('#catalog-class-basic-addedit-form-container').removeClass('with_pagination');
			}
			
			}catch(e){
				// to do
			}
		},

		enabledDisableSaveItems : function(uniqueId, showOnly){
			try{
			var existingList = $('input[name="hidden_valuelist_'+uniqueId+'"]').val();
			var IdList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
			var IdListArray = IdList.split(',');
			var entityId = $('body').data('mulitselectdatagrid').entityId;
			var entityType = $('body').data('mulitselectdatagrid').entityType;
			var entityList = entityId + '-' +entityType;
			var classDeliveryType = '';
			if(entityType == 'cre_sys_obt_cls'){
				var classDeliveryType = $('input[name="hidden_delivery_type_'+uniqueId+'"]').val();
			}

			var waitlistAvailable = 0;
			var enrolledAvailable = 0;
			var inprogressAvailable = 0;
			var noshowAvailable = 0;
			var cancelledAvailable = 0;
			var completedAvailable = 0;
			var alreadyenrolledstatus = 0;
			var incompleteAvailable = 0;
			var attendedAvailable = 0;
			// Already selected values that has been stored
			if(existingList != ''){
				var existingListArray = existingList.split(',');
				var existingListLen = existingListArray.length;
				var selectedId = '';
				for( var i = 0; i < existingListLen; i++) {
					existingListDetails = existingListArray[i];
					selectedValDetailsArr = existingListDetails.split('##');
					selectedId 				= selectedValDetailsArr[0];
					selectedStatusValue 	= selectedValDetailsArr[1];
					if($.inArray(selectedId, IdListArray) != -1){
						if(selectedStatusValue == 'Enrolled'){ enrolledAvailable = 1; }
						if(selectedStatusValue == 'Waitlist'){ waitlistAvailable = 1; }
						if(selectedStatusValue == 'In progress'){ inprogressAvailable = 1; }
						if(selectedStatusValue == 'Completed'){ completedAvailable = 1; }
						if(selectedStatusValue == 'Canceled'){ cancelledAvailable = 1; }
						if(selectedStatusValue == 'No Show'){ noshowAvailable = 1; }
						if(selectedStatusValue == 'Incomplete'){ incompleteAvailable = 1; }
						if(selectedStatusValue == 'Attended'){ attendedAvailable = 1; }

					}
				}
			}

			// Current grid values
			var selectedId = '';
			var selectedEnrolledStatus = '';
			var cancelledAllowed = 1;
			$('#datagrid-container-'+uniqueId+' .multiselect-singlecheck').each(function(){
				selectedId = $(this).val();
				if($(this).attr('checked') == true){
					selectedStatusValue = $('#enrolled_selected_'+selectedId+'-'+entityList).html();
					selectedEnrolledStatus = $('#enrolled_default_status_'+selectedId+'-'+entityList).val();
					regFromStatus = $('#enrolled_regfrom_status_'+selectedId+'-'+entityList).val();
					if(regFromStatus == 'TP' || regFromStatus == 'Cart'){
						cancelledAllowed = 0;
					}
					if(selectedStatusValue == 'Enrolled'){ enrolledAvailable = 1; }
					if(selectedStatusValue == 'Waitlist'){ waitlistAvailable = 1; }
					if(selectedStatusValue == 'In progress'){ inprogressAvailable = 1; }
					if(selectedStatusValue == 'Completed'){ completedAvailable = 1; }
					if(selectedStatusValue == 'Canceled'){ cancelledAvailable = 1; }
					if(selectedStatusValue == 'No Show'){ noshowAvailable = 1; }
					if(selectedStatusValue == 'Incomplete'){ incompleteAvailable = 1; }
					if(selectedStatusValue == 'Attended'){ attendedAvailable = 1; }
					if(selectedEnrolledStatus == 'Completed'){ alreadyenrolledstatus = 1; }
				}
			});

			$('.catalog-pub-add-list .completedandsave').show();
			$('.catalog-pub-add-list .enrolledandsave').show();
			if(cancelledAllowed){
				$('.catalog-pub-add-list .cancelledandsave').show();
			} else {
				$('.catalog-pub-add-list .cancelledandsave').hide();
			}
			if(classDeliveryType == 'lrn_cls_dty_vcl' || classDeliveryType == 'lrn_cls_dty_ilt'){
				$('.catalog-pub-add-list .noshowandsave').show();
			} else {
				$('.catalog-pub-add-list .noshowandsave').hide();
			}

			if(waitlistAvailable){
				$('.catalog-pub-add-list .completedandsave').hide();
				$('.catalog-pub-add-list .incompletedandsave').hide();
				// Enable the cancel action for waitlist cancellation - fix the ticket no:22426 by arun
			  //$('.catalog-pub-add-list .cancelledandsave').hide();
				$('.catalog-pub-add-list .noshowandsave').hide();
			}

			if(enrolledAvailable){
				$('.catalog-pub-add-list .enrolledandsave').hide();
			}

			if(incompleteAvailable){
				$('.catalog-pub-add-list .enrolledandsave').hide();
				$('.catalog-pub-add-list .cancelledandsave').hide();
				$('.catalog-pub-add-list .incompletedandsave').hide();
				$('.catalog-pub-add-list .noshowandsave').hide();
			}
			if(inprogressAvailable){
				$('.catalog-pub-add-list .enrolledandsave').hide();
				$('.catalog-pub-add-list .cancelledandsave').hide();
			}

			if(attendedAvailable){
				$('.catalog-pub-add-list .enrolledandsave').hide();
				$('.catalog-pub-add-list .cancelledandsave').hide();
				$('.catalog-pub-add-list .noshowandsave').hide();
			}

			if(completedAvailable){
				$('.catalog-pub-add-list .enrolledandsave').hide();
				$('.catalog-pub-add-list .cancelledandsave').hide();
				$('.catalog-pub-add-list .noshowandsave').hide();
			}

			if(noshowAvailable){
				$('.catalog-pub-add-list .enrolledandsave').hide();
				$('.catalog-pub-add-list .cancelledandsave').hide();
				$('.catalog-pub-add-list .completedandsave').hide();
			}

			if(cancelledAvailable){
				$('.catalog-pub-add-list .completedandsave').hide();
				$('.catalog-pub-add-list .incompletedandsave').hide();
				$('.catalog-pub-add-list .enrolledandsave').hide();
				$('.catalog-pub-add-list .noshowandsave').hide();
			}

			if(alreadyenrolledstatus){
				$('.catalog-pub-add-list .completedandsave').hide();
				$('.catalog-pub-add-list .incompletedandsave').hide();
				$('.catalog-pub-add-list .cancelledandsave').hide();
				$('.catalog-pub-add-list .enrolledandsave').hide();
				$('.catalog-pub-add-list .noshowandsave').hide();
				$('#admin-data-grid .completion_container_roster').hide();
			}
			}catch(e){
				// to do
			}
		},

		// Row edit option
		rowEditOptionDisplay : function(id, uniqueId){
			try{
			var value = $('#'+id).html();
			var txtInput = id.split('-');
			var malenghth = 4;
			if(txtInput[1] == 'assessment'){
				 malenghth = 2;
			}
			value = (value == '-' || value == Drupal.t('LBL1283')) ? '' : value;
			var inputHTML = '<input type="textbox" id="'+id+'" class="content-maxAttempt-validity-txtBox" value="'+value+'" size="10" maxlength="'+malenghth+'" onblur="$(\'body\').data(\'mulitselectdatagrid\').rowEditOptionSave(\''+id+'\', \''+uniqueId+'\');"  onKeyPress="return $(\'body\').data(\'mulitselectdatagrid\').onlyNumbers(event);" />';
			$('#div-'+id).html(inputHTML);
			$('#'+id).focus();
			}catch(e){
				// to do
			}
		},

		rowEditOptionSave : function(id, uniqueId,val){
			try{
			var value = $('#'+id).val().trim();
			var dbvalue;
			var txtInput = id.split('-');
			if(txtInput[4] == 'Horizontal' || txtInput[4] == 'Vertical'){
				dbvalue = val;
			}else{
				dbvalue = value == '-' ? -1 : value;
			}
			/*-- #37424: Unable to remove Content validity and attempts --*/
			if(txtInput[4] == 'max_attempts' || txtInput[4] == 'validity_days') {
				if(value == '' || (txtInput[1] == 'content' && value == Drupal.t('LBL1283')))
				dbvalue = 'NULL';
			}
			if(txtInput[4] == 'pre_status') {
				dbvalue = $('#'+id).attr('checked') == true ? 1 : 0;
			}
			if(txtInput[4] == 'preview_option') {
				dbvalue = $('#'+id).attr('checked') == true ? 1 : 0;
			}
			$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId);
			var url = this.constructUrl("administration/multiselect-grid/edit-option-save/"+id+"/"+dbvalue);
			setTimeout(function() {
				$.ajax({
					url : url,
					async: false,
					success: function(data) {
						if(txtInput[4] != 'pre_status' && txtInput[4] != 'preview_option' && txtInput[4] == 'no_of_attempts'){
							if(txtInput[1] == 'assessment' && dbvalue < 1) {
								var errorMsg = new Array();
								errorMsg[0] = Drupal.t('ERR245');
								var message_call = expertus_error_message(errorMsg, 'error');
								if(txtInput[3] != 'cre_sys_obt_cls'){
									$('#ctools-face-table #show_expertus_message').show();
									$('#ctools-face-table #show_expertus_message').html(message_call);
								}else {
									$('#bubble-face-table #show_expertus_message').show();
									$('#bubble-face-table #show_expertus_message').html(message_call);
								}
							}
						}
						if( txtInput[1] == 'content' && txtInput[4] ==  'max_attempts' ){
							// If the edited Attempts value is less than Lesson count means, its replace the original DB Attempts value
							value = data.max_attempts == null ? '' : data.max_attempts;
						}
						if( txtInput[1] == 'content' && txtInput[4] ==  'validity_days'){
							// If the edited Validity value is less than Lesson count means, its replace the original DB Attempts value
							value = data.validity_days == null ? '' : data.validity_days;
						}
						if(txtInput[1] == 'content' && value == '' && (txtInput[4] == 'max_attempts' || txtInput[4] == 'validity_days')) {
							value = Drupal.t('LBL1283');
						}
						  if(txtInput[1] == 'assessment' && txtInput[4] == 'pre_status'){ 
							
							  var ids = '';
							  var mid = $('#'+id).attr('data');
							  $(".multiselect-singlecheck").each(function(){
							          if($(this).attr("data") == mid && $(this).attr("id") != id){
							          	ids =  $(this).attr("id");
							          }
							  });
							  if(ids != ''){
								  if( dbvalue < 1){
		                                $('#'+id).attr('checked', false);
		                                $('#'+id).parent().attr('title',Drupal.t('LBL871')+" "+Drupal.t('Assessment'));
		                                $('#'+ids).attr('checked', true);                               
		                                $('#'+ids).parent().attr('title',Drupal.t('LBL1253')+" "+Drupal.t('Assessment'));
		                                
		                            }else{
		                                $('#'+id).attr('checked', true);                               
		                                $('#'+id).parent().attr('title',Drupal.t('LBL1253')+" "+Drupal.t('Assessment'));
		                                $('#'+ids).attr('checked', false);
		                                $('#'+ids).parent().attr('title',Drupal.t('LBL871')+" "+Drupal.t('Assessment'));
		                                
		                            }
							  }else{
	                            if( dbvalue < 1){
	                                $('#'+id).attr('checked', false);
	                                $('#'+id).parent().attr('title',Drupal.t('LBL871')+" "+Drupal.t('Assessment'));
	                            }else{
	                                $('#'+id).attr('checked', true);                               
	                                $('#'+id).parent().attr('title',Drupal.t('LBL1253')+" "+Drupal.t('Assessment'));
	                            }
							  }
	                        }
							var html = '<span class="row-edit-option" id="'+id+'" onClick="$(\'body\').data(\'mulitselectdatagrid\').rowEditOptionDisplay(\''+id+'\',\''+uniqueId+'\');">' + value + '</span>';
							$('#div-'+id).html(html);

						$('body').data('mulitselectdatagrid').destroyLoader('datagrid-div-'+uniqueId);
					}
				});
			}, 50);
			}catch(e){
				// to do
			}
		},

		retainCheckboxSelected : function(uniqueId){
			try{
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			var existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
			var type = uniqueId.split('-');
			if(existingList != '' && existingList != undefined){
				var existingListArray = existingList.split(',');
				var existingListLen = existingListArray.length;
				var selectedId = '';
				for( var i = 0; i < existingListLen; i++) {
					selectedId = existingListArray[i];
					if($('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).val() != 'undefined'){
						$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).attr('checked', true);
						if(this.currTheme == "expertusoneV2"){
							$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).parent().removeClass('checkbox-unselected');
							$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).parent().addClass('checkbox-selected');
						}

						if(type[0] == 'content') {	//retain attempts of content on page to page navigation
							if(document.getElementById('content-maxattempt-'+selectedId) !== undefined && document.getElementById('content-maxattempt-'+selectedId) !== null) {
				 				$('#content-maxattempt-'+selectedId).val($('#content-maxattempt-'+selectedId+'-hidden').val());
				 			}
						}
					}
				}
			}
			// If all checkbox selected - multiselect-all should be checked
			$('body').data('mulitselectdatagrid').retainSelectAllCheckbox(uniqueId);
			}catch(e){
				// to do
			}
		},

		retainSelectAllCheckbox : function(uniqueId){
			try{
			// If all checkbox selected - multiselect-all should be checked
			var multiselectVar = true;
			var removeClass = "checkbox-unselected";
			var addClass = "checkbox-selected";
			$('#datagrid-container-'+uniqueId+' .multiselect-singlecheck').each(function(){
				if($(this).attr('checked') == false){
					multiselectVar = false;
					removeClass = "checkbox-selected";
					addClass = "checkbox-unselected";
				}
			});
			$('#multiselect-selectall-'+uniqueId).attr('checked', multiselectVar);
			if($(this).attr('type')!='assessment'){
				$('#multiselect-selectall-'+uniqueId).parent().removeClass(removeClass);
				$('#multiselect-selectall-'+uniqueId).parent().addClass(addClass);
			}
			}catch(e){
				// to do
			}
		},

		userDisplayGridValues : function(cellvalue, options, rowObject){
			try{
		    var type = $('body').data('mulitselectdatagrid').type;
			var mode = $('body').data('mulitselectdatagrid').mode;
			var uniqueId = $('body').data('mulitselectdatagrid').uniqueId;
			var entityId = $('body').data('mulitselectdatagrid').entityId;
			var entityType = $('body').data('mulitselectdatagrid').entityType;
			var groupname  = '';
			var index  = options.colModel.index;
			var rowEditOptionId =  rowObject['id'] + '-' + type + '-' + entityId + '-' + entityType + '-' + index;
			if(index == 'MultiselectCheck'){
				var inputType = 'checkbox';
				var id = rowObject['MultiselectCheck'];
				return '<div class="checkbox-unselected"><input type="'+inputType+'" id="multiselect-singlecheck-'+uniqueId+'_'+id+'" value="'+id+'" class="multiselect-singlecheck" onclick="checkboxSelectedUnselectedCommon(this);" /></div>';
			}else if(index == 'user_name'){
					if(type == 'grpAddUsers' || type == 'RoleDisplayUsers' || type == 'RoleDisplayOwners'){
						var titleAllowed =  30 ;
					}
					else if(type == 'addUserOrder'){
						var titleAllowed =  18;
					}
					else{
						var titleAllowed =  20;
					}
					var userNameRestricted = $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index], 'user-grid-username');
					return '<span class="vtip" title="'+htmlEntities(rowObject[index])+'">'+userNameRestricted+'</span>';

		   }else if(index == 'name'){
			    if(type == 'grpAddOwners' || type == 'RoleDisplayUsers' || type == 'RoleDisplayOwners'){
					var titleAllowed =  20 ;
				}
				else if(type == 'addUserOrder'){
					var titleAllowed =  20;
				}
				else{
					var titleAllowed =  35;
				}
				var groupNameRestricted = $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index], 'owner-grid-group');
				return '<span class="vtip" title="'+rowObject[index]+'">'+groupNameRestricted+'</span>';

		   }else if(index == 'full_name'){
			    if(type == 'grpAddUsers' || type == 'RoleDisplayUsers' || type == 'RoleDisplayOwners'){
					var titleAllowed =  30 ;
				}
				else if(type == 'addUserOrder'){
					var titleAllowed =  20;
				}
				else{
					var titleAllowed =  35;
				}
				var fullNameRestricted = $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index], 'user-grid-fullname');
				return '<span class="vtip" title="'+htmlEntities(rowObject[index])+'">'+fullNameRestricted+'</span>';

			} else if(index == 'Remove') {
				var id = rowObject['id'];
				if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
					var esignObj = '';
		    		esignObj = "{'popupDiv':'ctools-face-table','esignFor':'DeleteFunctionality','type':'"+type+"','entityId':'"+entityId+"','entityType':'"+entityType+"','id':'"+id+"'}";
					return '<a href="JavaScript:void(0);" class="admin-pagination-action-link " onClick="$.fn.getNewEsignPopup('+esignObj+');">&nbsp;</a>';
				} else {
					return '<a href="JavaScript:void(0);" class="admin-pagination-action-link " onClick="$(\'body\').data(\'mulitselectdatagrid\').deleteEntity(\''+type+'\',\''+entityId+'\',\''+entityType+'\','+id+');">&nbsp;</a>';
				}
			}
			else if(index == 'AddUserOrder'){
		    	var drupalUId = rowObject['AddUserOrder'];
				return '<a href="JavaScript:void(0);" class="admin-select-user-order" onmousedown="adminUserOrderSelection(\''+drupalUId+'\');">'+Drupal.t('LBL674')+'</a>';
			}else {
				return rowObject[index] != 'undefined' ? rowObject[index] : '';
			}
			}catch(e){
				// to do
			}
		},

		displayGridValues : function(cellvalue, options, rowObject){
			try{
				
			var type = $('body').data('mulitselectdatagrid').type;
			var mode = $('body').data('mulitselectdatagrid').mode;
			var uniqueId = $('body').data('mulitselectdatagrid').uniqueId;
			var entityId = $('body').data('mulitselectdatagrid').entityId;
			var entityType = $('body').data('mulitselectdatagrid').entityType;
			var moduleId = rowObject['module'];
			var groupname  = '';
			var index  = options.colModel.index;
			var rowEditOptionId =  rowObject['id'] + '-' + type + '-' + entityId + '-' + entityType + '-' + index;
			if(index == 'MultiselectCheck'){

				var inputType = 'checkbox';
				var id = rowObject['MultiselectCheck'];
				if(type == 'assessment'){
					inputType = 'radio';
					return '<div class="checkbox-unselected"><input type="'+inputType+'" name= "assess-'+uniqueId+'" id="multiselect-singlecheck-'+uniqueId+'_'+id+'" value="'+id+'" class="multiselect-singlecheck" onclick="checkboxSelectedUnselectedCommon(this,\''+entityId+'\',\''+entityType+'\');"  /></div>';
				} else if(type == 'ContentMoveUsers' && rowObject['Status'].indexOf(Drupal.t("Completed")) >= 0){ 
					return '<div class="checkbox-unselected"><input type="'+inputType+'" id="multiselect-singlecheck-'+uniqueId+'_'+id+'" value="'+id+'" class="multiselect-singlecheck completed_sts" onclick="checkboxSelectedUnselectedCommon(this);" /></div>';
				}else {
					return '<div class="checkbox-unselected"><input type="'+inputType+'" id="multiselect-singlecheck-'+uniqueId+'_'+id+'" value="'+id+'" class="multiselect-singlecheck" onclick="checkboxSelectedUnselectedCommon(this);" /></div>';
				}

			}else if(index == 'pre_status') {
				var id = rowObject['MultiselectCheck'];
				if(type == 'assessment'){
					var inputType = 'checkbox';
					var checked = '';
					var vtip = Drupal.t('LBL871')+" "+Drupal.t('Assessment');
					if(rowObject['pre_status'] == 1){
						var checked = 'checked';
						var vtip = Drupal.t('LBL1253')+" "+Drupal.t('Assessment');
					}
					return '<div class="checkbox-unselected"><span title="'+vtip+'" class="vtip"><input type="'+inputType+'" name= "'+id+'-'+uniqueId+'-pre_status" '+checked+'  id="'+id+'-'+uniqueId+'-pre_status" value="'+id+'" data ="'+moduleId+'" class="multiselect-singlecheck " onclick="$(\'body\').data(\'mulitselectdatagrid\').rowEditOptionSave(this.id, \''+uniqueId+'\');"  /></span></div>';
				}

			}else if(index == 'preview_option') {
				var id = rowObject['MultiselectCheck'];
				if(type == 'assessment'){
					var inputType = 'checkbox';
					var checked = '';
					if(rowObject['preview_option'] == 1){
						var checked = 'checked';
					}
					return '<div class="checkbox-unselected"><input type="'+inputType+'" name= "'+id+'-'+uniqueId+'-preview_option" '+checked+'  id="'+id+'-'+uniqueId+'-preview_option" value="'+id+'" onclick="$(\'body\').data(\'mulitselectdatagrid\').rowEditOptionSave(this.id, \''+uniqueId+'\');"  /></div>';
				}
			} else if(index == 'Remove'){
				var id = rowObject['id'];
				if(type == 'trainings'){
					entityType = rowObject['DelType'];
				}
				if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
					var esignObj = '';
		    		esignObj = "{'popupDiv':'ctools-face-table','esignFor':'DeleteFunctionality','type':'"+type+"','entityId':'"+entityId+"','entityType':'"+entityType+"','id':'"+id+"'}";
					return '<a href="JavaScript:void(0);" class="admin-pagination-action-link " onClick="$.fn.getNewEsignPopup('+esignObj+');">&nbsp;</a>';

				}else{
					if((rowObject['ShowDelete'] != undefined) && (rowObject['ShowDelete'] != 'hide')) {
						return '';
					} else {
						return '<a href="JavaScript:void(0);" class="admin-pagination-action-link " onClick="$(\'body\').data(\'mulitselectdatagrid\').deleteEntity(\''+type+'\',\''+entityId+'\',\''+entityType+'\','+id+');">&nbsp;</a>';
					}
				}				
			} else if (type == 'survey' && index == 'survey_link' ) {
                var id = rowObject['id'];
                var entityId = $('body').data('mulitselectdatagrid').entityId;
                var entityType = $('body').data('mulitselectdatagrid').entityType;
                var fnobj = '';
                var popupWidth = '';
                
                if(Drupal.settings.user.language !== undefined && $.inArray(Drupal.settings.user.language, ["pt-pt", "de", "fr"]) != -1) {
    				popupWidth = 312;
    			} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ru') {
    				popupWidth = 347;
    			} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'es') {
    				popupWidth = 315;
    			} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it') {
    				popupWidth = 292;
    			} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ja') {
    				popupWidth = 321;
    			} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ko') {
    				popupWidth = 280;
    			} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'zh-hans') {
    				popupWidth = 281;
    			} else {
    				popupWidth = 288;
    			}
                fnobj = "{'entityId':'"+id+"','entityType':'"+entityType+"','url':'administration/survey/sharelink/single/"+entityId+"/"+entityType+"/"+id+"','popupDispId':'qtip_visible_disp_sharelink_"+id+"','catalogVisibleId':'qtip_visible_disp_sharelink_"+id+"','wid':'"+popupWidth+"','heg':'75','postype':'middle','poslwid':'','qdis':'','dispDown':'Y','linkid':'visible-sharelink-"+id+"'}";
                return '<div class="qtip_survey_sharelink" id="qtip_visible_disp_sharelink_'+id+'" style="position:relative;"><span class="vtip" title="'+Drupal.t('Share')+'"><a id="visible-sharelink-'+id+'" class="admin-survey-share-link" onClick="callVisibility('+fnobj+');">Sh</a><span class="narrow-search-results-item-detail-pipe-line sharelink-edit-delete-icons">|</span></span><span id="visible-popup-'+id+'" class="qtip-popup-visible"></span></div>';            
			} else if ((type == 'survey' ||  type == 'assessment') && index == 'view_option') {
				var id = rowObject['id'];
				var entityId = $('body').data('mulitselectdatagrid').entityId;
				var entityType = $('body').data('mulitselectdatagrid').entityType;
				var unique_id = rowObject['id'] + '-' + entityId + '-' + entityType;
					var listItems = '';
					var enrollSelection = '';
					if(rowObject['view_option'] != Drupal.t('LBL662')){	//'Horizontal'
						listItems = '<li data="Horizontal-'+unique_id+'" id="'+id+'-'+uniqueId+'-Horizontal"  onclick="$(\'body\').data(\'mulitselectdatagrid\').rowEditOptionSave(this.id, \''+uniqueId+'\',\''+id+'\');">'+Drupal.t('LBL662');+"</li>";
					}else if(rowObject['view_option'] != Drupal.t('LBL663')){	//'Vertical'
						listItems += '<li data="Vertical-'+unique_id+'" id="'+id+'-'+uniqueId+'-Vertical"  onclick="$(\'body\').data(\'mulitselectdatagrid\').rowEditOptionSave(this.id, \''+uniqueId+'\',\''+id+'\');">'+Drupal.t('LBL663');+"</li>";
			        }

					if(listItems != ''){
					  enrollSelection = '<span class="enrolled-selection "></span><ul class="sub-menu tp-assesement-view_opt  ">'+listItems+'</ul></ul>';
					}

				//var viewoption =  Drupal.t('Vertical');
					if(rowObject['view_option']){
						var viewoption =  rowObject['view_option'];
					}else{
						var viewoption =  Drupal.t('LBL663');
					}
				return '<span ><ul class="enrolled-status"><li><span class="selected-assesment-status" id="enrolled_selected_'+unique_id+'">'+viewoption+'</span>'+enrollSelection+'</span>';
				//return '<input type="radio" name="'+type+'-viewoption-'+id+'" id="'+type+'-viewoption-'+id+'" value="H" > <span class="vtip" title='+Drupal.t("LBL662")+'>'+Drupal.t('LBL662')+'</span><br/><input type="radio" name="'+type+'-viewoption-'+id+'" id="'+type+'-viewoption-'+id+'" value="V" checked> <span class="vtip" title='+Drupal.t("LBL663")+'>'+Drupal.t('LBL663')+'</span>';

			}else if ( type == 'assessment' && index == 'no_of_attempts') {
				var id = rowObject['id'];
					var maxAttemptVal = rowObject[index] == '-' ? ' ' : rowObject[index];
					return rowObject[index] != 'undefined' ? '<div id="div-'+rowEditOptionId+'"><span class="row-edit-option" id="'+rowEditOptionId+'" onClick="$(\'body\').data(\'mulitselectdatagrid\').rowEditOptionDisplay(\''+rowEditOptionId+'\', \''+uniqueId+'\');">' + maxAttemptVal + '</span></div>' : '';
			} 	

			else if ( type == 'content' && index == 'max_attempts') {
				var id = rowObject['id'];
				if(mode == 'edit') {
					return '<input type="text" class="input-box-rounded-corner" name="'+type+'-maxattempt-'+id+'" id="'+type+'-maxattempt-'+id+'" size ="5" maxlength="4" onKeyPress="return $(\'body\').data(\'mulitselectdatagrid\').onlyNumbers(event);" onBlur="$(\'body\').data(\'mulitselectdatagrid\').setContentAttempts(this);">';
				}else{
					var maxAttemptVal = rowObject[index] == '-' ? ' ' : rowObject[index];
					maxAttemptVal = maxAttemptVal == " " ? Drupal.t('LBL1283') : maxAttemptVal;
					return rowObject[index] != 'undefined' ? '<div id="div-'+rowEditOptionId+'"><span class="row-edit-option" id="'+rowEditOptionId+'" onClick="$(\'body\').data(\'mulitselectdatagrid\').rowEditOptionDisplay(\''+rowEditOptionId+'\', \''+uniqueId+'\');">' + maxAttemptVal + '</span></div>' : '';
				}
			} else if ( type == 'content' && index == 'validity_days') {
				var id = rowObject['id'];
				if(mode == 'edit') {
					return '<input type="text" class="input-box-rounded-corner" name="'+type+'-validitydays-'+id+'" id="'+type+'-validitydays-'+id+'" size ="5" maxlength="4" onKeyPress="return $(\'body\').data(\'mulitselectdatagrid\').onlyNumbers(event);">';
				} else {
					var validityDaysVal = rowObject[index] == '-' ? ' ' : rowObject[index];
					validityDaysVal = validityDaysVal == " " ? Drupal.t('LBL1283') : validityDaysVal; // #50062 Modified by joolavasavi
					return rowObject[index] != 'undefined' ? '<div id="div-'+rowEditOptionId+'"><span class="row-edit-option" id="'+rowEditOptionId+'" onClick="$(\'body\').data(\'mulitselectdatagrid\').rowEditOptionDisplay(\''+rowEditOptionId+'\', \''+uniqueId+'\');">' + validityDaysVal + '</span></div>' : '';
				}
			} else if ( type == 'content' && index == 'SequenceDrag') {
				var id = rowObject['id'];
				if(mode == 'edit' || (rowObject['ShowDelete'] > 0)){
					return '';
				} else{
					if((rowObject['countquery'] > 1)){

					return '<div class="dragndrop-selectable-item"><img src="/sites/all/themes/core/expertusone/expertusone-internals/images/drag.png"></div>';
				} else{
					return '';
				}
				}
		    } else if ( type == 'attachQuestion' && ((index == 'groupName') || (index == 'sequenceNo') || (index == 'score'))) {
				var id 			  = (mode == 'addmore') ? 700 : rowObject['id'];
				var validate_flag = eval(rowObject['validate_flag']);
				var applyStyle 	  = (validate_flag && (rowObject[index] == '')) ? "class='error';" : '';
				var applyVal   	  = rowObject[index] != '' ? rowObject[index] : '';
				var keyEventOn	  = 'onKeyPress="return $(\'body\').data(\'mulitselectdatagrid\').onlyNumbers(event);"';
				if(entityType == 'sry_det_typ_ass'){
					var onFocusEve    = 'onblur=\'textfieldTitleBlur(this,"Type a Question");\' onfocus=\'textfieldTitleClick(this, "Type a Question"); (function ($) {jQuery(".addedit-edit-attach-question-autocomplete").autocomplete("/?q=administration/learning/assessment/survey-assesment-autocomplete", {minChars: 3, max: 50, autoFill: true, mustMatch: false, matchContains: false, inputClass: "ac_input", loadingClass: "ac_loading", extraParams: {excluded_question_id: "1,2,3", id_required: 0}});})(jQuery);\' ';
				}
				else{
					var onFocusEve    = 'onblur=\'textfieldTitleBlur(this,"Type a Question");\' onfocus=\'textfieldTitleClick(this, "Type a Question"); (function ($) {jQuery(".addedit-edit-attach-question-autocomplete").autocomplete("/?q=administration/learning/survey/survey-assesment-autocomplete", {minChars: 3, max: 50, autoFill: true, mustMatch: false, matchContains: false, inputClass: "ac_input", loadingClass: "ac_loading", extraParams: {excluded_question_id: "1,2,3", id_required: 0}});})(jQuery);\' ';
				}
				var inputId	  	  = type+'-'+index+'-'+id;
				if((mode == 'edit') || (mode == 'addmore')) {
					if(index == 'groupName') {
						groupname = rowObject[index] == '' || rowObject[index] == undefined ? 'Group Name' : rowObject[index];
						groupGreyClass = groupname == 'Group Name' ? 'input-field-grey' : '';
						return '<div class="label-group-name" title="Enter Group Name">Enter Group Name :&nbsp;&nbsp;&nbsp;<input type="text" maxlength="50" size="30" value="'+groupname+'" name="'+inputId+'" id="'+inputId+'" onfocus="textfieldTitleClick(this, \'Group Name\'); (function ($) {jQuery(\'.addedit-survey-assess-group-autocomplete\').autocomplete(\'/?q=administration/survey-assessment/groupname-autocomplete\', {minChars: 3, max: 50, autoFill: true, mustMatch: false, matchContains: false, inputClass: \'ac_input\', loadingClass: \'ac_loading\'});})(jQuery);" onblur="textfieldTitleBlur(this, \'Group Name\');" class="addedit-survey-assess-group-autocomplete addedit-edit-ac-group-long-textfield ac_input '+groupGreyClass+' form-text" autocomplete="off"></div>';
					} else {
						return ((index == 'groupName') ? ('<div class="label-group-name" title="Enter Group Name">Enter Group Name : ') : '')+'<input type="text" '+((index == 'groupName') ? (onFocusEve+' size ="30"') : ' size ="5"')+' '+applyStyle+' class="input-box-rounded-corner" name="'+inputId+'" id="'+inputId+'" '+((index != 'groupName') ? keyEventOn : '')+' value="'+applyVal+'"> '+((index == 'groupName') ? ('</div>') : '');
					}
				} else {
					if(index == 'groupName') {
						groupname = rowObject[index] == '' || rowObject[index] == undefined ? 'Group Name' : rowObject[index];
						groupGreyClass = groupname == 'Group Name' ? 'input-field-grey' : '';
						return '<input type="text" maxlength="50" size="30" value="'+groupname+'" name="'+inputId+'" id="'+inputId+'" onfocus="textfieldTitleClick(this, \'Group Name\'); (function ($) {jQuery(\'.addedit-survey-assess-group-autocomplete\').autocomplete(\'/?q=administration/survey-assessment/groupname-autocomplete\', {minChars: 3, max: 50, autoFill: true, mustMatch: false, matchContains: false, inputClass: \'ac_input\', loadingClass: \'ac_loading\'});})(jQuery);" onblur="textfieldTitleBlur(this, \'Group Name\');" class="addedit-survey-assess-group-autocomplete addedit-edit-ac-group-textfield ac_input '+groupGreyClass+' form-text" autocomplete="off">';
					} else {
						return '<input type="text" '+((index == 'groupName') ? (onFocusEve+' size ="13"') : 'size ="5"')+' '+applyStyle+' '+((index != 'groupName') ? keyEventOn : '')+' name="'+inputId+'" class="input-box-rounded-corner" id="'+inputId+'" value="'+rowObject[index]+'">';
					}
				}
			}
		    else if(type == 'addProductOrder'){
		    	var titleAllowed = 12;
		    	uniqueName = 'class-code';
		    	var sessionLocationDet = rowObject[index];
		    	var widObj = $('body').data('mulitselectdatagrid');
		    	if(index == 'AddProductOrder'){
		    		titleAllowed = 13;
			    	var addProductArray = rowObject['AddProductOrder'];
			    	if(addProductArray['canAdded'] == '1'){
			    	  var nId  = addProductArray['NodeId'];
			    	  if(addProductArray['ObjectType'] == 'Class'){
			    		return '<a href="JavaScript:void(0);" class="admin-select-user-order vtip" onmousedown="adminProductOrderSelection(\''+nId+'\');" title="'+Drupal.t('LBL050')+'">'+widObj.titleRestrictionFadeoutImage(Drupal.t('LBL050'), 'add-product-to-cart')+'</a>';
			    	  }
			    	  else if (addProductArray['ObjectType'] == 'cre_sys_obt_crt' || addProductArray['ObjectType'] == 'cre_sys_obt_trn' || addProductArray['ObjectType'] == 'cre_sys_obt_cur'){
 			    		if(addProductArray['RegStatus'] == 'multiregister'){
 			    			var entityId  = addProductArray['PrgId'];
			    			var uniqueId  = "addordertpuser-"+entityId;
			    			var userIdList= addProductArray['RegUserId'];
			    		    return  '<div id="'+uniqueId+'"></div><a href="JavaScript:void(0);" class="admin-select-user-order vtip" onClick="$(\'input[name=form_selected_nid]:hidden\').val(\''+nId+'\');$(\'body\').data(\'adminlearningcore\').adminOrderMultiClassQTip(\'' + entityId + '\',\'' + uniqueId + '\', \''+ userIdList + '\'); return false;" title="'+Drupal.t('LBL050')+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL050'), 'multi-register-status')+'</a>';
			    		}
			    		else{
   		    			  return '<a href="JavaScript:void(0);" class="admin-select-user-order vtip" onmousedown="adminProductOrderSelection(\''+nId+'\');" title="'+Drupal.t('LBL050')+'">'+widObj.titleRestrictionFadeoutImage(Drupal.t('LBL050'), 'user-select-order')+'</a>';
			    		}
			    	  }
			    	}
			    	else{
			    	  var labelTxt = (addProductArray['is_cart_added'] == 1) ? Drupal.t('LBL049') : Drupal.t('LBL050');
		              var selectLable = (Drupal.settings.ajaxPageState.theme == "expertusoneV2") ? '<span class="unselect-app-product vtip" title="'+labelTxt+'">' + widObj.titleRestrictionFadeoutImage(labelTxt, 'order-added-to-cart') + '</span>' : '<span class="vtip" title="'+labelTxt+'">' + titleRestrictionFadeoutImage(labelTxt, 'order-added-to-cart') + '</span>';
		              return selectLable;
			    	}
		    	}
		    	else if(index == 'cls_title'){
		    		titleAllowed =  17;
		    		uniqueName = "class-title";
		    	}
		    	else if(index == 'delivery_type_name'){
		    		titleAllowed =  14;
		    		uniqueName = "delivery-type";
		    		var sessionLocationDet = rowObject['locationSessionDet'];
		    	}
		    	else if(index == 'price'){
		    		titleAllowed =  8;
		    		uniqueName = "product-price";
		    	}
		    	else if(index == 'Status'){
		    		titleAllowed =  8;
		    		uniqueName = "product-status";
		    	}
		    	else if(index == 'language'){
		    		titleAllowed =  10;
			    	var fullNameRestricted = $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(Drupal.t(rowObject[index]), 'language');
			    	return '<span class="vtip" title="'+ Drupal.t(rowObject[index]) +'">'+fullNameRestricted+'</span>';
		    	}

		    	var fullNameRestricted = $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index], uniqueName);
		    	if(index == 'cls_title') {
		    		return '<span class="vtip" title="'+ sessionLocationDet +'">'+fullNameRestricted+'</span>';
		    	}
		    	else {
		    		return '<span class="vtip" title="'+ htmlEntities(sessionLocationDet) +'">'+fullNameRestricted+'</span>';
		    	}
		     }
		    else if(type == 'content' && index == 'total_lesson'){
		    	return '<span class="lesson_text">'+rowObject[index]+'</span>';
		    }
		    else {
				return rowObject[index] != 'undefined' ? rowObject[index] : '';
			}
			}catch(e){
				// to do
			}
		},

		afterInsertRow : function(rowid,rowdata,rowelem){
			try{
            var type = $('body').data('mulitselectdatagrid').type;
            var uniqueId = $('body').data('mulitselectdatagrid').uniqueId;
            var entityId = $('body').data('mulitselectdatagrid').entityId;
            var entityType = $('body').data('mulitselectdatagrid').entityType;
            var list = $('input[name=hidden_idlist_'+uniqueId+']:hidden').val();
            list = (list.indexOf('-')>0)?list.split(','):'';
            var id = rowelem.id + '-' + entityId + '-' + entityType;
            if(list != ''){
	            for(var i=0; i < list.length; i++){
	                var op = list[i].split('-');
	                if(op[0]==rowelem.id){
	                    if(type=="survey"){
                            $("#multiselect-singlecheck-"+uniqueId+'_'+rowelem.id).parent().removeClass('checkbox-unselected');
                            $("#multiselect-singlecheck-"+uniqueId+'_'+rowelem.id).parent().addClass('checkbox-selected');
	                    }else{
                            $('#datagrid-container-'+uniqueId+' .checkbox-selected').removeClass('checkbox-selected')
	                    }
	                    $("#multiselect-singlecheck-"+uniqueId+'_'+rowelem.id).attr('checked', true);
	                    if(op[1]=='V'){
	                        $("#enrolled_selected_"+id).html(Drupal.t('LBL663'));
	                    	$('#'+uniqueId+'-Vertical').css('display','none');
							$('#'+uniqueId+'-Horizontal').css('display','block');
	                    }else{
	                        $("#enrolled_selected_"+id).html(Drupal.t('LBL662'));
	                       	$('#'+uniqueId+'-Horizontal').css('display','none');
							$('#'+uniqueId+'-Vertical').css('display','block');
	                    }
	                }
	            }
            }
			}catch(e){
				// to do
			}
		},

		enrollUserDisplayGridValues : function(cellvalue, options, rowObject){
			try{
			var type = $('body').data('mulitselectdatagrid').type;
			var mode = $('body').data('mulitselectdatagrid').mode;
			var uniqueId = $('body').data('mulitselectdatagrid').uniqueId;
			var entityId = $('body').data('mulitselectdatagrid').entityId;
			var entityType = $('body').data('mulitselectdatagrid').entityType;
			var groupname  = '';
			var index  = options.colModel.index;
			var uniqueId2 = rowObject['id'] + '-' + entityId + '-' + entityType;
			var rowEditOptionId =  rowObject['id'] + '-' + type + '-' + entityId + '-' + entityType + '-' + index;
			//Dummy declaration for translation
			var dstatus1 = Drupal.t('Enrolled');
			var dstatus2 = Drupal.t('Canceled');
			var dstatus3 = Drupal.t('Completed');
			var dstatus4 = Drupal.t('Incomplete');
			var dstatus5 = Drupal.t('No Show');
			var dstatus6 = Drupal.t('Expired');
			var dstatus7 = Drupal.t('Waitlist');
			var dstatus8 = Drupal.t('In progress');
			var dstatus9 = Drupal.t('Pending');
			var dstatus10 = Drupal.t('Attended');
			var dstatus11 = Drupal.t('Waived');
			if(index == 'MultiselectCheck'){
				var id = rowObject['MultiselectCheck'];
				var disabledStatus = '';
				if( (rowObject['Status'] == 'Completed' && rowObject['score'] != '' && rowObject['score'] != null ) || ( rowObject['Status'] == 'No Show' || rowObject['Status'] == 'Pending' || rowObject['Status'] == 'Canceled' || rowObject['Status'] == 'Expired' || rowObject['Status'] == 'Incomplete')){
					disabledStatus = 'disabled="disabled"';
				} else if(type == 'enrolltpuser' && rowObject['Status'] == 'Completed'){
					disabledStatus = 'disabled="disabled"';
				}
				return '<div class="checkbox-unselected"><input type="checkbox" '+disabledStatus+'  id="multiselect-singlecheck-'+uniqueId+'_'+id+'" value="'+id+'" class="multiselect-singlecheck" onclick="checkboxSelectedUnselectedCommon(this);"/></div>';

			} else if(index == 'Status'){
				var id = rowObject['id'];
				var rowNumber = rowObject['row_number'];
				var having_waived = rowObject['having_waived'];
				var listItems = '';
				var enrollSelection = '';				
				if(rowObject[index] != 'Completed' && rowObject[index] != 'No Show' && rowObject[index] != 'Canceled' && rowObject[index] != 'Cancelled' && rowObject[index] != 'Cancelled' && rowObject[index] != 'Reserved' && rowObject[index] != 'Pending' && rowObject[index] != 'Expired' && rowObject[index] != 'Incomplete'){
					if(rowObject[index] == 'Enrolled'){
						if(rowObject['exempted_sts'] == 1){
							if((rowObject['RegFrom'] == Drupal.t('Class')  || rowObject['RegFrom'] == 'Cart' || rowObject['RegFrom'] == '') && (type == 'enrolluser'  || type == 'shareuser' || type == 'enrolltpuser')) {
								  listItems = '<li data="Canceled-'+uniqueId2+'">'+Drupal.t('Canceled')+"</li>";
							}
							if(rowObject['SessionStarted'] == true) {
								listItems += '<li data="Completed-'+uniqueId2+'">'+Drupal.t('Completed')+"</li>";
							}
							listItems += '<li data="Enrolled-'+uniqueId2+'-'+rowNumber+'">'+Drupal.t('Enrolled')+"</li>";
						}else{
							// Added by velu for #0023621
							if((rowObject['RegFrom'] == Drupal.t('Class') || rowObject['RegFrom'] == 'Cart' || rowObject['RegFrom'] == '') && (type == 'enrolluser'  || type == 'shareuser' || type == 'enrolltpuser')) {
								  listItems = '<li data="Canceled-'+uniqueId2+'">'+Drupal.t('Canceled')+"</li>";
							}
							// #0016788 - Classroom Session should not be able to mark "Complete" or "No Show"  for future sessions - Implemented
							if(rowObject['SessionStarted'] == true) {
								listItems += '<li data="Completed-'+uniqueId2+'">'+Drupal.t('Completed')+"</li>";
								if(rowObject['DeliveryType'] == 'lrn_cls_dty_vcl' ||  rowObject['DeliveryType'] == 'lrn_cls_dty_ilt'){
									listItems += '<li data="No Show-'+uniqueId2+'">'+Drupal.t('No Show')+"</li>";
								}
								listItems += '<li data="Incomplete-'+uniqueId2+'">'+Drupal.t('Incomplete')+"</li>";
							}
							//listItems += '<li data="In Complete-'+uniqueId2+'">In Complete</li>';
							if((rowObject['is_mand'] == 1 || rowObject['is_compliance'] == 1) && (type == 'enrolluser'  || type == 'shareuser' || type == 'enrolltpuser')) {
								  listItems += '<li data="Waived-'+uniqueId2+'-'+rowNumber+'">'+dstatus11+"</li>";
							}
						}
						
					} else if(rowObject[index] == 'In progress'){
						
						if(rowObject['exempted_sts'] == 1){
							if(rowObject['RegFrom'] == Drupal.t('Class') || (type == 'enrolltpuser' && rowObject['RegFrom'] != 'Cart')) {
								listItems = '<li data="Canceled-'+uniqueId2+'">'+Drupal.t('Canceled')+"</li>";
							}
							listItems += '<li data="Completed-'+uniqueId2+'">'+Drupal.t('Completed')+"</li>";
							listItems += '<li data="In progress-'+uniqueId2+'">'+Drupal.t('In progress')+"</li>";
						}else{
							if(rowObject['RegFrom'] == Drupal.t('Class') || (type == 'enrolltpuser' && rowObject['RegFrom'] != 'Cart')) {
								listItems = '<li data="Canceled-'+uniqueId2+'">'+Drupal.t('Canceled')+"</li>";
							}
							listItems += '<li data="Completed-'+uniqueId2+'">'+Drupal.t('Completed')+"</li>";
							if(rowObject['DeliveryType'] == 'lrn_cls_dty_vcl' ||  rowObject['DeliveryType'] == 'lrn_cls_dty_ilt'){
								listItems += '<li data="No Show-'+uniqueId2+'">'+Drupal.t('No Show')+"</li>";
							}
							listItems += '<li data="Incomplete-'+uniqueId2+'">'+Drupal.t('Incomplete')+"</li>";
							
							if((rowObject['is_mand'] == 1 || rowObject['is_compliance'] == 1) && (type == 'enrolluser'  || type == 'shareuser' || type == 'enrolltpuser')) {
								  listItems += '<li data="Waived-'+uniqueId2+'-'+rowNumber+'">'+dstatus11+"</li>";
							}
						}
						
					} else if(rowObject[index] == 'Waitlist'){
						listItems = '<li data="Enrolled-'+uniqueId2+'">'+Drupal.t('Enrolled')+"</li>";
						if(rowObject['RegFrom'] == Drupal.t('Class') || (type == 'enrolltpuser' && rowObject['RegFrom'] != 'Cart')) {
							listItems += '<li data="Canceled-'+uniqueId2+'">'+Drupal.t('Canceled')+"</li>";
						}
					} else if(rowObject[index] == 'Incomplete'){
						listItems = '<li data="Enrolled-'+uniqueId2+'-'+rowNumber+'">'+Drupal.t('Enrolled')+"</li>";
					} else if(rowObject[index] == 'Attended'){
							listItems = '<li data="Completed-'+uniqueId2+'">'+Drupal.t('Completed')+'</li><li data="Incomplete-'+uniqueId2+'">'+Drupal.t('Incomplete')+'</li>';
					} else {
						listItems = '<li data="Enrolled-'+uniqueId2+'-'+rowNumber+'">'+Drupal.t('Enrolled')+"</li>";
						if(rowObject['RegFrom'] == Drupal.t('Class') || (type == 'enrolltpuser' && rowObject['RegFrom'] != 'Cart')) {
							listItems += '<li data="Canceled-'+uniqueId2+'">'+Drupal.t('Canceled')+"</li>";
						}
						listItems += '<li data="Completed-'+uniqueId2+'">'+Drupal.t('Completed')+"</li>";
						if(rowObject['DeliveryType'] == 'lrn_cls_dty_vcl' ||  rowObject['DeliveryType'] == 'lrn_cls_dty_ilt'){
							listItems += '<li data="No Show-'+uniqueId2+'">'+Drupal.t('No Show')+"</li>";
						}
					}
					if(listItems != ''){
					  enrollSelection = '<span class="enrolled-selection "></span><ul class="sub-menu">'+listItems+'</ul></ul>';
					}
				}
				var recertify = '';
				if(entityType != 'cre_sys_obt_cls' && rowObject['certifypath'] > 1){
						var wid = rowObject['id'];
						var status = '+';
						var exmpType = (type == 'enrolltpuser') ? 'tp' : 'class';
						var waivedlinkHTML = ''
						var recertifyurl = 'administration/training-plan-enroll/recertify-history/' + entityId + '/'+entityType+ '/'+id;
						
						var qtipRecertifyObj  = "{'entityId' : '"+ wid + "'"+
						", 'entityType' : '"+entityType +"'" +
						", 'url' : '"+ recertifyurl + "'" +
						", 'popupDispId' : 'enrolled-history-"+wid + "'"+
						", 'catalogVisibleId' : 'qtipAttachIdqtip_history_" + wid + "'" +
						", 'wid' : 600"+", 'heg' : 60" +", 'postype' : 'bottomright'" +",'poslwid' : 150 "+", 'qdis' : 'ctool'" +
						", 'linkid' : 'visible-history-"+ wid+"','dispDown' : 'Y'"+", 'enrId' : '"+entityId +"'}";	
				
						waivedlinkHTML = 
							'<span class="enrolled-history-container">' +
							'<span id="enrolled-history-' + wid + '" class="enrolled-history-status">' +
							'<a id="visible-history-'+wid+'" class="enrolled-history-status-link" onclick = "callVisibility(' + qtipRecertifyObj + ');">  </a>' +
							'<span id="visible-popup-' + id + '" class="qtip-popup-history" style="display:none; position:absolute; left:0px; top:0px;" ></span>'+
							'</span>' +
							'</span>';
						
					recertify  = '</li></ul><span  class = "recertify-history" ><span class="selected-enrolled-status" id="enrolled_selected_'+uniqueId2+'">'+waivedlinkHTML+'</span></span>';
					}
				
				if(rowObject['exempted_sts'] == 1 && (rowObject[index] == 'Enrolled' || rowObject[index] == 'In progress')){
					var wid = rowObject['id'];
					var status = Drupal.t('Waived');
					var exmpType = (type == 'enrolltpuser') ? 'tp' : 'class';
					var waivedlinkHTML = '';//'administration/enrollment/exempted/single/" + wid + "/"+exmpType+"/" + rowObject['exempted_id']+ "'"
					var waivedurl = 'administration/enrollment/exempted/view/'+entityId +'/' + id + '/'+exmpType +'/'+rowNumber;
					if(rowNumber > 5) {
						var qtipOptWaivedObj  = "{'entityId' : '"+ wid + "'"+
						", 'entityType' : '"+entityType +"'" +
						", 'url' : '"+ waivedurl + "'" +
						", 'popupDispId' : 'enrolled-exempted-"+wid + "'"+
						", 'catalogVisibleId' : 'qtipAttachIdqtip_exempted_disp_all'" +
						", 'wid' : 460"+", 'heg' : 150" +", 'postype' : 'bottomright'" +",'poslwid' : 70 "+", 'qdis' : 'ctool'" +
						", 'linkid' : 'visible-exempted-"+ wid+"','dispDown' : ''"+", 'enrId' : '"+entityId +"'}";	
						
					}else {
						var qtipOptWaivedObj  = "{'entityId' : '"+ wid + "'"+
						", 'entityType' : '"+entityType +"'" +
						", 'url' : '"+ waivedurl + "'" +
						", 'popupDispId' : 'enrolled-exempted-"+wid + "'"+
						", 'catalogVisibleId' : 'qtipAttachIdqtip_pwdstrength_visible_disp_" + wid + "'" +
						", 'wid' : 460"+", 'heg' : 150" +", 'postype' : 'bottomright'" +",'poslwid' : 70 "+", 'qdis' : 'ctool'" +
						", 'linkid' : 'visible-exempted-"+ wid+"','dispDown' : 'Y'"+", 'enrId' : '"+entityId +"'}";	
					}
					waivedlinkHTML = 
						'<span class="enrolled-exempted-container">' +
						'<span id="enrolled-exempted-' + wid + '" class="enrolled-exempted-status">' +
						'<a id="visible-exempted-'+wid+'" class="enrolled-exempted-status-link" onclick = "exemptedVisibility(' + qtipOptWaivedObj + ');">' +
						titleRestrictionFadeoutImage(status, 'view-status-fadeout-container') +
						'</a>' +
						'<span id="visible-popup-' + id + '" class="qtip-popup-exempted" style="display:none; position:absolute; left:0px; top:0px;" ></span>'+
						'</span>' +
						'</span>';
					//$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).removeClass('vtip');
					return '<span ><input type="hidden" id="enrolled_default_status_'+uniqueId2+'"name="enrolled_default_status_'+wid+'" value="'+status+'" /><input type="hidden" name="enrolled_status_'+wid+'" id="enrolled_status_'+uniqueId2+'" value="'+status+'" /><ul class="enrolled-status"><li><span class="selected-enrolled-status" id="enrolled_selected_'+uniqueId2+'">'+waivedlinkHTML+'</span>'+enrollSelection + ' ' +recertify+'</span>';
					}else{
					var status = (rowObject[index] == 'Registered') ? Drupal.t('Enrolled') : Drupal.t(rowObject[index]);
					if(having_waived == 1){					
					var wid = rowObject['id'];					
					var exmpType = (type == 'enrolltpuser') ? 'tp' : 'class';
					var waivedlinkHTML = '';//'administration/enrollment/exempted/single/" + wid + "/"+exmpType+"/" + rowObject['exempted_id']+ "'"
					var waivedurl = 'administration/enrollment/exempted/view/'+entityId +'/' + id + '/'+exmpType +'/'+rowNumber;
					if(rowNumber > 5) {
						var qtipOptWaivedObj  = "{'entityId' : '"+ wid + "'"+
						", 'entityType' : '"+entityType +"'" +
						", 'url' : '"+ waivedurl + "'" +
						", 'popupDispId' : 'enrolled-exempted-"+wid + "'"+
						", 'catalogVisibleId' : 'qtipAttachIdqtip_exempted_disp_all'" +
						", 'wid' : 460"+", 'heg' : 150" +", 'postype' : 'bottomright'" +",'poslwid' : 70 "+", 'qdis' : 'ctool'" +
						", 'linkid' : 'visible-exempted-"+ wid+"','dispDown' : ''"+", 'enrId' : '"+entityId +"'}";	
						
					}else {
						var qtipOptWaivedObj  = "{'entityId' : '"+ wid + "'"+
						", 'entityType' : '"+entityType +"'" +
						", 'url' : '"+ waivedurl + "'" +
						", 'popupDispId' : 'enrolled-exempted-"+wid + "'"+
						", 'catalogVisibleId' : 'qtipAttachIdqtip_pwdstrength_visible_disp_" + wid + "'" +
						", 'wid' : 460"+", 'heg' : 150" +", 'postype' : 'bottomright'" +",'poslwid' : 70 "+", 'qdis' : 'ctool'" +
						", 'linkid' : 'visible-exempted-"+ wid+"','dispDown' : 'Y'"+", 'enrId' : '"+entityId +"'}";	
					}
					waivedlinkHTML = 
						'<span class="enrolled-exempted-container">' +
						'<span id="enrolled-exempted-' + wid + '" class="enrolled-exempted-status">' +
						'<a id="visible-exempted-'+wid+'" class="enrolled-exempted-status-link" onclick = "exemptedVisibility(' + qtipOptWaivedObj + ');">' +
						titleRestrictionFadeoutImage(status, 'view-status-fadeout-container') +
						'</a>' +
						'<span id="visible-popup-' + id + '" class="qtip-popup-exempted" style="display:none; position:absolute; left:0px; top:0px;" ></span>'+
						'</span>' +
						'</span>';
					$('#enrolled_selected_'+id+'-'+entityId+'-'+entityType).removeClass('vtip');
					return '<span ><input type="hidden" id="enrolled_default_status_'+uniqueId2+'" name="enrolled_default_status_'+wid+'" value="'+status+'" /><input type="hidden" name="enrolled_status_'+wid+'" id="enrolled_status_'+uniqueId2+'" value="'+status+'" /><ul class="enrolled-status"><li><span class="selected-enrolled-status" id="enrolled_selected_'+uniqueId2+'">'+waivedlinkHTML+'</span>'+enrollSelection + ' ' +recertify+'</span>';
					
					}else {
						return '<span ><input type="hidden" id="enrolled_default_status_'+uniqueId2+'" name="enrolled_default_status_'+rowObject['id']+'"value="'+status+'" /><input type="hidden" name="enrolled_status_'+rowObject['id']+'" id="enrolled_status_'+uniqueId2+'" value="'+status+'" /><ul class="enrolled-status"><li><span class="selected-enrolled-status" id="enrolled_selected_'+uniqueId2+'">'+status+'</span>'+enrollSelection + ' ' +recertify+'</span>';
					}
				}
		

			} else if(index == 'Remove') {

				var id = rowObject['id'];
				if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
					var esignObj = '';
		    		esignObj = "{'popupDiv':'ctools-face-table','esignFor':'DeleteFunctionality','type':'"+type+"','entityId':'"+entityId+"','entityType':'"+entityType+"','id':'"+id+"'}";
					return '<a href="JavaScript:void(0);" class="admin-pagination-action-link " onClick="$.fn.getNewEsignPopup('+esignObj+');">&nbsp;</a>';
				} else {
					return '<a href="JavaScript:void(0);" class="admin-pagination-action-link " onClick="$(\'body\').data(\'mulitselectdatagrid\').deleteEntity(\''+type+'\',\''+entityId+'\',\''+entityType+'\','+id+');">&nbsp;</a>';
				}
				
			}else if(index == 'name'){  // For h5pcustomization.
				   var fullLen = rowObject[index].length;
					if(mode == 'view'){
								if(type == 'shareuser' ){
								var groupNameRestricted = (fullLen > 15)? $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index],'view-enrolluser-fullname'):rowObject[index];
							}else{
								var titleAllowed = 14;
								var groupNameRestricted = (fullLen > 17)? $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index],'view-fullname'):rowObject[index];
								}
	                       }else if(mode == 'edit'){
						var titleAllowed =15;
						var groupNameRestricted = (fullLen > 15)? $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index],'edit-fullname'):rowObject[index];
					}
	         return '<span class="vtip" title="'+htmlEntities(rowObject[index])+'">'+groupNameRestricted+'</span>';
				
			
			

			} else if(index == 'score') {
				var score = (rowObject['Score'] == undefined) ? '' : rowObject['Score'];
				var disabledStatus = '';
				if( (rowObject['Status'] == 'Completed' && rowObject['score'] != '' && rowObject['score'] != null ) || ( rowObject['Status'] == 'No Show' || rowObject['Status'] == 'Canceled' )  ){
					disabledStatus = 'readonly="readonly"';
				}
				if(rowObject['exempted_sts'] == 1 && (rowObject['Status']  == 'Enrolled' || rowObject['Status']  == 'In progress')) {
				   disabledStatus = 'readonly="readonly"';
				}
				var id = rowObject['id'];
				if(score == '' || (rowObject['Status'] != 'No Show' && rowObject['Status'] != 'Canceled' && score == 0)){
					return '<input type="text" size="3" maxlength="3" name="score_'+rowObject['id']+'" id="score_'+uniqueId2+'" data1="'+uniqueId+'_'+id+'" data2="'+uniqueId2+'" class="enroll-user-competion-score" value="" size ="5" '+disabledStatus+' onKeyPress="return $(\'body\').data(\'mulitselectdatagrid\').onlyNumbers(event);">';
				} else {
					//h5pcustomize added Incomplete #74257 - check with Vincent
					if(rowObject['Status'] == 'No Show' || rowObject['Status'] == 'Incomplete'){
						return '<span id="score_entered_'+uniqueId2+'"></span>';
					} else {
						
						return '<span id="score_entered_'+uniqueId2+'">'+score+'</span>';
					}
				}

			} else if(index == 'Date') {

				var defaultDate = rowObject[index];
				return '<div id="default_status_date_'+uniqueId2+'">'+defaultDate+'</div><input type="hidden" id="'+uniqueId2+'" value="'+defaultDate+'" /><input style="display:none;" size="8" type="text" name="completion_sel_date_'+rowObject['id']+'" id="completion_sel_date_'+uniqueId2+'" class="enroll-date-input-txtBox" />';

			} else if(index == 'user_name'){
				var userLen = rowObject[index].length;
				if(mode == 'view'){
					if(type == 'enrolluser'  || type == 'shareuser' ){
						var titleAllowed = 8;
						//var userNameRestricted = (userLen > 8)? $('body').data('mulitselectdatagrid').titleRestricted(rowObject[index], titleAllowed) : rowObject[index];
						var userNameRestricted = (userLen > 8)? $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index], 'view-enrolluser-username') : rowObject[index];
					}else{
						var titleAllowed = 9;
						var userNameRestricted = (userLen > 9)? $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index], 'view-username') : rowObject[index];
						userNameRestricted    += '<input type="hidden" id="enrolled_regfrom_status_'+uniqueId2+'" value="'+rowObject['RegFrom']+'" />';
					}
				}else if(mode == 'edit'){
					var titleAllowed = 15;
					var userNameRestricted = (userLen > 15)? $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index], 'edit-username') : rowObject[index];
				}
				return '<span class="vtip" title="'+htmlEntities(rowObject[index])+'">'+userNameRestricted+'</span><input type="hidden" id="enroll_user_'+rowObject['id']+'" value="'+rowObject[index]+'">';
			} else if(index == 'full_name'){
			  var fullLen = rowObject[index].length;
				if(mode == 'view'){
						if(type == 'enrolluser'  || type == 'shareuser' ){
//						 	var titleAllowed = 15;
//							var fullNameRestricted = (fullLen > 15)? $('body').data('mulitselectdatagrid').titleRestricted(rowObject[index], titleAllowed):rowObject[index];
							var fullNameRestricted = (fullLen > 15)? $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index],'view-enrolluser-fullname'):rowObject[index];
						}else{
							var titleAllowed = 14;
							var fullNameRestricted = (fullLen > 17)? $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index],'view-fullname'):rowObject[index];
							}
				}else if(mode == 'edit'){
					var titleAllowed =15;
					var fullNameRestricted = (fullLen > 15)? $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index],'edit-fullname'):rowObject[index];
				}
				return '<span class="vtip" title="'+htmlEntities(rowObject[index])+'">'+fullNameRestricted+'</span>';
			} else if(index == 'RegFrom'){
				if(rowObject[index]){
					  var titleAllowed = (type == 'enrolluser'  || type == 'shareuser' ) ? 10 : 25;
					  var registerFrom = (rowObject[index] == 'Cart') ? 'Class' : rowObject[index];
					  var regFromRestricted = $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(registerFrom, 'view-reg-from-fadeout-container');
					  return '<span class="vtip" title="'+htmlEntities(registerFrom)+'">'+regFromRestricted+'</span> <input type="hidden" id="enrolled_regfrom_status_'+uniqueId2+'" value="'+rowObject[index]+'" />';
				}

			} else if(index == 'AddToTemplate'){
				var id = rowObject['id'];
				var kwdName = rowObject['keywords'];
				if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
					var esignObj = '';
		    		//esignObj = "{'popupDiv':'ctools-face-table','esignFor':'DeleteFunctionality','type':'"+type+"','entityId':'"+entityId+"','entityType':'"+entityType+"','id':'"+id+"'}";
					esignObj = "{'popupDiv':'ctools-face-table','esignFor':'AddKeywordToTemplate','type':'"+type+"','entityId':'"+entityId+"','entityType':'"+entityType+"','id':'"+id+"','kwdName':'"+kwdName+"'}";
					//admin-pagination-action-link
					return '<a href="JavaScript:void(0);" class="admin-insert-to-template-keyword" onClick="$.fn.getNewEsignPopup('+esignObj+');">'+Drupal.t('LBL1145')+'&nbsp;</a>';
				} else {
					return '<a href="JavaScript:void(0);" class="admin-insert-to-template-keyword" onmousedown="$(\'body\').data(\'mulitselectdatagrid\').addKeywordtoTemplate(\''+type+'\','+entityId+',\''+entityType+'\','+id+',\''+kwdName+'\');">'+Drupal.t('LBL1145')+'&nbsp;</a>';
				}

			}else if(index == 'manager_name' || index == 'organization_name' ){
				var userLen = rowObject[index].length;
				var titleAllowed = 20;
				var userNameRestricted = (userLen > 8)? $('body').data('mulitselectdatagrid').titleRestrictionFadeoutImage(rowObject[index], 'view-name') : rowObject[index];
				return '<span class="vtip" title="'+htmlEntities(rowObject[index])+'">'+userNameRestricted+'</span>';
			}
			 else {
				return rowObject[index] != 'undefined' ? rowObject[index] : '';
			}
			}catch(e){
				// to do
			}
		},

		titleRestricted : function(title, chara) {
			try{
			var restrictTitle;
			if(title){
			  if(title.length > chara) {
				restrictTitle = title.substring(0,chara);
				restrictTitle = restrictTitle+'...';
			  }else {
				restrictTitle =  title;
			  }
			}else {
			  restrictTitle =  '';
			}
			return restrictTitle;
			}catch(e){
				// to do
			}
		},
		titleRestrictionFadeoutImage : function(title, uniqval) {
			try{
				var restrictTitleNew;
				if(title) {
				  class_fadeout = uniqval;
				  restrictTitleNew =  '<div class="fade-out-title-container-unprocessed fade-out-title-container '+class_fadeout+'"><span class="title-lengthy-text">'+title+'</span><span class="fade-out-image"></span></div>';
				}
				else {
					restrictTitleNew = '';
				}

			return restrictTitleNew;
			}catch(e){
				// to do
			}
		},
		searchDataGrid : function(mode, type, searchKeyword, entityId, entityType, excludedId, skipAccess){
		//searchDataGrid : function(mode, type, searchKeyword, entityId, entityType, excludedId){
			try{
			entityType = ((entityType !='' || entityType != undefined) && (type == 'RoleDisplayUsers' || type == 'RoleDisplayOwners')) ? 'cre_sec' : entityType; // for RoledisplayUsers ; Condition added for this ticket #0040915
			var uniqueId = type+'-'+entityId+'-'+entityType;
			$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId);
			searchKeyword = encodeURIComponent(searchKeyword.replace(/\//g, Drupal.settings.custom.EXP_AC_SEPARATOR));
			if(type == 'RoleDisplayUsers' || type == 'RoleDisplayOwners'){
				var dataGridURL = this.constructUrl("roles/users/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId);
			}else{
				var dataGridURL = this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"/"+searchKeyword+"/"+entityId+"/"+entityType+"/"+excludedId);
			}
			/*this.gridUrl = this.constructUrl("administration/multiselect-grid/"+mode+"/"+type+"//"+entityId+"/"+entityType+"/"+excludedId);
			var showClearLink = "#datagrid-autocomplete-addltext-clr";
			$(showClearLink).css('display', 'block');
            */
			if(type == 'enrolluser' || type == 'enrolltpuser'){
				var searchType = $('#search_all_enroll_type-hidden').val();
				$('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL,postData: { searhType : searchType ,skipAccess:skipAccess} });
			}else if(type == 'CountrySetting'){
				var searchType = $('#search_all_country_type-hidden').val();
				$('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL,postData: { searhType : searchType} });
			}else if(type == 'ContentMoveUsers'){
		    	/* clicking on the search icon in move users qtip */
		    	var searchType = $('#search_all_moveuser_type-hidden').val();
		    	$('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL,postData: { searhType : searchType} });
		    }else if(type == 'trainings'){
		    	var searchType = $('#search_all_trainings_type-hidden').val();
		    	$('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL,postData: { searhType : searchType} });
		    }else if(type == 'content'){
				var searchType = $('#search_all_content_type-hidden').val();
				$('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL,postData: { searhType : searchType} });
		    }else if(type == 'equivalence'){
				var searchType = $('#search_all_equivalence_type-hidden').val();
				$('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL,postData: { searhType : searchType} });
		    }else if(type == 'prerequisite'){
				var searchType = $('#search_all_prerequisite_type-hidden').val();
				$('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL,postData: { searhType : searchType} });
		    }else if(type == 'grpAddOwners' || type == 'RoleDisplayOwners'){
		    	var searchType = $('#search_all_user_type-hidden').val();
				$('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL,postData: { searhType : searchType} });
		    }else if(type == 'SurAssAttachQuestion'){
		    	/* Ticket: 46992 */
				var searchType = $('#tagsearch_all_surass_type-hidden').val();
				dataGridURL = dataGridURL+"&searchType="+searchType;
				$('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL,postData: { searhType : searchType} });
				/* Ticket: 46992 */
		    }else if(type == 'shareuser'){
				var searchType = $('#search_all_enroll_type-hidden').val();
				dataGridURL = dataGridURL+"&searchType="+searchType;
				$('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL,postData: { searhType : searchType} });
		    }else if(type == 'RoleDisplayUsers' || type == 'grpAddUsers'){
				var searchType =$('#search_all_user_type-hidden').val();
				dataGridURL = dataGridURL+"&searchType="+searchType;
				$('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL,postData: { searhType : searchType} });
		    }else {
			    $('#datagrid-container-'+this.uniqueId).setGridParam({url: dataGridURL});
		    }
		    $('#datagrid-container-'+this.uniqueId).trigger("reloadGrid",[{page:1}]);
			}catch(e){
				// to do
			}
		},
		editEntity : function(type, entityId, entityType, associateId,index){
			try{
			var dispIp 	= type+'-'+index+'-'+associateId+'input';
			var dispstr = type+'-'+index+'-'+associateId+'txt';
			var ipBox 	= type+'-'+index+'-'+associateId;
			var ipVal   = $("#"+dispstr).html();

			$("#"+dispIp).show();
			$("#"+dispstr).hide();
			$("#"+ipBox).val(ipVal);
			}catch(e){
				// to do
			}
		},

		deleteEntity : function(type, entityId, entityType, associateId){
			try{
				var delObj = this;
				var wSize = 300;
				var closeButAction = '';
				var html = '';
				var msg ='';
				if(type=='session' && entityType=='cre_sys_obt_cls'){
					msg = Drupal.t("MSG522");
				}else{
					msg = Drupal.t("MSG772");
				}
			    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
			    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
			    html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+msg+'</td></tr>';
			    html+='</table>';
			    html+='</div>';
			    $("body").append(html);
			    var closeButt={};
			    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};
			   	closeButt[Drupal.t('Yes')]= function(){
			   		$('#delete-msg-wizard').remove();
			   		var obj = delObj;
					$('#admin-data-grid .messages').remove();
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+this.uniqueId);
					$.ajax({
						url : delObj.constructUrl("administration/multiselect-grid/delete/"+type+"/"+entityId+"/"+entityType+"/"+associateId),
						async: false,
						success: function(data) {
						    $("#datagrid-container-"+obj.uniqueId).trigger("reloadGrid",[{page:1}]);
						    if(type == "survey" || type == "assessment")
						    	$("input[name="+type+"]").click();
						    else
						    	$("#datagrid-container-"+obj.uniqueId).trigger("reloadGrid",[{page:1}]);
						}
					});
					if ($("#datagrid-container-"+delObj.uniqueId).getGridParam("records")-1 <= 10) {
						$("#datagrid-container-"+delObj.uniqueId+"_toppager").css('visibility','hidden');
					}
					if(type == 'content') {
						if($("#datagrid-container-"+delObj.uniqueId).getGridParam("records") == 1 && document.getElementById("content_hints_markup")){
							$('#content_hints_markup').remove();
						}
						$("#root-admin").data("narrowsearch").refreshGrid();
					}
					if(type == 'assessment') {
						if($("#datagrid-container-"+delObj.uniqueId).getGridParam("records") == 1 && document.getElementById("assess-hints-markup")){
							$('#assess-hints-markup').remove();
						}
						//46112: UI Issue -- Loader is displayed twice on deleting assessment in TP creation screen
						// Two time loader display isse.
						//$("#root-admin").data("narrowsearch").refreshGrid();
					}
					if(type == 'trainings'){
						$("#add-training-hidden-btn-"+delObj.uniqueId).css('display','block');
					}
				};
		    	var drupalTitle = Drupal.t("LBL286");
			    $("#delete-msg-wizard").dialog({
			        position:[(getWindowWidth()-400)/2,200],
			        bgiframe: true,
			        width:wSize,
			        resizable:false,
			        modal: true,
			        title:Drupal.t(drupalTitle),//"title",
			        buttons:closeButt,
			        closeOnEscape: false,
			        draggable:false,
			        zIndex : 10005,
			        overlay:
			         {
			           opacity: 0.9,
			           background: "black"
			         }
			    });
			    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='unique-delete-class'></div>");

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
			    if($('div.qtip-defaults').length > 0) {
			    	var prevZindex = $('.qtip-defaults').css('z-index');
			    	$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
			    	$('.ui-widget-overlay').css('z-index', prevZindex+1);
			    }
			}catch(e){
				// to do
			}
		},

		onlyNumbers : function(evt)
		{
			try{
	         var charCode = (evt.which) ? evt.which : evt.keyCode;
	         if (charCode == 46 || charCode == 37 || charCode == 39) // Right and left arrow key support added : #35968
	        	 return true;
	         else if (charCode > 31 && (charCode < 48 || charCode > 57))
                return false;

             return true;
			}catch(e){
				// to do
			}
		},
        multiSelectSearchBoxKeyDown : function(evt){
        	try{
    		evt = evt || window.event;
    	    var charCode = evt.keyCode || evt.which;
    	    if (charCode == 13) {
    	    	//$("#root-admin").data("narrowsearch").updateAttachedCourseGroup(updatedGroup);
    	    	$('.admin-pagination-search-go').click();
    	        if(evt.preventDefault){
    	            evt.preventDefault();
    	        }else{
    	            evt.returnValue = false;
    	        };
    	        return false;
    	    }
        	}catch(e){
				// to do
			}
        },

    	callEnrollExportProcess : function(courseId,entityId,entityType,type){
    		try{
    			var obj = this;
    			if(type == 'Class') {
    				var searchType = $('#search_all_enroll_type-hidden').val();
    				var defaultText = $('#enrolluser-autocomplete_hidden').val();
    				var searchKeyword = $('.addedit-edit-enrolluser-autocomplete').val();
    				if (searchKeyword == defaultText) {
    					searchKeyword = '';
    				}
    				searchKeyword = encodeURIComponent(searchKeyword);
    				url = obj.constructUrl("administration/exportoption/" + courseId + "/" + entityId + "/" + entityType + "/" + searchType +
    						"/" + searchKeyword + "//CSV");
    			}else if(type == 'Group'){ // added Csv Export #0039985
    				var searchType = $('#search_all_user_type-hidden').val();
    				var defaultText = $('#username-search-autocomplete_hidden').val();		// username-search-autocomplete
    				var searchKeyword = $('.addedit-edit-username-search-autocomplete').val();
    				if (searchKeyword == defaultText) {
    					searchKeyword = '';
    				}
    				searchKeyword = encodeURIComponent(searchKeyword);

    				url = obj.constructUrl("administration/grpUserExportOption/" + entityId + "/" + entityType + "/" + searchType + "/" + searchKeyword + "//CSV");
    			}
    			else if(type == 'GroupOwners'){ //added csv export #0051866
    				var searchType = $('#search_all_user_type-hidden').val();
    				var defaultText = $('#username-search-autocomplete_hidden').val();		// username-search-autocomplete
    				var searchKeyword = $('#addusername-search-autocomplete').val();
    				if (searchKeyword == defaultText) {
    					searchKeyword = '';
    				}
    				searchKeyword = encodeURIComponent(searchKeyword);

    				url = obj.constructUrl("administration/grpOwnerExportOption/" + entityId + "/" + entityType + "/" + searchType + "/" + searchKeyword + "//CSV");
    			}
    			else {
    				var searchType = $('#search_all_enroll_type-hidden').val();
    				var defaultText = $('#enrolltpuser-autocomplete_hidden').val();
    				var searchKeyword = $('.addedit-edit-enrolltpuser-autocomplete').val();
    				if (searchKeyword == defaultText) {
    					searchKeyword = '';
    				}
    				searchKeyword = encodeURIComponent(searchKeyword);
    				url = obj.constructUrl("administration/tpexportoption/" + entityId + "/" + entityType + "/" + searchType + "/" + searchKeyword + "//CSV");
    			}

    			 window.location = url;
    		}catch(e){
				// to do
			}
		},
		callRecertifyExportProcess : function(entityId,entityType,enrollId){
			try{
				var obj = this;
				var type = "CSV";
				//var searchKeyword = $('#org-users-autocomplete-'+orgId).val();
				//searchKeyword = encodeURIComponent(searchKeyword);
				url = obj.constructUrl("administration/tprecertify/" + entityId + "/" + entityType + "/" + enrollId + "/" + type);
				window.location = url;
			}catch(e){
				// to do
			}
		},


		callOrgUsersExportProcess : function(orgId){
			try{
				var obj = this;
				var searchKeyword = $('#org-users-autocomplete-'+orgId).val();
				searchKeyword = encodeURIComponent(searchKeyword);
				url = obj.constructUrl("administration/people/organization/expoertorgusers/" + orgId + "/" +searchKeyword);
				window.location = url;
			}catch(e){
				// to do
			}
		},

    	moreEnrollSearchHideShow : function () {
    		try{
    		$('#select-list-dropdown-list').slideToggle();
    		$('#select-list-dropdown-list li:last').css('border-bottom','0px none');
    		}catch(e){
				// to do
			}
    	},

    	moreEnrollSearchTypeText : function(dText,dType) {
    		try{
    		var dCode = $('#'+dText+'-enr-search').text();
    		$('#select-list-dropdown-list').hide();
    		$('#select-list-dropdown').text(dCode);
    		/*-- #37512: Code Re-Factoring fix --*/
    		$('#enrolltpuser-autocomplete').addClass('input-field-grey');
    		$('#search_all_enroll_type-hidden').val(dText);
    		var displayText = Drupal.t('LBL036') + ' ' + dCode;
    		if(dType == 'TP'){
    			if(dText=='manager'){
    				var newDisplayText = Drupal.t('LBL036') + ' '+Drupal.t('LBL134');
    				$('#enrolltpuser-autocomplete').val(newDisplayText);
    				$('#enrolltpuser-autocomplete_hidden').val(newDisplayText);

    			}else if(dText=='group'){
    				var newDisplayText = Drupal.t('LBL1270');
    				$('#enrolltpuser-autocomplete').val(newDisplayText);
    				$('#enrolltpuser-autocomplete_hidden').val(newDisplayText);

    			}else{
    			$('#enrolltpuser-autocomplete').val(displayText);
    			$('#enrolltpuser-autocomplete_hidden').val(displayText);
    			}
    		}else{
    			 if(dText=='user'){
    					var newDisplayText = Drupal.t('LBL181');
    					$('#enrolluser-autocomplete').val(newDisplayText);
      			  $('#enrolluser-autocomplete_hidden').val(newDisplayText);
    			 }else if(dText=='fullname'){
 					var newDisplayText = Drupal.t('LBL036') +' '+ Drupal.t('LBL107');
					$('#enrolluser-autocomplete').val(newDisplayText);
					$('#enrolluser-autocomplete_hidden').val(newDisplayText);
    			 }else if(dText=='org'){
   					 var newDisplayText = Drupal.t('LBL036') + ' '+Drupal.t('LBL137');
 					   $('#enrolluser-autocomplete').val(newDisplayText);
   			     $('#enrolluser-autocomplete_hidden').val(newDisplayText);
 			     }else if(dText=='jobrole'){
    				  var newDisplayText = Drupal.t('LBL036') + ' '+Drupal.t('LBL133')+' '+ Drupal.t('LBL107');
    					$('#enrolluser-autocomplete').val(newDisplayText);
      			  $('#enrolluser-autocomplete_hidden').val(newDisplayText);
    			 }else if(dText=='manager'){
    	   			var newDisplayText = Drupal.t('LBL036') + ' '+Drupal.t('LBL134');
    	  			$('#enrolluser-autocomplete').val(newDisplayText);
    	    		$('#enrolluser-autocomplete_hidden').val(newDisplayText);
    			 }else if(dText=='group'){
     	   			var newDisplayText = Drupal.t('LBL1270');
     	  			$('#enrolluser-autocomplete').val(newDisplayText);
     	    		$('#enrolluser-autocomplete_hidden').val(newDisplayText);
    	  	 }else{
    			   $('#enrolluser-autocomplete').val(displayText);
    			   $('#enrolluser-autocomplete_hidden').val(displayText);
    			 }
    		}
    		$('#enrolluser-autocomplete').addClass('input-field-grey');
    		}catch(e){
				// to do
			}
    	 },
    	 moreContentSearchTypeText : function(dText,dType) {
      		try{
      		var dCode = $('#'+dText+'-enr-search').text();
      		$('#select-list-dropdown-list').hide();
      		$('#select-list-dropdown').text(dCode);
      		$('#search_all_content_type-hidden').val(dText);
      		$('#content-autocomplete').addClass('input-field-grey');
      		var displayText = Drupal.t('LBL036') + ' ' + dCode;
      		if(dType == 'Content'){
      			if(dText=='name'){
      				var newDisplayText = Drupal.t('LBL756');
      				$('#content-autocomplete').val(newDisplayText);
      				$('#content-autocomplete_hidden').val(newDisplayText);

      			}else if(dText=='tags'){
      				var newDisplayText = Drupal.t('LBL193');
      				$('#content-autocomplete').val(newDisplayText);
      				$('#content-autocomplete_hidden').val(newDisplayText);

      			}else{
 	     			$('#enrolltpuser-autocomplete').val(displayText);
 	     			$('#enrolltpuser-autocomplete_hidden').val(displayText);
      			}
      		}
      		}catch(e){
  				// to do
  			}
      	 },

      	moreEqlCourseSearchTypeText : function(dText,dType) {
      		try{
      		var dCode = $('#'+dText+'-enr-search').text();
      		$('#select-list-dropdown-list').hide();
      		$('#select-list-dropdown').text(dCode);
      		$('#search_all_equivalence_type-hidden').val(dText);
      		$('#equivalence-autocomplete').addClass('input-field-grey');
      		var displayText = Drupal.t('LBL036') + ' ' + dCode;
      		if(dType == 'Course'){
      			if(dText=='name'){
      				var newDisplayText = Drupal.t('LBL702');
      				$('#equivalence-autocomplete').val(newDisplayText);
      				$('#equivalence-autocomplete_hidden').val(newDisplayText);

      			}else if(dText=='tags'){
      				var newDisplayText = Drupal.t('LBL193');
      				$('#equivalence-autocomplete').val(newDisplayText);
      				$('#equivalence-autocomplete_hidden').val(newDisplayText);

      			}else{
 	     			$('#equivalence-autocomplete').val(displayText);
 	     			$('#equivalence-autocomplete_hidden').val(displayText);
      			}
      		}
      		}catch(e){
  				// to do
  			}
      	 },
      	morePreCourseSearchTypeText : function(dText,dType) {
      		try{
      		var dCode = $('#'+dText+'-enr-search').text();
      		$('#select-list-dropdown-list').hide();
      		$('#select-list-dropdown').text(dCode);
      		$('#search_all_prerequisite_type-hidden').val(dText);
      		$('#prerequisite-autocomplete').addClass('input-field-grey');
      		var displayText = Drupal.t('LBL036') + ' ' + dCode;
      		if(dType == 'Course'){
      			if(dText=='name'){
      				var newDisplayText = Drupal.t('LBL702');
      				$('#prerequisite-autocomplete').val(newDisplayText);
      				$('#prerequisite-autocomplete_hidden').val(newDisplayText);

      			}else if(dText=='tags'){
      				var newDisplayText = Drupal.t('LBL193');
      				$('#prerequisite-autocomplete').val(newDisplayText);
      				$('#prerequisite-autocomplete_hidden').val(newDisplayText);

      			}else{
 	     			$('#prerequisite-autocomplete').val(displayText);
 	     			$('#prerequisite-autocomplete_hidden').val(displayText);
      			}
      		} else if(dType == 'Tp'){
      			if(dText=='name'){
      				var newDisplayText = Drupal.t('LBL702');
      				$('#prerequisite-autocomplete').val(newDisplayText);
      				$('#prerequisite-autocomplete_hidden').val(newDisplayText);

      			}else if(dText=='tags'){
      				var newDisplayText = Drupal.t('LBL193');
      				$('#prerequisite-autocomplete').val(newDisplayText);
      				$('#prerequisite-autocomplete_hidden').val(newDisplayText);

      			}else{
 	     			$('#prerequisite-autocomplete').val(displayText);
 	     			$('#prerequisite-autocomplete_hidden').val(displayText);
      			}
      		}
      		}catch(e){
  				// to do
  			}
      	 },

      /* Function called try to seearch different country filter like name and code*/
    	 moreCountrySearchTypeText : function(dCode,dText) {
    	  try{
     		$('#select-list-dropdown-list').hide();
     		$('#select-list-dropdown').text(dCode);
     		$('#search_all_country_type-hidden').val(dText);
     		var displayText = Drupal.t('LBL036') + ' ' + dCode;
     		$('#countrylist-autocomplete').addClass('input-field-grey');
     		if(dText=='countryname'){
     				var newDisplayText = Drupal.t('LBL036') + ' '+ Drupal.t('LBL107');//Type Country name
     				$('#countrylist-autocomplete').val(newDisplayText);
       			$('#countrylist-autocomplete_hidden').val(newDisplayText);
     		}else if(dText=='countrycode'){
     				var newDisplayText = Drupal.t('LBL036') + ' '+ Drupal.t('LBL096'); //Type country code
  					$('#countrylist-autocomplete').val(newDisplayText);
    			  $('#countrylist-autocomplete_hidden').val(newDisplayText);
  			}else{
     			  $('#countrylist-autocomplete').val(displayText);
     			  $('#countrylist-autocomplete_hidden').val(displayText);
     		}
     		$('#countrylist-autocomplete').addClass('input-field-grey');
    	  }catch(e){
				// to do
			}
     	 },

     	/* function called when selecting items in the dropdown
			in move users qtip under content versioning */
	 	 moreMoveUserSearchTypeText : function(dCode,dText,dType) {
	 	  try{
	  		$('#select-moveuser-dropdown-list').hide();
	  		$('#select-moveuser-dropdown').text(dCode);
	  		$('#search_all_moveuser_type-hidden').val(dText);
	  		$('#contentmoveusers-autocomplete').addClass('input-field-grey');
	  		//var displayText = Drupal.t('LBL036') + ' ' + dCode;
	  		var displayText = (dCode == Drupal.t('LBL054')) ? Drupal.t('LBL181') : Drupal.t('LBL951');
				 if(dText=='user'){
					var newDisplayText = Drupal.t('LBL181');
					$('#contentmoveusers-autocomplete').val(newDisplayText);
				    $('#contentmoveusers-autocomplete_hidden').val(newDisplayText);
				 }else if(dText=='status'){
					var newDisplayText = Drupal.t('LBL036') + ' ' + Drupal.t('LBL102');
					$('#contentmoveusers-autocomplete').val(newDisplayText);
				    $('#contentmoveusers-autocomplete_hidden').val(newDisplayText);
				 }else{
				   $('#contentmoveusers-autocomplete').val(displayText);
				   $('#contentmoveusers-autocomplete_hidden').val(displayText);
				 }
				$('#contentmoveusers-autocomplete').addClass('input-field-grey');
	 	 }catch(e){
				// to do
			}
	  	 },

	  	trainingsSearchTypeText : function(dCode,dText) {
	  		try{
		  		$('#select-trainings-dropdown-list').hide();
		  		$('#select-trainings-dropdown').text(dCode);
		  		$('#search_all_trainings_type-hidden').val(dText);

				$('#trainings-autocomplete').addClass('input-field-grey');
	  		}catch(e){
				// to do
			}
		  	 },

	  	 addKeywordtoTemplate : function(type, entityId, entityType, associateId, keywordName){
	  		try{
     		var cursorId = $('#cursor_id').val();
     		var text = keywordName;
    		if(text != '' && text != undefined && text != null){
    			text = ' #@'+keywordName+'@# ';
    		}
    		else{
    			text = '';
    		}
    		if((cursorId.indexOf('message'))>=0){
    			$('#root-admin').data('narrowsearch').removeActiveQtip();
    		 //$('#qtipAttachIdqtip_addkeyword_visible_disp_'+entityId+'_'+entityType).closest('.active-qtip-div').hide();
    		  tinyMCE.activeEditor.selection.moveToBookmark(myBookmark);
    		  tinyMCE.execInstanceCommand(cursorId,"mceInsertContent",false,text);
    		}else if((cursorId.indexOf('subject'))>=0 || (cursorId.indexOf('sms'))>=0){
    		  var txtarea = document.getElementById(cursorId);
    		  
    		  if((cursorId.indexOf('sms'))>=0){
	    		  var txt_area_val = txtarea.value;
	    		  txtarea.value = txt_area_val.replace(Drupal.t('LBL1235'), "");
	    		  $("#edit-sms-text").removeClass("cc-mail-empty");
	    		  $("#edit-sms-text").addClass("cc-mail-notempty");
	    	  }
    		  
    		  var scrollPos = txtarea.scrollTop;
    		  var strPos = 0;
    		  var br = ((txtarea.selectionStart || txtarea.selectionStart == '0') ? "ff" : (document.selection ? "ie" : false ) );
    		  if(br == "ie") {
    			txtarea.focus();
    			var range = document.selection.createRange();
    			range.moveStart ('character', -txtarea.value.length);
    			strPos = range.text.length;
    		  }
    		   else if (br == "ff")
    		     strPos = txtarea.selectionStart;
    		     var front = (txtarea.value).substring(0,strPos);
    		     var back = (txtarea.value).substring(strPos,txtarea.value.length);
    		     txtarea.value=front+text+back; strPos = strPos + text.length;
    		     if (br == "ie") {
    		       txtarea.focus();
    		       var range = document.selection.createRange();
    		       range.moveStart ('character', -txtarea.value.length);
    		       range.moveStart ('character', strPos);
    		       range.moveEnd ('character', 0); range.select();
    		     }
    		     else if (br == "ff") {
    			   txtarea.selectionStart = strPos;
    			   txtarea.selectionEnd = strPos;
    			   txtarea.focus();
    			 } txtarea.scrollTop = scrollPos;
    			 $('#root-admin').data('narrowsearch').removeActiveQtip();
    			// $('#qtipAttachIdqtip_addkeyword_visible_disp_'+entityId+'_'+entityType).closest('.active-qtip-div').hide();
    		  }else{

    		  }
	  		}catch(e){
				// to do
			}
 		},
 		setContentAttempts: function(maxAttempt) {
 			if(document.getElementById(maxAttempt.id+'-hidden') === undefined || document.getElementById(maxAttempt.id+'-hidden') === null) {
 				$('#catalog-class-basic-addedit-form-container').append('<input type="hidden" id="'+maxAttempt.id+'-hidden" name="'+maxAttempt.name+'-hidden" value="'+maxAttempt.value+'">');
 			}
 			else {
 				document.getElementById(maxAttempt.id+'-hidden').value = maxAttempt.value;
 			}
 		},
 		showMenuOptions : function(element) {
 			try {
 				if($(element).next().css('display') == 'block'){
 					$(element).next().css('display', 'none');
 				} else {
 					$(element).next().css('display', 'block');
 				}
 			} catch(e) {
 				
 			}
 		}


	});

	$.extend($.ui.mulitselectdatagrid.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);

$(function(){ 	$("body").mulitselectdatagrid(); });

//Hide the drop down option values
$('body').bind('click', function(event) {
	try{
	if(event.target.id != 'admin-dropdown-arrow'){
		$('#select-list-dropdown-list dropdown-link-font ').hide();
	}
	}catch(e){
		// to do
	}
});
$('#edit-sms-text, #edit-subject').live('focus', function(event) {
	try{
		if(document.getElementById('cursor_id')!= null){
			$('#cursor_id').val(event.target.id);
		}
	}catch(e){
		// to do
	}
});
//remove data grid resizer span
$('.ui-jqgrid .ui-jqgrid-resize').live('mouseover mousemove keypress', function(){
	try{
	$(this).remove();
	}catch(e){
		// to do
	}
});

function exemptedVisibility(qtipObj){
	$('#enrolled_selected_'+qtipObj.entityId+'-'+qtipObj.enrId+'-'+qtipObj.entityType).parents('table.ui-jqgrid-btable:first').find('td').css('overflow','visible');
		$('.qtip-popup-exempted').html('').hide();
		$('.completion_container_roster').hide()
		$('#' + qtipObj.popupDispId).qtipPopup({
			wid : qtipObj.wid,
			heg : qtipObj.heg,
			entityId : qtipObj.entityId,
			popupDispId : qtipObj.popupDispId,
			catalogVisibleId : qtipObj.catalogVisibleId,
			postype : qtipObj.postype,
			poslwid : (qtipObj.poslwid == '' || qtipObj.poslwid == undefined || qtipObj.poslwid == null) ? '' : qtipObj.poslwid,
			posrwid : (qtipObj.posrwid == '' || qtipObj.posrwid == undefined || qtipObj.posrwid == null) ? '' : qtipObj.posrwid,
			disp	: (qtipObj.qdis == '' || qtipObj.qdis == undefined || qtipObj.qdis == null) ? ''	: qtipObj.qdis,
			linkid	: qtipObj.linkid,
			onClsFn	: qtipObj.onClsFn,
			dispDown : (qtipObj.dispDown == undefined) ? '': qtipObj.dispDown,
		});
		setTimeout(function(){EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("paintContent"+ qtipObj.popupDispId)},10);
		$("#root-admin").data("narrowsearch").visiblePopup(qtipObj);	
	}

function enrollmentExemptedEdit(qtipObj){
	//console.log(qtipObj.toSource());
	var waivedlinkHTML = '';
	var qtipOptWaivedObj  = "{'entityId' : '"+ qtipObj.entityId + "'"+
	", 'entityType' : '"+qtipObj.entityType +"'" +
	", 'url' : '"+qtipObj.url+"'" +
	", 'popupDispId' : 'enrolled-exempted-"+qtipObj.entityId + "'"+
	", 'catalogVisibleId' : 'qtipAttachIdqtip_pwdstrength_visible_disp_" + qtipObj.entityId + "'" +
	", 'wid' : 400"+", 'heg' : 200" +", 'postype' : 'bottomright'" +",'poslwid' : 70 "+", 'qdis' : 'ctool'" +
	", 'linkid' : 'visible-exempted-"+ qtipObj.entityId+"','dispDown' : 'Y'"+", 'enrId' : '"+qtipObj.enrId +"'}";	

	waivedlinkHTML = 
		'<span class="enrolled-exempted-container">' +
		'<span id="enrolled-exempted-' + qtipObj.entityId + '" class="enrolled-exempted-status">' +
		'<a id="visible-exempted-'+qtipObj.entityId+'" onclick = "exemptedVisibility(' + qtipOptWaivedObj + ');" class="enrolled-exempted-status-link">Waived' +
		'</a>' +
		'<span id="visible-popup-' + qtipObj.entityId + '" class="qtip-popup-exempted" style="display:none; position:absolute; left:0px; top:0px;" ></span>'+
		'</span>' +
		'</span>';
	
	$('#enrolled_selected_'+qtipObj.entityId +'-'+qtipObj.enrId+'-'+qtipObj.entityType).html(waivedlinkHTML);
	$('#visible-exempted-'+qtipObj.entityId).click();
} 

function exemptedEditVisibility(qtipObj){
	//$('#enrolled_selected_'+qtipObj.entityId+'-'+qtipObj.enrId+'-'+qtipObj.entityType).parents('table.ui-jqgrid-btable:first').find('td').css('overflow','visible');
		$('.qtip-popup-exempted-edit').html('').hide();
		$('#' + qtipObj.popupDispId).qtipPopup({
			wid : qtipObj.wid,
			heg : qtipObj.heg,
			entityId : qtipObj.entityId,
			popupDispId : qtipObj.popupDispId,
			postype : qtipObj.postype,
			poslwid : (qtipObj.poslwid == '' || qtipObj.poslwid == undefined || qtipObj.poslwid == null) ? '' : qtipObj.poslwid,
			posrwid : (qtipObj.posrwid == '' || qtipObj.posrwid == undefined || qtipObj.posrwid == null) ? '' : qtipObj.posrwid,
			disp	: (qtipObj.qdis == '' || qtipObj.qdis == undefined || qtipObj.qdis == null) ? ''	: qtipObj.qdis,
			linkid	: qtipObj.linkid,
			onClsFn	: qtipObj.onClsFn,
			dispDown : (qtipObj.dispDown == undefined) ? '': qtipObj.dispDown,
		});
		setTimeout(function(){EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("paintContent"+ qtipObj.popupDispId);
		if(document.getElementById('exempteddetaillistform')){$('.edit-exempted-list-values #bubble-face-table').css('bottom','-'+$('.edit-exempted-list-values #bubble-face-table').css('bottom'));}},10);
		$("#root-admin").data("narrowsearch").visiblePopup(qtipObj);
}

(function($) {
	try{
	$.fn.exemptedQtipClose = function(popupid,entity_id,type,uniqueid) {
		if(typeof(type)==='undefined') type = '';
		//console.log("waived status unique id"+uniqueid);
		if(typeof(uniqueid)==='undefined') uniqueid = '';
		if(type == 'all'){		
			$('.qtip-popup-exempted').html('').hide();
			if($('#hidden-unwaived-id').val() == 'unwaived'){	
				 $('#hidden-unwaived-id').val('');				
				 $('#unwaivedandsave_'+uniqueid).click();				
			}else{
				$('#waivedandsave_'+uniqueid).click();	
			}
			
		}else{
			$('.qtip-popup-exempted').html('').hide();
		}
		
	};
	}catch(e){
			// To Do
		}
})(jQuery);
