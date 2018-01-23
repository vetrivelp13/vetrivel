var ajaxCall = false;
var repQryState = ''; //Added for #0070534
(function($) {
$.widget("ui.reportsearch", {
		_init: function() {	
		 try{	
			var searchStr = '';
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			this.saveFlag = false;
			this.report_editable = 0;
			this.report_builder_type = '';
			this.report_id;	//change by ayyappans for 40015: Unable to create report using query builder
			this.publish_title = '';
			this.hasMandatoryFields = '';
			this.reportLaunch = '';
			this.editstatus = '';
			this.current_step_div='report-query-builder-section';
			this.reportCategory	= '';
			this.reportSelectedCategory	= '';
			this.reportBuilderType	= '';
			this.parentTableId = '';
			this.directLaunch=0;
		  }catch(e){
		 		//Nothing to do
		  }
},	

reportSelectWizard: function(){
	try{
	$('#report-select-wizard').remove();
	var html = '';
	
	html+='<div id="report-select-wizard" style="display:none; padding: 0px;">';
	html+='<div class="new-report">';
		//change by ayyappans for 37727: When we edit the report the steps on should be click able so that we can go the section directly
	html+='<div class="report-wizard-image" onClick="$(\'body\').data(\'reportsearch\').reportAddWizard(\'report-data-source\', \'\', \'new\');"></div>';	
	html+='<a href="javascript:void(0)" onClick="$(\'body\').data(\'reportsearch\').reportAddWizard(\'report-data-source\', \'\', \'new\');" class="new-report-title">'+Drupal.t('LBL784')+'</a>';
	html+='<div class="report-green-title">'+Drupal.t('MSG490')+'?</div>';
	html+='<div class="new-report-names"><span class="orange-arrow "></span>'+Drupal.t('MSG491')+'</div>';
	html+='<div class="new-report-names"><span class="orange-arrow "></span>'+Drupal.t('MSG492')+'</div>';
	html+='<div class="new-report-names"><span class="orange-arrow "></span>'+Drupal.t('MSG493')+'</div>';
	html+='<div class="new-report-names"><span class="orange-arrow "></span>'+Drupal.t('MSG494')+'</div>';
	html+='<div class="new-report-names"><span class="orange-arrow "></span>'+Drupal.t('MSG495')+'</div>';
	html+='</div>';	
	html+='<div class="new-report">';
	html+='<div class="report-sql-query-builder-image" onClick="$(\'body\').data(\'reportsearch\').reportAddWizard(\'report-query-builder-section\', \'\', \'new\');"></div>';	
	html+='<a href="javascript:void(0)" onClick="$(\'body\').data(\'reportsearch\').reportAddWizard(\'report-query-builder-section\', \'\', \'new\');" class="new-report-title">'+Drupal.t('LBL785')+'</a>';
	html+='<div class="report-green-title">'+Drupal.t('MSG496')+'</div>';
	html+='<div class="new-report-names"><span class="orange-arrow "></span>'+Drupal.t('MSG492')+'</div>';
	html+='<div class="new-report-names"><span class="orange-arrow "></span>'+Drupal.t('MSG493')+'</div>';
	html+='<div class="new-report-names"><span class="orange-arrow "></span>'+Drupal.t('MSG494')+'</div>';
	html+='<div class="new-report-names"><span class="orange-arrow "></span>'+Drupal.t('MSG495')+'</div>';
	html+='</div>';	
	html+='</div>';
	
    $("body").append(html);

    var closeButt={};
    $("#report-select-wizard").dialog({
      /*  position:[(getWindowWidth()-700)/2,100],*/
        bgiframe: true,
        width:810,
        height:495,
        resizable: false,
        modal: true,
        title:Drupal.t('Report'),
        buttons:closeButt,
        close: function(){},
        closeOnEscape: false,
        draggable: true,
        overlay:
         {
           opacity: 0.9,
           background: "black"
         }
    });  
	$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	$("#report-select-wizard").show();	
    $('.ui-dialog-titlebar-close').click(function(){
        $("#report-select-wizard").remove();
    });
    if(this.currTheme == 'expertusoneV2'){
    	$('.ui-dialog-content').addClass('edit-report-wizard');
    	changeDialogPopUI();
    }
	}catch(e){
 		//Nothing to do
	}
},

reportAddWizard: function(type, reportId, status){
	try{
	$('.active-qtip-div').remove();
	$('body').data('reportsearch').getReportCategory();
	var obj	= this;
	$('#report-select-wizard').remove();
	obj.publish_title = '';
	if(reportId!='')
		this.report_id = reportId;
	
	if(reportId=='' || reportId==undefined){		
		obj.reportSelectedCategory='';
		obj.reportBuilderType = '';
	}
	if(status == undefined) {
		status = '';
	} else if(status == 'new') { //change by ayyappans for 40015: Unable to create report using query builder
		this.report_id = undefined;
	}
	this.current_step_div = type;
	if(type == 'report-data-source'){
		this.report_builder_type = 'design_wizard';
	} else if(type == 'report-query-builder-section'){
		this.report_builder_type = 'query_builder';
	}

	this.saveFlag = false;
	
	$("body").data("reportsearch").fieldsArray = '';
	$("body").data("reportsearch").headerArray = '';
	$("body").data("reportsearch").footerArray = '';
	$("body").data("reportsearch").criteriaArray = '';
	$("body").data("reportsearch").sectionpop = 'add';
	
	$('#report-add-wizard').remove();
	var html = '';
	
	html+='<div id="report-add-wizard" style="display:none; padding: 0px;">';
	html+='<div id="show_expertus_message" style="display:none;"></div>';
	html+='<div id="Canvas" style="position:relative;height:5px;width:5px;"></div>';
	
	/*steps navigation */
	/*if(type == 'report-query-builder-section' ) { */
	 if(obj.currTheme == 'expertusoneV2'){
		 //open top border box
		 html+='<div class="report-wizard-topbar"><a class="previousLink-inactive" onclick="$(\'body\').data(\'reportsearch\').navigateHeader(\'previous\')">'+Drupal.t('LBL692')+'</a>';
	 }	else{
		html+='<a class="previousLink-inactive" onclick="$(\'body\').data(\'reportsearch\').navigateHeader(\'previous\')">'+Drupal.t('LBL692')+'</a>';
	 }
	/*} else {
		html+='<div class="previousLink"><a onclick="$(\'body\').data(\'reportsearch\').navigateHeader(\'previous\')"></a></div>';
	}*/
	html+='<div class="breadcrump-container">';
	html+='<div class="reports-title-div-selected" id="report-data-source-breadcrump">';
	//change by ayyappans for 37727: When we edit the report the steps on should be click able so that we can go the section directly
	if(status != 'new') {
		if(type == 'report-data-source'){
			html+='<span style="cursor: pointer;" class="report-menu-left-bg" onclick="$(\'body\').data(\'reportsearch\').navigateHeader(\'show\', \'report-data-source\', \''+status+'\')">'+Drupal.t('LBL828')+'</span></div>';
		}else{
			html+='<span style="cursor: pointer;" class="report-menu-left-bg" onclick="$(\'body\').data(\'reportsearch\').navigateHeader(\'show\', \'report-query-builder-section\', \''+status+'\')">'+Drupal.t('LBL786')+'</span></div>';
		}
		html+='<div style="cursor: pointer;" class="reports-title-div" onclick="$(\'body\').data(\'reportsearch\').navigateHeader(\'show\', \'report-builder-section\', \''+status+'\')" id="report-builder-section-breadcrump">'+Drupal.t('LBL787')+'</div>';
		html+='<div style="cursor: pointer;" class="reports-title-div-end" onclick="$(\'body\').data(\'reportsearch\').navigateHeader(\'show\', \'report-query-details\', \''+status+'\')" id="report-query-details-breadcrump">'+Drupal.t('LBL614')+'</div><div class="clearBoth"></div></div>';
	}
	else {
		if(type == 'report-data-source'){
			html+='<span class="report-menu-left-bg">'+Drupal.t('LBL828')+'</span></div>';
		}else{
			html+='<span class="report-menu-left-bg">'+Drupal.t('LBL786')+'</span></div>';
		}
		html+='<div class="reports-title-div" id="report-builder-section-breadcrump">'+Drupal.t('LBL787')+'</div>';
		html+='<div class="reports-title-div-end" id="report-query-details-breadcrump">'+Drupal.t('LBL614')+'</div><div class="clearBoth"></div></div>';
	}
	html+='<a class="nextLink" onclick="$(\'body\').data(\'reportsearch\').navigateHeader(\'next\')">'+Drupal.t('LBL693')+'</a>';
	if(obj.currTheme == 'expertusoneV2'){
	html+='</div>'; //close top border box
	}
	html+='<div id="report-resultmsg-div"></div>';
	html+='<div id="report-success-resultmsg-div"></div>';
	
	/*Create your datasource tab*/
	html+='<div id="report-data-source" class="maintabs" style="display:none;">';
	html+='<div class="tables-container">';
	html+='<div class="clearBoth query-tables-left"><div class="query-tables-right"><div class="query-tables-middle"></div></div></div>';
	
	/* THE BELOW TAGS FOR REPORT SEARCH FILLTER */
	html+='<div class="report-query-build-container"><div class="report-table-search-container"><div class="report-search-panel-header"><h2>'+Drupal.t('LBL844')+'</h2></div>';
	if(this.currTheme == 'expertusoneV2'){
		html+='<div class="report-header-search-item">';
		html+='<div class="filter-search-start-date-left-bg"></div>';
		html+='<input class="filter-search-start-date-middle-bg ac_input" id="report_table_autocomplete" ondrop="return false;" onfocus="$(\'#lnr-reports-search\').data(\'lnrreportssearch\').hightlightedText(\'report_table_autocomplete\',\'Search\')" onblur="$(\'#lnr-reports-search\').data(\'lnrreportssearch\').hightlightedText(\'report_table_autocomplete\',\'Search\')"  onchange="$(\'#lnr-reports-search\').data(\'lnrreportssearch\').hightlightedText(\'report_table_autocomplete\',\'Search\')" onkeypress="$(\'#lnr-reports-search\').data(\'lnrreportssearch\').reportEnterKey();" />';
		html+='<a class="narrow-text-search" href="javascript:void(0)" onclick="$(\'body\').data(\'reportsearch\').reportAutoSearchList();"></a><div class="filter-search-start-date-right-bg"></div></div>';
		html+='</div>';
	}
	else{
		html+='<span class="eol-search-input"><input id="report_table_autocomplete" ondrop="return false;" onfocus="$(\'#lnr-reports-search\').data(\'lnrreportssearch\').hightlightedText(\'report_table_autocomplete\',\'SEARCH\')" onblur="$(\'#lnr-reports-search\').data(\'lnrreportssearch\').hightlightedText(\'report_table_autocomplete\',\'SEARCH\')"  onchange="$(\'#lnr-reports-search\').data(\'lnrreportssearch\').hightlightedText(\'report_table_autocomplete\',\'SEARCH\')" onkeypress="$(\'#lnr-reports-search\').data(\'lnrreportssearch\').reportEnterKey();this.style.fontSize=\'13px\';this.style.fontStyle=\'normal\';" /></span>';
		html+='<a href="javascript:void(0)" class="report-table-search-results" onclick="$(\'body\').data(\'reportsearch\').reportAutoSearchList();"></a></div>';
	}
	
	/* PAINT REPORT SEARCH RESULTS */
	html+='<div id="query_builder_tables" class="item_container scroll-pane">';
	html+='</div></div><div class="clearBoth query-tables-footer-left"><div class="query-tables-footer-right"><div class="query-tables-footer-middle"></div></div></div></div>';
	html+='<div style="display:block; float:left; width:10px; height:100%;"></div>';
	html+='<div id="query_builder_tagsDrop"><div class="clearBoth query-tables-left"><div class="query-tables-right"><div class="query-tables-middle"></div></div></div>';
	html+='<div class=""><div class="back ui-draggable ui-droppable scroll-pane-drop-container" id="cart_items"></div></div><div class="clearBoth query-tables-footer-left"><div class="query-tables-footer-right"><div class="query-tables-footer-middle"></div></div></div>';
	html+='<input type="hidden" name="listTables" id="listTables" value="" />';
	html+='<input type="hidden" name="listTablesDisplay" id="listTablesDisplay" value="" />';
	html+='<br /><input type="hidden" name="Relcoloumn" id="Relcoloumn" value="" />';
	html+='<br /><input type="hidden" name="selColoumn" id="selColoumn" value="" />';
	html+='<br /><input type="hidden" name="windowPositions" id="windowPositions" value="" />';
	html+='<br /><input type="hidden" name="report_details_id" id="report_details_id" value="" />';
	//html+='<div><input type="button" value="Save" onclick="$(\'body\').data(\'reportsearch\').validateReportQueryBuilder();"/></div>';
	html+='</div><div class="clearBoth"></div>';
	html+='</div>';

	//Query builder tab
	html+='<div id="report-query-builder-section" class="maintabs" style="display:none;">';
	html+='<div class="report-query-builder-textarea"><div class="report-query-builder-textarea-info">'+Drupal.t('MSG497')+'</div><textarea id="qry_builder_area" rows="10" cols="80" onkeypress="$(\'body\').data(\'reportsearch\').queryTextClick();" onclick="$(\'body\').data(\'reportsearch\').queryTextClick();" onblur="$(\'body\').data(\'reportsearch\').queryTextBlur();"></textarea></div>';
	//Please make sure when deleting the below commented line
	//html+= '<div><div class="admin-save-button-left-bg"></div><a class="admin-save-button-middle-bg" onclick="$(\'body\').data(\'reportsearch\').setQueryBuilder()">'+Drupal.t('LBL783')+'</a><div class="admin-save-button-right-bg"></div><div class="clearBoth"></div></div>';
	html+='</div>';
	
	//Design you report layout tab
	html+='<div id="report-builder-section" class="maintabs" style="display:none;">';
	html+='</div>';
	
	//Save and Publish tab
	var themeValue=1;
	html+='<div id="report-query-details" style="display:none;"  class="maintabs" align="center" >';
	if(obj.currTheme == 'expertusoneV2'){
		var lang = Drupal.settings.user.language; // language fix for the #0043857
		if(lang == 'pt-pt'){
			html+='<br/><table width="60%" cellpadding="1" cellspacing="0" border="0">';
		}else{
			html+='<br/><table width="57%" cellpadding="1" cellspacing="0" border="0">';
		}
	}else
	{
	html+='<br/><table width="62%" cellpadding="1" cellspacing="0" border="0">'; 
	}
	var testFormCls = '';
	if(this.currTheme == 'expertusoneV2'){
		testFormCls = 'textform';
	}
    html+= '<tr><td valign="top" class="report-query-wizard-tdWdith commanTitleAll"><span>'+Drupal.t('LBL799')+': </span><span class="require-text">*</span></td><td><input type="text" name="set_mainquerform_ReportTitle" id="set_mainquerform_ReportTitle"  size="50" class="reports-text-box '+testFormCls+'"></td></tr>';
    html+= '<tr><td valign="top" class="commanTitleAll"><span>'+Drupal.t('LBL229')+': </span></td><td><textarea name="set_mainquerform_ReportDescription" id="set_mainquerform_ReportDescription" rows="5" cols="50" class="reports-text-area '+testFormCls+'"></textarea></td></tr>';
    if(themeValue!='' && themeValue!=null){
    //html+= '<tr><td width="40%"><span>'+Drupal.t('LBL802')+': </span></td><td><input type="text" name="set_mainquerform_Theme_Name" id="set_mainquerform_Theme_Name"  size="50" class="reports-text-box"></td></tr>';
    }
    if(obj.reportVisibility.length > 0){
	    html+= '<tr><td width="40%" class="commanTitleAll"><span>'+Drupal.t('LBL800')+': </span><span class="require-text">*</span></td>'
	     html+='<td>';
	      if(obj.currTheme == 'expertusoneV2'){
			  html += '<div class="expertus-dropdown-bg set_report_dropdown"><div class="expertus-dropdown-icon ">';
		   }	
	    html+= '<select id="report_visibility" name="report_visibility" multiple="multiple">';
	    for(var i=0;i<obj.reportVisibility.length;i++){
	    	html+= '<option title="'+htmlEntities(obj.reportVisibility[i]['name'])+'" value="'+obj.reportVisibility[i]['code']+'"><span class="vtip" title="'+htmlEntities(obj.reportVisibility[i]['name'])+'">'+obj.reportVisibility[i]['name']+'</span></option>';
	    }
	    html+= '</select>';
	     if(obj.currTheme == 'expertusoneV2'){
			 html += '</div></div>';
		 }
	    html+='</td></tr>';
    }
    if(obj.reportCategory.length > 0){
	    html+= '<tr><td width="40%" class="commanTitleAll"><span>'+Drupal.t('LBL801')+': </span><span class="require-text">*</span></td>';
	    html+= '<td>';
	    if(obj.currTheme == 'expertusoneV2'){
			html += '<div class="expertus-dropdown-bg set_report_dropdown"><div class="expertus-dropdown-icon ">';
		}		
	    html+='<select id="set_report_type" name="set_report_type">';
	    for(var i=0;i<obj.reportCategory.length;i++){
	    	html+= '<option title="'+obj.reportCategory[i]['code']+'" value="'+obj.reportCategory[i]['code']+'">'+obj.reportCategory[i]['name']+'</option>';
	    }
	    html+= '</select>';
	    if(obj.currTheme == 'expertusoneV2'){
			html += '</div></div>';
		}
	    html+= '</td></tr>';
    }
    html+= '<tr><td width="40%"></td><td><div class="addedit-form-cancel-and-save-actions-row"><div class="addedit-form-cancel-container-actions">';
    if(obj.currTheme == 'expertusoneV2'){
		html += '<div class="white-btn-bg-left"></div>';
	    html += '<input type="button" value="'+Drupal.t('LBL109')+'" name="cancelreport" id="edit-catalog-course-basic-cancel"  class="addedit-edit-catalog-course-basic-cancel admin-action-button-middle-bg addedit-form-expertusone-throbber exp-addedit-form-cancel-button white-btn-bg-middle"  onclick="$(this).dialog(\'destroy\');$(this).dialog(\'close\'); $(\'#report-add-wizard\').remove();">';
	    html += '<div class="white-btn-bg-right"></div>';
	}
    else{
    	html+= '<input type="button" value="'+Drupal.t('LBL109')+'" name="cancelreport" id="edit-catalog-course-basic-cancel"  class="addedit-edit-catalog-course-basic-cancel admin-action-button-middle-bg addedit-form-expertusone-throbber exp-addedit-form-cancel-button form-submit ajax-processed"  onclick="$(this).dialog(\'destroy\');$(this).dialog(\'close\'); $(\'#report-add-wizard\').remove();">';
    }
    html+= '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>';
   	html+= '<input type="button" value="'+Drupal.t('LBL141')+'" name="saveandpublish" id="edit-catalog-course-save-publish" onclick="$(\'body\').data(\'reportsearch\').reportPublishAndUnpublishdetails(\'cre_rpt_rps_itv\');"  class="admin-save-button-dropdown-middle-bg-wrapper admin-save-button-middle-bg form-submit ajax-processed"><span class="pub-unpub-add-action-wrapper pub-unbpub-more-btn " onclick="displayPubActionList(\'crs-pub-save-btn\')" id="pub-unpub-action-btn">&nbsp;</span></div>'+    	
    	   '<ul class="catalog-pub-add-list crs-pub-save-btn" style="display: none;">'+
           '<li class="save-pub-unpub-sub-menu"><input type="button" class="form-submit ajax-processed" value="'+Drupal.t('LBL614')+'" name="unpublish" id="edit-catalog-course-save-unpublish" onclick="$(\'body\').data(\'reportsearch\').reportPublishAndUnpublishdetails(\'cre_rpt_rps_atv\');"></li></ul><div class="clearBoth"></div></div><div class="clearBoth"></div></div></td>'
    html+='</table><br/>';
	html+='</div>';

	$("body").append(html);

    $('#'+type).show();
	
    $("#report_visibility").multiselect();
    if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
    var dialogWidth = 885;
    }else
    {
    var dialogWidth = 930;
    }
    /*if(this.currTheme == 'expertusoneV2'){
    	dialogWidth = 820;
    }*/

    var closeButt={};
    $("#report-add-wizard").dialog({
        position:[(getWindowWidth()-dialogWidth)/2,100],
        bgiframe: true,
        width:dialogWidth,
        //height: 850,
        resizable: false,
        modal: true,
        title:Drupal.t('Report'),
        buttons:closeButt,
        overflow: scroll,
        dialogClass: 'report-add-wizard-dialog',
        close: function(){
			if(obj.saveFlag==true){
				if (typeof $("#lnr-reports-search").data("lnrreportssearch").refreshLastAccessedReportResult != 'undefined' && $("#lnr-reports-search").data("lnrreportssearch").refreshLastAccessedReportResult() == false) {
					$("#paintLearnerReportsResults").trigger("reloadGrid",[{current:true}]);
				}
			}
			// $(".ac_results").remove(); // while closing dialog remove auto-complete list #0023888
			$('.ac_results').css('display', 'none'); // while closing dialog display none auto-complete list #0042860
		},
        closeOnEscape: false,
        draggable: false,
        overlay:
         {
           opacity: 0.9,
           background: "black"
         }
    });
	if ($("#calendarprimaryview").size())
    	calRefreshAftClose();
	$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	$("#report-add-wizard").show();	
    $('.ui-dialog-titlebar-close').click(function(){
        $("#report-add-wizard").remove();
    });
    
    if(type == 'report-data-source'){
    	$("#report-add-wizard").css('height','auto');
    }
    if(this.currTheme == 'expertusoneV2'){
    	$('.ui-dialog-content').addClass('edit-report-wizard');
    	changeDialogPopUI();
    	//$('.expertusV2PopupContainer').after('<div class="popupBotLeftCorner ui-helper-clearfix"><div class="popupBotRightCorner"><div class="popupBotmiddle"></div></div></div>');
    }
    //when it comes from report design wizard
    if(type == 'report-data-source' ){
	    /*Table list autopopulate values*/
		$("#report_table_autocomplete" ).autocomplete(
				"/?q=administration/report/search/tables-autocomplete",{
				minChars :3,
				max :50, 
				autoFill :true,
				mustMatch :false,
				matchContains :false,
				inputClass :"ac_input",
				loadingClass :"ac_loading"
				},	    			
				"appendTo", 
				"#report-add-wizard"
		);
		$('#report_table_autocomplete').bind('keyup', function(e) {
		    if ( e.keyCode === 13 ) { // 13 is enter key
		    	obj.reportAutoSearchList();
		    }
		});
		$("body").data("reportsearch").getQueryBuilderTables();
		
    }
    //CUSTOM TOOL TIP FOR SHOWING THE RESTRICTED CHARACTED
    vtip();
  //backgrond overlay div come with full window script
    jQuery('.ui-widget-overlay').css({"height":"100%", "position":"fixed"});
	}catch(e){
 		//Nothing to do
	}
},
reportAutoSearchList: function(){
	try{
	var autoValue = $('#report_table_autocomplete').val();
	autoValue = $.trim(autoValue);
	if(autoValue == '' || autoValue == null || autoValue == undefined || autoValue == Drupal.t("LBL304").toUpperCase() || autoValue == 'Search') {
		autoValue = '0';
	}
	$("body").data("reportsearch").getQueryBuilderTables('',autoValue);
	}catch(e){
 		//Nothing to do
	}
},
setJsScrollpane: function(type,Id){
	try{
	if(type == 'scroll-pane') {
		$('.scroll-pane').jScrollPane();
	} else if(type == 'scroll-pane-drop-container') {
		$('.scroll-pane-drop-container').jScrollPane();
	} else if(type == 'tableRelated') {
			$('.'+Id).jScrollPane();
	} else if(type == 'tableCustomscrollbar') {
		if($('#cart_items #col_tbl_'+Id).find('.jspContainer').length == 0 ) {
			$('#cart_items #col_tbl_'+Id).jScrollPane();
		} /*else {
			$('#cart_items #col_tbl_'+Id).children().removeClass('jspContainer').removeAttr('style');
			$('#cart_items #col_tbl_'+Id).children().children().removeClass('jspPane').removeAttr('style');
			$('#cart_items #col_tbl_'+Id).find('.jspVerticalBar').remove();
			$('#cart_items #col_tbl_'+Id).jScrollPane();
		}*/
	}
	}catch(e){
 		//Nothing to do
	}
},
setQueryBuilder: function(fromStep){
	try{
	var obj = this;
	var timestamp = new Date().getTime();
	//param +=  "&qryval="+$("#qry_builder_area").val()+'&timestamp='+timestamp;
   var param = {
        qryval : $("#qry_builder_area").val(),
        timestamp: timestamp
    };
	var url = obj.constructUrl("administration/report-search/qry-build/"+obj.report_id);
	var returnVal = false;
	obj.createLoader('report-add-wizard');
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(result){
			obj.destroyLoader('report-add-wizard');
			repQryState = result.result;
		  if(result.Err == 'Success'){
			$('show_expertus_message').hide();
			obj.report_id = result.report_id;
			if(fromStep == undefined) {
				fromStep = 'report-query-builder-section';
			}
			obj.loadReportLayout(result.report_id, fromStep, 0);
			obj.commonError=false;	
			returnVal = true;
			//Success Method
			if(obj.report_id==undefined) {
				obj.editstatus = '';
				$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-selected');
				$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-div-completed');
			}
			else {
				$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-div-completed');
				obj.editstatus = 'edit';
				if(obj.publish_title == ''){
					$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-selected');
				} else {
					$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-edit-selected');
				}
			}
		  }
		  else{
			  var error = new Array();
			  error[0] = result.Err;
			  var message_call = expertus_error_message(error,'error');
			  $('#show_expertus_message').show();
			  $('#show_expertus_message').html(message_call);
			  obj.current_step_div = 'report-query-builder-section';
			  $('#report-data-source').hide();
			  $('#report-query-details').hide();
			  $('#report-builder-section').hide();
			  $('#report-query-builder-section').show();
		  }
		},
		failure: function(data){
			var msg = new Array();
			obj.destroyLoader('report-add-wizard');
			
			if(fromStep == undefined) {
				fromStep = 'report-query-builder-section';
			}
			obj.loadReportLayout(obj.report_id, fromStep, 0);
			obj.commonError=false;	
			returnVal = true;

			$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-div-completed');
			obj.editstatus = 'edit';
			if(obj.publish_title == ''){
				$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-selected');
			} else {
				$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-edit-selected');
			}
			
			msg[0] = Drupal.t('ERR256');
			var message_call = expertus_error_message(msg,'error');
			$('#show_expertus_message').show();
			$('#show_expertus_message').html(message_call);
			obj.gridErrorHandler();
		},
  		error:function (XMLHttpRequest, textStatus, errorThrown) {
			var msg = new Array();
			obj.destroyLoader('report-add-wizard');
			
			if(fromStep == undefined) {
				fromStep = 'report-query-builder-section';
			}
			obj.loadReportLayout(obj.report_id, fromStep, 0);
			obj.commonError=false;	
			returnVal = true;

			$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-div-completed');
			obj.editstatus = 'edit';
			if(obj.publish_title == ''){
				$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-selected');
			} else {
				$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-edit-selected');
			}
			
			msg[0] = Drupal.t('ERR256');
			var message_call = expertus_error_message(msg,'error');
			$('#show_expertus_message').show();
			$('#show_expertus_message').html(message_call);
			obj.gridErrorHandler();
	   	}
	});
	return returnVal;
	}catch(e){
 		//Nothing to do
	}
},
queryTextClick: function(){
	try{
	var textVal = $('#qry_builder_area').val().length;
	if(textVal >= 0){
		$('.report-query-builder-textarea-info').css('display','none');
	}
	}catch(e){
 		//Nothing to do
	}
},
queryTextBlur: function(){
	try{
	var textVal = $('#qry_builder_area').val();
	if(textVal==''){
		$('.report-query-builder-textarea-info').css('display','block');
	} else {
		$('.report-query-builder-textarea-info').css('display','none');
	}
	}catch(e){
 		//Nothing to do
	}
},

loadReportData : function(){
	try{
	this.directLaunch = 0;
	var obj = this;
	var currTheme = Drupal.settings.ajaxPageState.theme;
	url = obj.constructUrl("administration/report-search/load-querybuilder/"+obj.report_id);
	obj.publish_title = '';
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		success: function(result){
			var reportDetails = result.report_details;
			var reportType = reportDetails.report_builder_type;
			$("#report-data-source-breadcrump").removeClass().addClass("reports-title-div-edit-selected");
			$("#report-builder-section-breadcrump").removeClass().addClass("reports-title-next-div-completed");
			if(result.report_title == null) {				
				$("#report-builder-section-breadcrump").removeClass().addClass("reports-title-div-grey-end-prev");
				$("#report-query-details-breadcrump").removeClass().addClass("reports-title-div-end");
				obj.editstatus = 'edit';
			}
			else {
				$("#report-query-details-breadcrump").removeClass().addClass("reports-title-div-edit-end");
				obj.publish_title = result.report_title;
				obj.editstatus = 'edit';
			}
			if(reportType == 'query_builder'){
				$('#qry_builder_area').removeClass('query-builder-inital-text');
				$('#qry_builder_area').val(result.query_org_data);
				obj.queryTextClick();
			} else {
				var tableSql = reportDetails.table_sql;
				var tableAliasSql = reportDetails.table_alias_sql;
				var columnSql = reportDetails.column_sql;
				var columnAliasSql = reportDetails.column_alias_sql;
				var relatedColumn = reportDetails.where_sql;
				var windowPositions = reportDetails.window_positions;
				
				$('#cart_items ._jsPlumb_endpoint').remove();
				$('#cart_items ._jsPlumb_connector ').remove();

				$("#cart_items > .jspContainer > .jspPane").html('');
				
				$("#listTables").val(tableSql);
				$("#listTablesDisplay").val(tableAliasSql);
				
				var tableSqlArray = tableSql.split(',');
				var tableAliasSqlArray = tableAliasSql.split(',');
				
				for(var j=0; j< tableSqlArray.length; j++){
					var parentTableId = 'tbl_'+tableSqlArray[j];
					var parentAliasTableId = 'tbl_'+tableAliasSqlArray[j];
					
					/*
					$('#rel_tbl_'+parentTableId).val(tableSql);
					$('#rel_alias_tbl_'+parentTableId).val(tableAliasSql);
					$('#rel_fields_tbl_'+parentTableId).val(relatedColumn);
					*/
					
					$("body").data("reportsearch").setJsScrollpane('scroll-pane-drop-container');
					$('body').data('reportsearch').showRelatedTableInContainer(parentTableId, parentAliasTableId);
					$("body").data("reportsearch").setAndRemoveDraggable();
	
					var columnSqlArray = columnSql.split('~~');
					var columnSqlLen = columnSqlArray.length;
					var columnCheckName = '';
					if(currTheme == "expertusoneV2"){
						for(var i=0; i<columnSqlLen; i++){
							columnCheckName = 'check_'+columnSqlArray[i];
							$("#"+jqSelector(columnCheckName)).attr('checked',true);
							$("#"+jqSelector(columnCheckName)).parent().removeClass('checkbox-unselected');				
							$("#"+jqSelector(columnCheckName)).parent().addClass('checkbox-selected');
						}
					}
					else{
						for(var i=0; i<columnSqlLen; i++){
							columnCheckName = 'check_'+columnSqlArray[i];
							$("#"+jqSelector(columnCheckName)).attr('checked',true);
						}
					}
				}

				$('body').data('reportsearch').setWindowPosition(windowPositions);

				//jsPlumb.draggable(jsPlumb.getSelector(".window"));
				jsPlumb.draggable(jsPlumb.getSelector(".window"),{containment: "#cart_items",scroll: false,snapMode: "inner",snapTolerance: 30,snap: "#cart_items"});
				
				//CUSTOM TOOL TIP FOR SHOWING THE RESTRICTED CHARACTED
				vtip();
				
			}
			
      // Alert the user if there is an active schedule
      if (typeof result.has_active_schedule_msg == 'object' && result.has_active_schedule_msg.length > 0) {
        var $renderedMsg = expertus_error_message(result.has_active_schedule_msg, 'warning');
        $('#show_expertus_message').show();
        $('#show_expertus_message').html($renderedMsg);
      }
		} // end ajax success
	}); 
	}catch(e){
 		//Nothing to do
	}
},

/*
 * get the report details from database for third step on report edit page
 */
loadReportDetails: function(){
	try{
	var obj = this;
	obj.createLoader('report-query-details');
	url = obj.constructUrl("administration/report-search/load-report-details/"+obj.report_id);
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		success: function(result){
			obj.destroyLoader('report-query-details');
			$('#set_mainquerform_ReportTitle').val(result.report_title);
			$('#set_mainquerform_ReportDescription').val(result.report_desc);
			obj.reportSelectedCategory = result.report_type;
			
			if(result.report_visibility != '' && result.report_visibility != null){
				var visibilityArray = result.report_visibility.split(",");
				$("#report_visibility").val(visibilityArray);
				// $("select").val(visibilityArray);
			}
			$('#set_report_type').find('option[value="'+obj.reportSelectedCategory+'"]').attr('selected', true);
			
			if(result.theme_name != '' && result.theme_name != null){
				$('#set_mainquerform_Theme_Name').closest('tr').hide();
			}
			$("select").multiselect("refresh");
			if( result.status == 'cre_rpt_rps_atv') {
				$('#edit-catalog-course-save-unpublish').val(Drupal.t('LBL571'));
				$("#edit-catalog-course-save-unpublish").attr("onClick","$(\'body\').data(\'reportsearch\').reportPublishAndUnpublishdetails(\'cre_rpt_rps_itv\')");
				$("#edit-catalog-course-save-publish").attr("onClick","$(\'body\').data(\'reportsearch\').reportPublishAndUnpublishdetails(\'cre_rpt_rps_atv\')");
			}
			//Added for #0070534
			if(repQryState == 'NON_SCALABLE_QUERY'){
				var msg = new Array();
				msg[0] = Drupal.t('ERR256');
				var message_call = expertus_error_message(msg,'error');
				$('#show_expertus_message').show();
				$('#show_expertus_message').html(message_call);
				repQryState = '';
			}
		}
	}); 
	}catch(e){
 		//Nothing to do
	}
},
/* 
 * get report categories, visbility and criteria condition from slt_profile_list_items
 */
getReportCategory: function() {
	try{
	var obj = this;
	url = obj.constructUrl("administration/report-search/load-report-list-items");
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		success: function(result){
			obj.reportCategory = result.category;
			obj.reportVisibility = result.visibility;
			obj.reportCriteriaCondition = result.criteria_condition;
		}
	}); 
	}catch(e){
 		//Nothing to do
	}
},

launchReport: function(reportId, pageFrom){
	try{
	var launch = 1;
	this.directLaunch = 1;
	this.loadReportLayout(reportId, pageFrom, launch);
	$('.filter-addfilter-container').hide();
	if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
	$('#report-builder-section').addClass('launch-report-builder-section');
	$('#report_criteria_form').addClass('report-launch-section');
	$('.report-wizard-topbar').hide();
	$(".report-launch-section").find('.datetime:odd').css("background", "none");
	}
	}catch(e){
 		//Nothing to do
	}
},

loadReportLayout: function(reportId, pageFrom, launch){
	try{
	$('#'+pageFrom).hide();
	$('#report-builder-section').show();
	var obj = this;
	// Hide the breadcrumb on report launch
	if(launch == 1){
		$('.previousLink').hide();
		$('.breadcrump-container').hide();
		$('.nextLink').hide();
		obj.reportLaunch = "launch";
		$.ajax({
			  url:'?q=ajax/launchcount/'+reportId
			}); 
	}
	else {
		obj.reportLaunch = "";
	}
	
	obj.createLoader('report-add-wizard');
	var param = '';
	obj.report_id = reportId;
	url = obj.constructUrl("administration/report-search/load-theme/"+reportId);
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(result){
			var reportDetails = result.report_details;
			
			obj.allThemes = result.all_themes;
			obj.adminAccess = result.admin_access;
			obj.reportCriteria = result.criteria;
			
			obj.theme_name = reportDetails.theme_name;
			obj.report_header_text = reportDetails.report_header_text;
			obj.report_footer_text = reportDetails.report_footer_text;
			obj.report_title = reportDetails.title;
			obj.report_theme_id = reportDetails.theme_id;
			obj.reportBuilderType = reportDetails.report_builder_type;
			
			obj.column_sql = reportDetails.column_sql;
			obj.limit_sql = reportDetails.limi_sql;
			obj.column_alias_sql = reportDetails.column_alias_sql;
			
			obj.header_style = reportDetails.header_style;
			obj.footer_style = reportDetails.footer_style;
			obj.criteria_column_style = reportDetails.criteria_column_style;
			obj.grid_header_style = reportDetails.grid_header_style;
			obj.grid_header_font_style = reportDetails.grid_header_font_style;
			obj.grid_row_style = reportDetails.grid_row_style;
			obj.grid_row_border_style = reportDetails.grid_row_border_style;
			obj.grid_row_font_style = reportDetails.grid_row_font_style;
			obj.grid_alternate_row_style = reportDetails.grid_alternate_row_style;
			obj.grid_alternate_row_font_style = reportDetails.grid_alternate_row_font_style;
			// Ram : this is No need For this Ticket #34161 obj.report_header_text_short = reportDetails.report_header_text_short; 
		}
	}); 
	
	obj.report_editable = 0;
	if(launch == 0 && obj.adminAccess == 1){
		obj.report_editable = 1;
	}

	var allThemeObj = obj.allThemes;
	var allThemeLength = allThemeObj.length;
	
	var html = '';
	var selectedThemeName = (obj.theme_name == '' || obj.theme_name == null) ? 'ExpertusONE Theme' : htmlEntities(obj.theme_name); 
	selectedThemeName = titleRestrictionFadeoutImage(selectedThemeName,'lnrreports-search-selected-themename',16);
	
	html+='<input type="hidden" id="header_style" value="'+obj.header_style+'" />';
	html+='<input type="hidden" id="footer_style" value="'+obj.footer_style+'" />';
	html+='<input type="hidden" id="criteria_column_style" value="'+obj.criteria_column_style+'"/>';
	html+='<input type="hidden" id="grid_header_style" value="'+obj.grid_header_style+'"/>';
	html+='<input type="hidden" id="grid_row_style" value="'+obj.grid_row_style+'"/>';
	html+='<input type="hidden" id="grid_alternate_row_style" value="'+obj.grid_alternate_row_style+'"/>';

	var hideHeader = 0;
	if(obj.report_editable == 0 && obj.report_header_text == 'REPORT HEADER'){ // Default header should not be shown
		if(obj.report_title != null){
			var orgHeaderText = (obj.report_header_text == 'REPORT HEADER') ? obj.report_title : obj.report_header_text;
		} else {
			hideHeader = 1;
		}
	} else {
		var orgHeaderText = obj.report_header_text;
	}
	var classText = 'class=""';
	if(obj.report_editable == 0 ) {
		classText = 'class="vtip"';
	}
	html+='<div id="criteria_errors" class="criteria_errors error"></div>';
	if(hideHeader == 0){
		html+='<div id="report_header">';
			if(obj.report_editable){
				html+='<div id="report_theme_selection">';
				if(obj.currTheme == 'expertusoneV2'){
					var iconCls = 'add-icon';
				}
				else{
					var iconCls = 'theme-drpdwn-icon';
				}
					html+='<div id="selected_theme"><span id="selected_theme_labelclass" class="selected_theme_label">'+titleRestrictionFadeoutImage(selectedThemeName,'lnrreports-search-all-theme-fadeout')+'</span><span id="theme-drpdwn-class" class="'+iconCls+' "></span></div>';
						html+='<div id="list-of-themes"><ul>';
						for(var c = 0; c < allThemeLength; c++){
							/* Added htmlEntities function to avoid compiling javascript for security issue - By Ganeshbabuv, June 26th 2015 8 PM */
							themeNameStr=addslashesForReport(allThemeObj[c].theme_name); 
							html+='<li id="theme-drpdwn-class" class="vtip" title="'+htmlEntities(allThemeObj[c].theme_name)+'" onClick="$(\'body\').data(\'reportsearch\').changeTheme('+allThemeObj[c].id+', \''+themeNameStr+'\');">'+titleRestrictionFadeoutImage(allThemeObj[c].theme_name,'lnrreports-search-all-theme-fadeout',19)+'</li>';
						}
						html+='</ul></div>';
						var themeStyle = 'style="display: none;"';
				html+='</div>';
				html+='<div id="save_theme"';
				if(obj.theme_name != ''){  
					html += ' style="display:none;" ';
				}
				html+='>';
					html+= '<span id="save_theme_container">';
						html+= '<div class="bottom-qtip-tip-up"></div>';
						html+= '<a class="qtip-close-button" onclick="$(\'#save_theme_container\').hide();"></a>';
						html+= '<table cellspacing="0" cellpadding="0" id="bubble-face-table"><tbody>';
							html+= '<tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr>';
							html+= '<tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
								html+= '<div id="theme_error" class="error"><div>Theme name is required.</div></div>';
								html+= '<div id="theme_message" class="error"></div>';
								
								if(obj.currTheme == 'expertusoneV2'){
								html+='<span class="commanTitleAll">';
								html+=Drupal.t('LBL802')+'</span>:<span style="display:inline-block;" class="mandatory_symbol">&nbsp;*&nbsp;</span> <input type="text" id="new_theme_name" class="theme-name-txtbox" /><br/>';	
								html+='<span class="save-button-container"> <div class="white-btn-bg-left"></div><span class="tip_close white-btn-bg-middle" onclick="$(\'#save_theme_container\').hide();">Close</span><div class="white-btn-bg-right"></div><span class="admin-save-button-left-bg"></span><input type="button" class="admin-save-button-middle-bg" value="'+Drupal.t('LBL141')+'" onClick="$(\'body\').data(\'reportsearch\').saveTheme('+obj.report_id+');" /><span class="admin-save-button-right-bg"></span></span><br/>';
								}else{
									html+=Drupal.t('LBL802')+':<span style="display:inline-block;" class="mandatory_symbol">&nbsp;*&nbsp;</span> <input type="text" id="new_theme_name"/ ><br/>';	
								html+='<span class="save-button-container"> <span class="tip_close" onclick="$(\'#save_theme_container\').hide();">Close</span><span class="admin-save-button-left-bg"></span><input type="button" class="admin-save-button-middle-bg" value="'+Drupal.t('LBL141')+'" onClick="$(\'body\').data(\'reportsearch\').saveTheme('+obj.report_id+');" /><span class="admin-save-button-right-bg"></span></span><br/>';
								}
							html+= '</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-blt"></td>';
							html+= '<td class="bubble-br"></td></tr></tbody></table>';
					html+='</span>';
					html+='<span class="trigger_save">Save Theme</span>';
			html+='</div>';
			}
			html+='<div id="report_header_text" class="individual-item">';
				html+='<span id="report_header_style_container" class="style_container"></span>';
				html+='<div class="text-align-center"><span id="report_header_text_container"><input maxlength="100" type="text" id="report_header_text_input" value="'+htmlEntities(obj.report_header_text)+'" onblur="$(\'body\').data(\'reportsearch\').updateHeaderFooter(\'header\', this.value);" /></span></div>';
				/*-- Fix for #34161: Issue in Reports  No Border For Hover In --*/
				html+='<div class="text-align-center"><span id="report_header_label_container" '+ classText +' title ="'+htmlEntities(orgHeaderText)+'">'+titleRestrictionFadeoutImage(htmlEntities(orgHeaderText),'lnrreports-search-header-label-text')+'</span></div>';
			html+='</div>';
		html+='</div>';
	}	
	
	html+='<div id="report_narrow_filter">';
	//change by ayyappans for 39574: Remove the refine section in the launched report if there are no refine filters set
	var reportClass = '', gridClass = '';
	if(obj.reportLaunch != "launch" || obj.reportCriteria !='' && obj.reportCriteria != undefined) {
		if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
			html+='<div id="report_narrow_filter_items"><div class="clearBoth block-findtraining-left"><div class="block-findtraining-right"><div class="block-findtraining-middle"><div class="root-admin-links-heading">'+Drupal.t('LBL829')+'</div></div></div></div><div class="narrow-search-filters">';
			html+='<div id="report_narrow_filter_items_container">';
			html+='<div class="filter-addfilter-container">';
		}else{
			html+='<div id="report_narrow_filter_items"><div class="clearBoth block-findtraining-left"><div class="block-findtraining-right"><div class="block-findtraining-middle">&nbsp;</div></div></div><div class="narrow-search-filters">';
			html+='<div id="report_narrow_filter_items_container">';
			html+='<div class="filter-addfilter-container"><span class="root-admin-links-heading">'+Drupal.t('LBL829')+'</span>';	
		}
		if(obj.report_editable){
			if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
				html+='<div class="add-filter-container"><div class="narrow-search-actionbar-orange-btnLeft"></div><div class="narrow-search-actionbar-orange-btnBG"><div id="report_add_narrow_filter">'+Drupal.t('LBL789')+'<span id="report_add_narrow_filter_popup" class="style_container"></span></div></div><div class="narrow-search-actionbar-orange-btnRight"></div></div>';
			}else
			{
				html+='<div id="report_add_narrow_filter"><span class="add-narrow-img"></span>'+Drupal.t('LBL789')+'<span id="report_add_narrow_filter_popup" class="style_container"></span></div>';
			}
		}
		html+='<div class="clearBoth"></div></div><form id="report_criteria_form"></form></div>';

		html+='</div>';
		html+='<div class="clearBoth block-footer-left"><div class="block-footer-right"><div class="block-footer-middle">&nbsp;</div></div></div></div>';
		reportClass = '';
		gridClass = '';
	} else {
		html+='<div style="display:none;"><form id="report_criteria_form"></form></div>';
		reportClass = ' no-filters';
		gridClass = 'grid-no-filters';
	}
		html+='<div id="report_grid_section" class="'+gridClass+'">';
		html+='<div class="clearBoth block-findtraining-left"><div class="block-findtraining-right"><div class="block-findtraining-middle">&nbsp;</div></div></div><div class="narrow-search-filters'+reportClass+'">';
		
		if(obj.report_editable){
			if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
			html+='<div class="report-datagrid-setting-tools-container clearBoth">';
			html+='<div title="'+Drupal.t('LBL847')+'" class="vtip query-builder-custom-icon"><div id="report_grid_header_theme_update"></div></div><span id="report_grid_header_style_container" class="style_container"></span>';
			html+='<div title="'+Drupal.t('LBL846')+'" class="vtip query-builder-custom-icon"><div id="report_grid_footer_theme_update"></div></div><span id="report_grid_footer_style_container" class="style_container"></span>';
			html+='<div class="query-builder-custom-icon"><div id="report_grid_preview_data" title="'+Drupal.t('LBL845')+'" class="vtip" onClick="$(\'body\').data(\'reportsearch\').filterAndShowResults();"></div></div>';
			html+='</div>';
			}else
			{	
			html+='<div class="report-datagrid-setting-tools-container clearBoth">';
			html+='<div title="'+Drupal.t('LBL847')+'" class="vtip"><div id="report_grid_header_theme_update"></div></div><span id="report_grid_header_style_container" class="style_container"></span>';
			html+='<div title="'+Drupal.t('LBL846')+'" class="vtip"><div id="report_grid_footer_theme_update"></div></div><span id="report_grid_footer_style_container" class="style_container"></span>';
			html+='<div id="report_grid_preview_data" title="'+Drupal.t('LBL845')+'" class="vtip" onClick="$(\'body\').data(\'reportsearch\').filterAndShowResults();"></div>';
			html+='</div>';
			}
		}
		
		
		if(obj.report_editable == 0){
			html+='<div style="display:none;" id="report-exp-pdf-div" class="report-setting-tools-container-launch clearBoth">';
			html+="<div id='reports-export-xcel-container' class='reports-export-xcel-container'><a onclick=\"$('#lnr-reports-search').data('lnrreportssearch').callReportExportProcess('"+reportId+"','CSV');\" class='vtip reports-export-xcel-icon' title='"+Drupal.t('LBL309')+"'></a></div>";
			html+="<div id='reports-pdf-container' class='reports-pdf-container'><a onclick=\"$('#lnr-reports-search').data('lnrreportssearch').callReportExportProcess('"+reportId+"','PDF');\" class='vtip reports-pdf-icon' title='"+Drupal.t('LBL308')+"'></a></div>";
			html+='</div>';
		}
		
		
		html+='<table id="report_grid_container"></table>';
			if(obj.report_editable == 0){
				html+='<div id="report_result_pager"></div>';
			}
	//html+='</div>';
	html+= '<div id="reports-noresults"></div><div id="reports-noresults-launch"></div></div>';
	html+='<div class="clearBoth block-footer-left"><div class="block-footer-right"><div class="block-footer-middle">&nbsp;</div></div></div></div>';
	html+='<div class="clearBoth"></div>';
	html+='<div id="report_footer">';
	if(obj.report_editable == 1 || (obj.report_footer_text != 'REPORT FOOTER' && obj.report_footer_text != null)){ // Default footer should not be shown
			html+='<div id="report_footer_text" class="individual-item">';
				html+='<span id="report_footer_style_container" class="style_container"></span>';
				html+='<div class="text-align-center"><span id="report_footer_text_container"><input type="text" maxlength="100" id="report_footer_text_input" value="'+htmlEntities(obj.report_footer_text)+'" onblur="$(\'body\').data(\'reportsearch\').updateHeaderFooter(\'footer\', this.value);" /></span></div>';
				html+='<div class="text-align-center"><span id="report_footer_label_container">'+obj.report_footer_text+'</span></div>';
			html+='</div>';
		
	}
	html+='</div>';
	obj.destroyLoader('report-add-wizard');
	$('#report-builder-section').html(html);
	if(obj.reportLaunch != "launch" || obj.reportCriteria !='' && obj.reportCriteria != undefined) {
		this.loadCriteria();
	}
	
	$("#selected_theme").click(function () {
		$("#list-of-themes").toggle();
		var themeliLen=$("#list-of-themes ul").children("li").length;
		if(themeliLen >3){
			$("#list-of-themes").jScrollPane({autoReinitialise: true});
		}
	});	
	// Reset hover border for Launch 
	if(obj.report_editable == 0){
		$("#report_header_label_container").hover(function () {
			$(this).css('border','none');
		});
		$("#report_footer_label_container").hover(function () {
			$(this).css('border','none');
		});
	}
	
	// Close styling pop-up
	$('.tip_close').die('click');
	$('.tip_close').live('click', function(){
		$('span.style_container').empty();
		$('span.style_container').css('display','none');
	});
	
	if(obj.report_editable){
		
		// Close theme dropdown 
		$('body').bind('click', function(event) {
		theme_label = $(event.target).closest('#selected_theme').length;
			if(!(event.target.id == 'theme-drpdwn-class' || theme_label == 1)){
				$('#list-of-themes').hide();
			} 
		});
		// Save theme
		$('#save_theme .trigger_save').click(function(){
			$('#save_theme_container').show();
		});
		
		// Load Narrow Filter
		$('#report_add_narrow_filter').click(function(e){
			if(e.target == this ){ 
				obj.loadNarrowFilter();	
			};
		});
		
		// Begin - Editing report header and footer
		$('#report_header_label_container').click(function(){
			$('#report_header_text_container').show();
			$('#report_header_label_container').hide();
			$('#report_header_text_input').focus();
		});
		$('#report_header_text_input').keydown(function(event){
			if (event.keyCode == 13) {
				event.preventDefault();
				$('#report_header_text_input').blur();
			}
		});
		$('#report_footer_label_container').click(function(){
			$('#report_footer_text_container').show();
			$('#report_footer_label_container').hide();
			$('#report_footer_text_input').focus();
		});
		$('#report_footer_text_input').keydown(function(event){
			if (event.keyCode == 13) {
				event.preventDefault();
				$('#report_footer_text_input').blur();
			}
		});
		// End - Editing report header and footer
		
		// Begin - Styling
		
		var display_timeout = 0;
		$("#report_header_label_container").hover(function () {
			$(".style_container").css({"top": ""});
			$(".style_container").css({"left": ""});
			if(display_timeout != 0) {
					clearTimeout(display_timeout);
			}
			var this_element = this;
	        display_timeout = setTimeout(function() {
	        	display_timeout = 0;
				obj.stylingHeaderFooter('header', 'report_header_style_container');
	        }, 500);
		}, function () {
			if(display_timeout != 0) {
				clearTimeout(display_timeout);
			}
		});
		
		var display_timeout = 0;
		$("#report_footer_label_container").hover(function () {
			$(".style_container").css({"top": ""});
			$(".style_container").css({"left": ""});
			if(display_timeout != 0) {
					clearTimeout(display_timeout);
			}
			var this_element = this;
	        display_timeout = setTimeout(function() {
				display_timeout = 0;
				obj.stylingHeaderFooter('footer', 'report_footer_style_container');
	        }, 500);
		}, function () {
			if(display_timeout != 0) {
				clearTimeout(display_timeout);
			}
		});
	
		var display_timeout = 0;
		$("#report_criteria_form #hide-show-header-txt.editable").die('mouseover');
		$("#report_criteria_form #hide-show-header-txt.editable").live("mouseover", function () {
			var reportCriteriaId = $(this).attr('data');
			if(display_timeout != 0) {
					clearTimeout(display_timeout);
			}
			var this_element = this;
	        display_timeout = setTimeout(function() {
	        	display_timeout = 0;
				obj.stylingHeaderFooter('criteria_column', 'criteria_style_'+reportCriteriaId);
	        }, 500);
		}, function () {
			if(display_timeout != 0) {
				clearTimeout(display_timeout);
			}
		});
		$("#report_criteria_form .narrow-filter-fields-conatiner .criteria-textBox").click( function () {
			$("#report_criteria_form #bubble-face-table").hide();
			$("#report_criteria_form .qtip-tip-point-left").hide();
		});
		
		// End - Styling
		
	}
	
	if(obj.report_editable == 0){
		//Lauch screen UI fix	
		$('#report_header').css('border-top','0px none');
		//change by ayyappans for 39574: Remove the refine section in the launched report if there are no refine filters set
		if(obj.reportLaunch != "launch" || obj.reportCriteria !='' && obj.reportCriteria != undefined) {
			var validateStatus = obj.criteriaValidate(0);
			if(validateStatus != 1){
				$('#reports-noresults-launch').html(Drupal.t('LBL185')+': '+Drupal.t('ERR174')+'.');
				obj.applyThemes();
				return false;
			}
		}
	}

	if(this.directLaunch==1){
		obj.loadResultGrid(1);
	}else{
		obj.loadResultGrid(0);
	}
	//Added for #0070534
	if(repQryState == 'NON_SCALABLE_QUERY'){
		var msg = new Array();
		msg[0] = Drupal.t('ERR256');
		var message_call = expertus_error_message(msg,'error');
		$('#show_expertus_message').show();
		$('#show_expertus_message').html(message_call);
		repQryState = '';
	}
	
	}catch(e){
 		//Nothing to do
	}
	
},
loadResultGrid : function(preview){
	try{
	var obj = this;
	obj.createLoader('report_grid_section');
	
	var els = document.getElementById('report_criteria_form').elements;
	var val = '';
	var elementNameAndValue = '';
	for (var i = 0, len = els.length; i < len; ++i) {
		if (els[i].tagName == "INPUT") {
			val = els[i].value;
			if(els[i].name != '' && els[i].name != undefined){
				elementNameAndValue += els[i].name + '~~' + val + Drupal.settings.custom.EXP_AC_SEPARATOR;
			}
		}
	}
	if(elementNameAndValue == ''){ elementNameAndValue = '0';}
	
	var column_sql = obj.column_sql;
	var column_alias_sql = obj.column_alias_sql;
	
	var columnSqlArray = column_sql.split('~~');
	var columnAliasSqlArray = column_alias_sql.split(',');
	var columnSqlLength = columnSqlArray.length;
	
	var dynamicColModels = new Array();
	var columnAliasSQLReplaced = '';
	var sortableOption = obj.report_editable == 0 ? true : false; 
	for(var c = 0; c < columnSqlLength; c++){
		var colWidth = 120;
		columnAliasSQLReplaced = columnAliasSqlArray[c].split(' ').join('_');
		columnAliasSQLReplaced = $.trim(columnAliasSQLReplaced);
		var calculatedColWidth = $.fn.getTextWidth(columnAliasSQLReplaced.toUpperCase(), obj.grid_header_font_style/* '17px Verdana'*/);
		calculatedColWidth += 20;
		colWidth = calculatedColWidth < colWidth ? colWidth : calculatedColWidth;
		dynamicColModels[c] = {name: columnAliasSQLReplaced, index: columnAliasSqlArray[c], title: true, widgetObj:'', sortable: sortableOption, resizable: true, formatter: obj.displayGridValues, width: colWidth};
	}
	$('#show_expertus_message').hide();
	obj.previewMsg = preview;
	var resultURL = obj.constructUrl('learning/report-result-content/'+obj.report_id+'/'+obj.report_editable+'/'+preview+'&inputNameValues='+encodeURIComponent(elementNameAndValue));
	$('#report_grid_container').jqGrid('GridUnload');
	$('#report_grid_container').jqGrid({
		url: resultURL,
		datatype: "json",
		mtype: 'GET',
		colNames: columnAliasSqlArray,
		colModel: dynamicColModels,
		rowNum:10,
		rowList:[10,25,50],
		pager: '#report_result_pager',
		viewrecords:  true,
		multiselect: false,
		sortable: (obj.report_builder_type == 'design_wizard' && obj.report_editable) ? {update: function(permutation) {
	        	obj.setColumnReorder(permutation);
	    	}} : false,
		emptyrecords: "",
		sortorder: "desc",
		toppager: false,
		botpager: false,
		height: 'auto',
		loadtext: "",
		recordtext: "",
		pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
		loadui:false,
		resizeStop: obj.adjustContentTblWidth,
		loadComplete: obj.callbackDataGrid,
//		beforeRequest: obj.beforeRequest,
		autowidth: true,	//change by ayyappans for 39574: Remove the refine section in the launched report if there are no refine filters set
		shrinkToFit: false,
		loadError : function(xhr,st,err) {
	           //jQuery("#rsperror").html("Type: "+st+"; Response: "+ xhr.status + " "+xhr.statusText);
	    		//if(xhr.statusText.indexOf('Internal Server Error')>=0){
	    			var msg = new Array();
	    			obj.destroyLoader('report_grid_section');
	    			msg[0] = Drupal.t('ERR256');
					var message_call = expertus_error_message(msg,'error');
					$('#show_expertus_message').show();
					$('#show_expertus_message').html(message_call);
	    			obj.gridErrorHandler();
	    		//}
	   },
	   pginput: false,
			// pgbuttons: false
	}).navGrid('#report_result_pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});
	$("#report-add-wizard").dialog({height:'auto'});	
	//$("#report_result_pager").css({"width":"650px","height":"35px"});
	$("#report_result_pager").css({"width":"auto"});
	if(Drupal.settings.ajaxPageState.theme== "expertusone"){
	$("#report_result_pager #data-table-page-view").css("margin-top","5px");
	$("#report_result_pager .ui-pg-table").css("margin-top","5px");
	}	
	$('#reports-noresults-launch').css('display','none');
	}catch(e){
 		//Nothing to do
	}
},

gridErrorHandler: function(){
	try{
		var obj = this;
		var reportId = obj.report_id;
		var param = {'reportId':reportId};
		url = obj.constructUrl("administration/report-query/destroy");
		$.ajax({
			type: "POST",
			dataType: 'json',
			async : true,
			url: url,
			data:  param,
			success: function(results){
				// do nothing
			}
		}); 
	}catch(e){
 		//Nothing to do
	}
},

setColumnReorder: function(permutation){
	try{
	var obj = this;
	var reportId = obj.report_id;
	var param = {'reorder':permutation,'reportId':reportId};
	obj.createLoader('report_grid_section');
	url = obj.constructUrl("administration/report-column/reorder");
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(results){
			obj.destroyLoader('report_grid_section');
			obj.column_sql = results.column_sql;
			obj.column_alias_sql = results.column_alias_sql;
		}
	}); 
	}catch(e){
 		//Nothing to do
	}
},

beforeRequest: function(){
	try{
	//change by ayyappans for 39574: Remove the refine section in the launched report if there are no refine filters set
/*		var obj = $('body').data('reportsearch');
		if(obj.reportLaunch != "launch" || obj.reportCriteria != undefined && obj.reportCriteria != '') {
			$('#gbox_report_grid_container').css('width','650px');
		}
		else {
			$('#gbox_report_grid_container').css('width','99%');
		}
*/	}catch(e){
		//Nothing to do
	}
},

adjustContentTblWidth: function() {
 try{	
  var tableWidth = $("#gview_report_grid_container table.ui-jqgrid-htable").width();
  $("#gview_report_grid_container .ui-jqgrid-bdiv").css('width',+tableWidth+'px');
 }catch(e){
		//Nothing to do
	}
},

changeTheme: function(themeId, themeName){
	try{
	var obj = this;
	var param = '';
	themeName = titleRestrictionFadeoutImage(themeName, 'lnrreports-search-selected-themename','16');
	$('#selected_theme .selected_theme_label').html(themeName);
	obj.createLoader('report-builder-section');
	url = obj.constructUrl("administration/report-search/change-theme/"+obj.report_id+"/"+themeId);
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(data){
			if(data.result == 'updated'){
				
				$('#list-of-themes').hide();
				var reportDetails = data.report_details;
				
				obj.report_theme_id = reportDetails.theme_id;
				
				obj.header_style = reportDetails.header_style;
				obj.footer_style = reportDetails.footer_style;
				obj.criteria_column_style = reportDetails.criteria_column_style;
				obj.grid_header_style = reportDetails.grid_header_style;
				obj.grid_header_font_style = reportDetails.grid_header_font_style;
				obj.grid_row_style = reportDetails.grid_row_style;
				obj.grid_row_border_style = reportDetails.grid_row_border_style;
				obj.grid_row_font_style = reportDetails.grid_row_font_style;
				obj.grid_alternate_row_style = reportDetails.grid_alternate_row_style;
				obj.grid_alternate_row_font_style = reportDetails.grid_alternate_row_font_style;
				
				obj.applyThemes();
			}
			obj.destroyLoader('report-builder-section');
		}
	}); 
	}catch(e){
 		//Nothing to do
	}
},

saveTheme: function(reportId, themeId){
	try{
	var newThemeName = $('#new_theme_name').val();
	$('#theme_error').hide();
	if(newThemeName.replace(/\s/g,"") == ''){
		$('#theme_error').show();
		return false;
	}
	var obj = this;	
	obj.createLoader('save_theme');
	url = obj.constructUrl("administration/report-search/save-theme/"+obj.report_id+"/"+newThemeName);
	var param = {'newThemeName':newThemeName};
	$.ajax({
		type: "POST",
		dataType: 'json',
		data:  param,
		url: url,
		success: function(data){
			obj.destroyLoader('save_theme');
			
			if(data.result == 'duplicate'){
				
				$('#theme_message').html(Drupal.t('MSG513'));
				$('#theme_message').show();
				
			} else if(data.result == 'created'){
			newThemeName = titleRestrictionFadeoutImage(newThemeName,'lnrreports-search-selected-themename', '16');
				$('#selected_theme .selected_theme_label').html(newThemeName);
				
				$('#save_theme').hide();
				$('#save_theme_container').hide();
				$('#save_theme_container input:text').val('');
				
				obj.allThemes = data.all_themes;
				var allThemeObj = obj.allThemes;
				var allThemeLength = allThemeObj.length;
				var html = '';
				//html='<ul>';
				for(var c = 0; c < allThemeLength; c++){
					themeNameStr=addslashesForReport(allThemeObj[c].theme_name); 
					html+='<li id="theme-drpdwn-class" class="vtip" title="'+htmlEntities(allThemeObj[c].theme_name)+'" onClick="$(\'body\').data(\'reportsearch\').changeTheme('+allThemeObj[c].id+', \''+themeNameStr+'\');">'+titleRestrictionFadeoutImage(allThemeObj[c].theme_name,'lnrreports-search-all-theme-fadeout',19)+'</li>';
				}
				//html+='</ul>';
				
				$('#list-of-themes ul').html(html);
				vtip();
			}
		}
	}); 
	}catch(e){
 		//Nothing to do
	}
},

styleDropDownList: function(selectedFont, selectedFontSize, selectedFontStyle,type){
	try{
	var obj = this;
	var font_list = new Array('Arial', 'Helvetica', 'Verdana');
	if(type == 'criteria_column'){
	   var font_size = new Array('1', '14');
	}
	else{
	   var font_size = new Array('1', '25');
	}
	var font_style = new Array('Normal', 'Italic');
	
	// Font
	var font_list_dropdown = '';
	var selectedItem = '';
	$.each(font_list, function(key, value){
		selectedItem = value == selectedFont ? 'selected' : '';
		font_list_dropdown += '<option value="'+value+'" '+selectedItem+'>'+value+'</option>';
	});

	// Font Size
	var font_size_dropdown = '';
	selectedItem = '';
	for(var c = font_size[0]; c <= font_size[1]; c++){
		selectedItem = c+'px' == selectedFontSize ? 'selected' : '';
		font_size_dropdown += '<option value="'+c+'px" '+selectedItem+' >'+c+'</option>';
	}
	
	// Font Style
	var font_style_dropdown = '';
	selectedItem = '';
	$.each(font_style, function(key, value){
		selectedItem = value == selectedFontStyle ? 'selected' : '';
		font_style_dropdown += '<option value="'+value+'" '+selectedItem+'>'+value+'</option>';
	});
	
	var styleResult = new Array();
	styleResult['font_list_dropdown'] = font_list_dropdown;
	styleResult['font_size_dropdown'] = font_size_dropdown;
	styleResult['font_style_dropdown'] = font_style_dropdown;
	
	return styleResult;
	}catch(e){
 		//Nothing to do
	}
},

stylingHeaderFooter: function(type, container){
	try{
	var obj = this;
	var param = '';
	if($('#'+container).is(':visible') == false){
		$('.tip_close').click();
		obj.createLoader(container);
		url = obj.constructUrl("administration/report-search/load-style/"+obj.report_id+"/"+type);
		$.ajax({
			type: "POST",
			dataType: 'json',
			async : false,
			url: url,
			data:  param,
			success: function(data){
				
				var styleDropDownStyleList =  obj.styleDropDownList(data.fontfamily, data.fontsize, data.fontstyle,type);
				
				var html = '';
				$('#'+container).html(html);
				if(container == 'report_grid_header_style_container' || container == 'report_grid_footer_style_container') {
					html+='<div class="qtip-tip-point-right"></div>';
				} else {
					html+='<div class="qtip-tip-point-left"></div>';
				}
				html+='<table cellspacing="0" cellpadding="0" id="bubble-face-table" class="bubble-table-container">';
				html+='<tbody>';
				html+='<tr>';
				html+='<td class="bubble-tl"></td>';
				html+='<td class="bubble-t"></td>';
				html+='<td class="bubble-tr"></td>';
				html+='</tr>';
				html+='<tr>';
				html+='<td class="bubble-cl"></td>';
				html+='<td valign="top" class="bubble-c">';
				
				html+= '<div class="tip_close admin-bubble-close">&nbsp;</div>';
				html+= '<div>';
					html += '<span class="label">'+Drupal.t('LBL791')+':</span><span class="input">';
					if(obj.currTheme == 'expertusoneV2'){
						html += '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon ">';
					}
					html += '<select id="'+type+'-font-list">'+styleDropDownStyleList['font_list_dropdown']+'</select>';
					if(obj.currTheme == 'expertusoneV2'){
						html += '</div></div>';
					}
					html += '</span><div class="clearBoth"></div>';
				html+= '</div>';
				html+= '<div>';
					html += '<span class="label">'+Drupal.t('LBL792')+':</span><span class="input">';
					if(obj.currTheme == 'expertusoneV2'){
						html += '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon ">';
					}
					html += '<select id="'+type+'-font-size">'+styleDropDownStyleList['font_size_dropdown']+'</select>';
					if(obj.currTheme == 'expertusoneV2'){
						html += '</div></div>';
					}
					html += '</span><div class="clearBoth"></div>';
				html+= '</div>';
				html+= '<div>';
					html += '<span class="label">'+Drupal.t('LBL793')+':</span><span class="input">';
					if(obj.currTheme == 'expertusoneV2'){
						html += '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon ">';
					}
					html += '<select id="'+type+'-font-style">'+styleDropDownStyleList['font_style_dropdown']+'</select>';
					if(obj.currTheme == 'expertusoneV2'){
						html += '</div></div>';
					}
					html += '</span><div class="clearBoth"></div>';
				html+= '</div>';
				html+= '<div>';
					html+= '<span class="label">'+Drupal.t('LBL794')+':</span><span class="input"><input type="text" id="'+type+'-font-color" size="10" value="'+data.color+'" /><input id="'+type+'-font-color-picker" type="text" value="'+data.color+'" /></span><div class="clearBoth"></div>';
				html+= '</div>';
				if(type == 'grid_header' || type == 'grid_row'){
					html+= '<div>';
						html+= '<span class="label">'+Drupal.t('LBL795')+':</span><span class="input"><input type="text" id="'+type+'-background-color" size="10" value="'+data.backgroundcolor+'" /><input id="'+type+'-background-color-picker" type="text" value="'+data.backgroundcolor+'" /></span><div class="clearBoth"></div>';
					html+= '</div>';
					html+= '<div>';
						html+= '<span class="label">'+Drupal.t('LBL797')+':</span><span class="input"><input type="text" id="'+type+'-border" size="10" value="'+data.border+'" /><input id="'+type+'-border-picker" type="text" value="'+data.border+'" /></span><div class="clearBoth"></div>';
					html+= '</div>';
					html+= '<div>';
						html+= '<span class="label">'+Drupal.t('LBL798')+':</span><span class="input"><input type="text" id="'+type+'-line-height" size="10" value="'+data.lineheight+'" /></span><div class="clearBoth"></div>';
					html+= '</div>';
				}
				if(type == 'grid_row'){
					html+= '<div>';
						html+= '<span class="label">'+Drupal.t('LBL796')+':</span><span class="input"><input type="text" id="'+type+'-alt-background-color" size="10" value="'+data.backgroundaltcolor+'" /><input id="'+type+'-alt-background-color-picker" type="text" value="'+data.backgroundaltcolor+'" /></span><div class="clearBoth"></div>';
					html+= '</div>';
				}
				html+= '<div>';
					if(obj.currTheme == 'expertusoneV2'){
						html += '<span class="apply_label"><div class="white-btn-bg-left"></div><span class="tip_close white-btn-bg-middle">'+Drupal.t('LBL123')+'</span><div class="white-btn-bg-right"></div>';
					}
					else{
						html += '<span class="apply_label"><span class="tip_close">'+Drupal.t('LBL123')+'</span>';
					}
					html += '<div class="admin-save-button-left-bg"></div><input type="button" value="'+Drupal.t('LBL725')+'" class="admin-save-button-middle-bg" onclick="$(\'body\').data(\'reportsearch\').updateHeaderFooterStyle(\''+type+'\', \''+container+'\'); return false;" /><div class="admin-save-button-right-bg"></div></span><div class="clearBoth"></div>';
				html+= '</div>';
				html+='</td>';
				html+='<td class="bubble-cr"></td>';
				html+='</tr>';
				html+='<tr>';
				html+='<td class="bubble-bl"></td>';
				html+='<td class="bubble-b">';
				html+='<table width="100%" cellspacing="0" cellpadding="0">';
				html+='<tbody>';
				html+='<tr>';
				html+='<td class="bubble-blt"></td>';
				html+='<td class="bubble-blr"></td>';
				html+='</tr>';
				html+='</tbody>';
				html+='</table>';
				html+='</td>';
				html+='<td class="bubble-br"></td>';
				html+='</tr>';
				html+='</tbody>';
				html+='</table>';
				$('#'+container).html(html);
				$('#'+container).show();
				
				// Font Color
			    $('#'+type+'-font-color-picker').colorPicker();
			    $('#'+type+'-font-color-picker').change(function(){
			    	var selectedColor = $(this).val();
			    	$('#'+type+'-font-color').val(selectedColor);
			    });
			    $('#'+type+'-font-color').keyup(function(){
			    	$('#'+type+'-font-color-picker').next().css("background-color", $(this).val());
			    });

			    // Background Color
			    $('#'+type+'-background-color-picker').colorPicker();
			    $('#'+type+'-background-color-picker').change(function(){
			    	var selectedColor = $(this).val();
			    	$('#'+type+'-background-color').val(selectedColor);
			    });
			    $('#'+type+'-background-color').keyup(function(){
			    	$('#'+type+'-background-color-picker').next().css("background-color", $(this).val());
			    });

			    // Alternate Row Background Color
			    $('#'+type+'-alt-background-color-picker').colorPicker();
			    $('#'+type+'-alt-background-color-picker').change(function(){
			    	var selectedColor = $(this).val();
			    	$('#'+type+'-alt-background-color').val(selectedColor);
			    });
			    $('#'+type+'-alt-background-color').keyup(function(){
			    	$('#'+type+'-alt-background-color-picker').next().css("background-color", $(this).val());
			    });
			    
			    // Border Color
			    $('#'+type+'-border-picker').colorPicker();
			    $('#'+type+'-border-picker').change(function(){
			    	var selectedColor = $(this).val();
			    	$('#'+type+'-border').val(selectedColor);
			    });
			    $('#'+type+'-border').keyup(function(){
			    	$('#'+type+'-border-picker').next().css("background-color", $(this).val());
			    });
			    
				obj.destroyLoader(container);
			}
		}); 	
	}
	//alert(type +'||'+ container);
	if(type == "header") {
		var elePos 		= $('#report_header_label_container').position();
		var eleWidth 	= $('#report_header_label_container').width();
		$('#'+container).css({'left':Math.round(elePos.left)+Math.round(eleWidth)+25});
	}
	}catch(e){
 		//Nothing to do
	}
},

updateHeaderFooter: function(type, value){
	try{
	var obj = this;
	var param = '';
	
	if(type == "header" && value == ""){		
		value = $("#report_header_label_container").text();
		$('#report_header_text_input').val(value);
	}else if(type == "footer" && value == ""){
		value = $("#report_footer_label_container").text();
		$('#report_footer_text_input').val(value);
	}	
	obj.createLoader('report_'+type);
	newvalue = value.replace(/\//g,Drupal.settings.custom.EXP_AC_SEPARATOR);
	url = obj.constructUrl("administration/report-search/update-header-footer/"+obj.report_id+"/"+type+"/"+encodeURIComponent(newvalue));
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(data){
			obj.destroyLoader('report_'+type);
			if(data.result == 'updated'){
				obj.displayThemeSave();
				$('#report_'+type+'_text_container').hide();
				$('#report_'+type+'_label_container').html(value);
				$('#report_'+type+'_label_container').show();
			}
		}
	}); 	
	}catch(e){
 		//Nothing to do
	}
},
		
updateHeaderFooterStyle: function(type, container){
	try{
	var obj = this;
	var param = '';

	var fontlist = $('#'+container+' #'+type+'-font-list').val();
	var fontsize = $('#'+container+' #'+type+'-font-size').val();
	var fontstyle = $('#'+container+' #'+type+'-font-style').val();
	var fontcolor = $('#'+container+' #'+type+'-font-color').val();
	fontcolor = fontcolor.replace('#','*');

	var value = fontlist+'$$'+fontsize+'$$'+fontstyle+'$$'+fontcolor;

	if(type == 'grid_header' || type == 'grid_row'){
		var backgroundcolor = $('#'+container+' #'+type+'-background-color').val();
		var border = $('#'+container+' #'+type+'-border').val();
		backgroundcolor = backgroundcolor.replace('#','*');
		border = border.replace('#','*');
		var lineheight = $('#'+container+' #'+type+'-line-height').val();
		value += '$$'+backgroundcolor+'$$'+border+'$$'+lineheight;
	}
	if(type == 'grid_row'){
		var altbackgroundcolor = $('#'+container+' #'+type+'-alt-background-color').val();
		altbackgroundcolor = altbackgroundcolor.replace('#','*');
		value += '$$'+altbackgroundcolor;
	}
	
	obj.createLoader(container);
	url = obj.constructUrl("administration/report-search/update-header-footer-style/"+obj.report_id+"/"+type+"/"+value);
	$.ajax({
		type: "POST",
		dataType: 'json',
		url: url,
		data:  param,
		success: function(data){
			obj.displayThemeSave();
			obj.destroyLoader(container);
			$('#'+container).html('');
			$('#'+container).hide();
			$('#save_theme').show();
			if(type == 'grid_header'){
				$('#gview_report_grid_container div:nth-child(2)').attr('style', data.style2);
				$('#gview_report_grid_container .ui-state-default div').attr('style', data.style1);
			} else if(type == 'grid_row'){
				$('#report_grid_container tr').attr('style', data.style3);
				$('#report_grid_container tr:odd > td').attr('style', data.style1);
				$('#report_grid_container tr:even > td').attr('style', data.style2);
			} else if(type == 'criteria_column'){
				$('#report_criteria_form .label').attr('style', data.style1);
			} else {
				$('#report_'+type+'_label_container').attr('style', data.style1);
			}
			var stylegridWidth = $('#gview_report_grid_container .ui-jqgrid-bdiv').width();
			$('#gview_report_grid_container .ui-jqgrid-hdiv').css('width',stylegridWidth+'px');
			return false;
		}
	});
	return false;
	}catch(e){
 		//Nothing to do
	}
},

displayThemeSave : function(){
	try{
		
	}catch(e){
 		//Nothing to do
	}
},

displayGridValues : function(cellvalue, options, rowObject){
	try{
	var index  = options.colModel.index;
	return (rowObject[index] == 'undefined' || rowObject[index] == null) ? '' : rowObject[index];
	}catch(e){
 		//Nothing to do
	}
},

applyThemes: function(){
	try{
	var obj = $('body').data('reportsearch');
	$('#report_header_label_container').attr('style', obj.header_style);
	// For Reports Old theme Border Issue #34161
	$('#report_header_label_container').css('border','medium none');
	$('#report_footer_label_container').attr('style', obj.footer_style);
	
	$('#gview_report_grid_container div:nth-child(2)').attr('style', obj.grid_header_style);
	$('#gview_report_grid_container .ui-state-default div').attr('style', obj.grid_header_font_style);
	var borderRow = "border-bottom:"+obj.grid_row_border_style;
	var borderLeftRight = $('#report_grid_container').attr('style')+";"+obj.grid_row_border_style+";";
	$('#report_grid_container').attr('style', borderLeftRight);
	$('#report_grid_container tr:not(.jqgfirstrow)').attr('style', borderRow);
	$('#report_grid_container tr:odd:not(.jqgfirstrow) > td').attr('style', obj.grid_row_style);
	$('#report_grid_container tr:even:not(.jqgfirstrow) > td').attr('style', obj.grid_alternate_row_style);
//	$('#report_grid_container tr > td').css('padding', '0');	//do not remove this line to keep the alignment of header and rows
	$('#report_grid_container tr > td').css('padding-right', '5');
	
	//$('#report_grid_container tr:first-child').attr('style', 'display:none');
	}catch(e){
 		//Nothing to do
	}
},

modifierFormHtml : function(criteria,firstDateCriteriaId) {
 try{	
  var mandatory  = criteria.mandatory;
  var userfilter = criteria.use_user_details;
  var readOnly   = '';
  var mandatoryChkClass ='checkbox-unselected';
  var userfilterChkClass ='checkbox-unselected';
  if(userfilter == 1){
    mandatory = 1;
    readOnly = 'disabled';
    userfilterChkClass = 'checkbox-selected';
  }
  if(mandatory ==1){
    mandatoryChkClass  = 'checkbox-selected';
  }
	var obj = this;
	var html = '';
	html+='<div id="buble-face-modifier-'+criteria.id+'"><div class="qtip-tip-point-left"></div>';
	html+='<table cellspacing="0" cellpadding="0" id="bubble-face-table" class="bubble-table-container bubble-modifier-container" border="0">';
	html+='<tbody>';
	html+='<tr>';
	html+='<td class="bubble-tl"></td>';
	html+='<td class="bubble-t"></td>';
	html+='<td class="bubble-tr"></td>';
	html+='</tr>';
	html+='<tr>';
	html+='<td class="bubble-cl"></td>';
	html+='<td valign="top" class="bubble-c">';
	html+= '<div class="modifier_tip_close admin-bubble-close" onclick="$(\'body\').data(\'reportsearch\').modifiersClosePopup('+criteria.id+');" >&nbsp;</div>';
	if(criteria.column_type != 'datetime' && criteria.column_type != 'date') {	
		html+= '<div class="conditiontype">';
		html += '<span class="modifierlabel">'+Drupal.t("LBL1162")+':</span><span class="input">';//Condition
		if(obj.currTheme == 'expertusoneV2'){
				html += '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon ">';
		}
		var selectedItem ='';
		var  hiddencondtype  ='';
		html += '<select id="condition-filter-' + criteria.id + '" ' + readOnly + '>';
		if(criteria.condition_type == 'cre_rpt_cnd_con'){
		selectedItem = 'selected';
		hiddencondtype ='cre_rpt_cnd_con';
		}
	    html += '<option id="list-box-filter-input-contains-'+criteria.id+'" value ="cre_rpt_cnd_con" '+selectedItem+' >'+Drupal.t("LBL1141")+'</option>';
	    selectedItem = '';
	    if(criteria.condition_type == 'cre_rpt_cnd_equ'){
	    	selectedItem = 'selected';
	    	hiddencondtype ='cre_rpt_cnd_equ';
	    }
	    html += '<option id="list-box-filter-input-equal-'+criteria.id+'"  value="cre_rpt_cnd_equ" '+selectedItem+'>'+Drupal.t("LBL1142")+'</option>';
	    selectedItem = '';
	    if(criteria.condition_type == 'cre_rpt_cnd_sta'){
	    	selectedItem = 'selected';
	    	hiddencondtype ='cre_rpt_cnd_sta';
	    }
		html += '<option id="list-box-filter-input-startswith-'+criteria.id+'" value ="cre_rpt_cnd_sta" '+selectedItem+'>'+Drupal.t("LBL1143")+'</option>';
		html +=	'</select>';
		html +=	'<span style="display:none;" id="hiddencondtype-'+criteria.id+'">'+hiddencondtype+'</span>';
	
		if(obj.currTheme == 'expertusoneV2'){
		  html += '</div></div>';
		
		}
		html += '</span><div class="clearBoth"></div>';
		html+= '</div>';
	}

	// User Filter Logic Show/hide
		var filterName = ["full_name", "user_name","last_name","first_name","survey_username","instructor_fullname","manager_user_name","enrolled_username","manager_fullname","managername", "managerfullname", "fullname", "username", "lastname", "firstname", "managerusername","userdottedmanagername","userdottedmanagerusername"];

		var sepcolname = criteria.column_name.split(".");
		var splitvalue = sepcolname.length > 1 ? sepcolname[1].toLowerCase() : sepcolname[0].toLowerCase();
		var userFilterHtml = '';
		if($.inArray(splitvalue, filterName) > -1){
			 userFilterHtml+= '<div>';
			 userFilterHtml+= '<span class="modifierlabel mandatory-check">'+Drupal.t("LBL1161")+':</span>'; //Use Login Details
			 userFilterHtml+= '<span class="input">';
			 userFilterHtml+= '<label class="'+userfilterChkClass+'">';
			 userFilterHtml+= '<input type="checkbox" id="user-filter-'+criteria.id+'"  value="'+userfilter+'" onclick="$(\'body\').data(\'reportsearch\').mandatoryModifiersCheck('+criteria.id+');"/>';
			 userFilterHtml+= '</label>';
			 userFilterHtml+= '</span>';
			 userFilterHtml+= '<div class="clearBoth"></div>';
			 userFilterHtml +=	'<span style="display:none;" id="hiddenuserfilter-'+criteria.id+'">'+userfilter+'</span>';
		}	  	
	html+= '<div>';
	html+= '<span class="modifierlabel mandatory-check">'+Drupal.t('LBL1147')+':</span>';
	html+= '<span class="input">';
	html+= '<label class="'+mandatoryChkClass+'">';
	html+= '<input type="checkbox" id="mandatory-'+criteria.id+'"  value="'+mandatory+'" '+readOnly+' onclick="$(\'body\').data(\'reportsearch\').mandatoryModifiersCheck('+criteria.id+');" />';
	html+= '</label>';
	html+= '</span>';
	html+= '<div class="clearBoth"></div>';
    html+= '</div>';
    html+= '<span style="display:none;" id="hiddenmandatory-'+criteria.id+'">'+mandatory+'</span>';
    html+= userFilterHtml;
    html+= '</div>';
	html+= '<div>';
		if(obj.currTheme == 'expertusoneV2'){
			html += '<span class="apply_label"><div class="white-btn-bg-left"></div><span class="modifier_tip_close white-btn-bg-middle" onclick="$(\'body\').data(\'reportsearch\').modifiersClosePopup('+criteria.id+');">'+Drupal.t('LBL123')+'</span><div class="white-btn-bg-right"></div>';
		}
		else{
			html += '<span class="apply_label"><span class="tip_close" onclick="$(\'body\').data(\'reportsearch\').modifiersClosePopup('+criteria.id+');">'+Drupal.t('LBL123')+'</span>';
		}
		html += '<div class="admin-save-button-left-bg"></div>';
		html +=	'<input type="button" value="'+Drupal.t('LBL725')+'" class="admin-save-button-middle-bg" data-criteria =\''+escape(JSON.stringify(criteria))+'\' onclick="$(\'body\').data(\'reportsearch\').updateCriteriaModifiers(this,'+firstDateCriteriaId+'); return false;" />';
		html += '<div class="admin-save-button-right-bg"></div></span><div class="clearBoth"></div>';
	html+= '</div>';
	html+='</td>';
	html+='<td class="bubble-cr"></td>';
	html+='</tr>';
	html+='<tr>';
	html+='<td class="bubble-bl"></td>';
	html+='<td class="bubble-b">';
	html+='<table width="100%" cellspacing="0" cellpadding="0">';
	html+='<tbody>';
	html+='<tr>';
	html+='<td class="bubble-blt"></td>';
	html+='<td class="bubble-blr"></td>';
	html+='</tr>';
	html+='</tbody>';
	html+='</table>';
	html+='</td>';
	html+='<td class="bubble-br"></td>';
	html+='</tr>';
	html+='</tbody>';
	html+='</table>';
	html+='</div>';
	return html;
 }catch(e){
		//Nothing to do
	}
},
mandatoryModifiersCheck: function(criteriaId) {
	try{
	if($('#user-filter-'+criteriaId).attr("checked")==true) {
		$('#mandatory-'+criteriaId).attr('checked',true);
		$('#mandatory-'+criteriaId).attr('disabled', true);
		$('#condition-filter-'+criteriaId).val('cre_rpt_cnd_equ');
		$('#condition-filter-'+criteriaId).attr('disabled', true);
		$('#user-filter-'+criteriaId).parent().removeClass('checkbox-unselected');				
		$('#user-filter-'+criteriaId).parent().addClass('checkbox-selected');
	}else {
		$('#mandatory-'+criteriaId).removeAttr("disabled");
		$('#condition-filter-'+criteriaId).removeAttr('disabled');
		$('#user-filter-'+criteriaId).parent().removeClass('checkbox-selected');				
		$('#user-filter-'+criteriaId).parent().addClass('checkbox-unselected');
	} 
	if($('#mandatory-'+criteriaId).attr("checked")==true) {
		$('#mandatory-'+criteriaId).parent().removeClass('checkbox-unselected');				
		$('#mandatory-'+criteriaId).parent().addClass('checkbox-selected');
	}else{
		$('#mandatory-'+criteriaId).parent().removeClass('checkbox-selected');				
		$('#mandatory-'+criteriaId).parent().addClass('checkbox-unselected');
	}
	}catch(e){
 		//Nothing to do
	}
},
updateCriteriaModifiers : function (trigger,firstDateCriteriaId) {
	try{
	var criteriaJson = unescape($(trigger).data('criteria'));
	var isDate = 0;
	//console.log(criteriaJson);
	var criteria = eval('(' + criteriaJson + ')'); 
	//console.log(criteria);
	var criteriaId 		= criteria.id;
	var conditionFilter ='';
	if(criteria.column_type != 'datetime' && criteria.column_type != 'date') {
		   conditionFilter = $('#condition-filter-'+criteriaId).val();
	}else{
		isDate = 1; 
	}  
	
	
	var mandatory =$('#mandatory-'+criteriaId).is(':checked') ? 1 : 0;
	var originalMandatory = mandatory;
	var userfilter = 0;
	if($('#user-filter-'+criteriaId).length > 0){
		userfilter = $('#user-filter-'+criteriaId).is(':checked')? 1: 0;
	}
	if(userfilter == 1){
		mandatory = 0;
	}
	var obj = this;
	var param = '';
	obj.createLoader('buble-face-modifier-'+criteriaId);
	url = obj.constructUrl("administration/report-search/update-criteria-modifiers/"+obj.report_id+"/"+criteriaId+"|"+firstDateCriteriaId+"/"+isDate+"/"+conditionFilter+"/"+mandatory+"/"+userfilter);
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(data){
			//Added for #0070534
			var msg = new Array();
			if(data.result == 'NON_SCALABLE_QUERY'){
				msg[0] = data.status == 'cre_rpt_rps_dft' ? Drupal.t('ERR255') : Drupal.t('ERR256');
				var message_call = expertus_error_message(msg,'error');
				$('#show_expertus_message').show();
				$('#show_expertus_message').html(message_call);
			}
			obj.destroyLoader('buble-face-modifier-'+criteriaId);
			$('#modifier_id_'+criteriaId).hide();
			if(criteria.column_type != 'datetime' && criteria.column_type != 'date') {
			  $('#hiddencondtype-'+criteriaId).html(conditionFilter);
			}
			$("#mandatory_"+criteriaId).val(mandatory);
			$("#username_filter_" + criteriaId).val(userfilter);
			$('#hiddenmandatory-'+criteriaId).html(originalMandatory);
			if($('#user-filter-'+criteriaId).length > 0){
			   $('#hiddenuserfilter-'+criteriaId).html(userfilter);
			}
		   if(mandatory ==1 || userfilter ==1) {
			   if(criteria.column_type == 'datetime' || criteria.column_type == 'date') {
				   $('#mandatory-symbol-'+firstDateCriteriaId).css('display','inline-block');
			   }else{
				   $('#mandatory-symbol-'+criteriaId).css('display','inline-block');
			   }
		   }else{
			   if(criteria.column_type == 'datetime' || criteria.column_type == 'date') {
				   $('#mandatory-symbol-'+firstDateCriteriaId).css('display','none');
			   }else{
			       $('#mandatory-symbol-'+criteriaId).css('display','none');
			   }
		   }
		},
		failure: function(data){
			var msg = new Array();
			obj.destroyLoader('buble-face-modifier-'+criteriaId);
			msg[0] = Drupal.t('ERR256');
			var message_call = expertus_error_message(msg,'error');
			$('#show_expertus_message').show();
			$('#show_expertus_message').html(message_call);
			
			$('#modifier_id_'+criteriaId).hide();
			if(criteria.column_type != 'datetime' && criteria.column_type != 'date') {
			  $('#hiddencondtype-'+criteriaId).html(conditionFilter);
			}
			$("#mandatory_"+criteriaId).val(mandatory);
			$("#username_filter_" + criteriaId).val(userfilter);
			$('#hiddenmandatory-'+criteriaId).html(originalMandatory);
			if($('#user-filter-'+criteriaId).length > 0){
			   $('#hiddenuserfilter-'+criteriaId).html(userfilter);
			}
		   if(mandatory ==1 || userfilter ==1) {
			   if(criteria.column_type == 'datetime' || criteria.column_type == 'date') {
				   $('#mandatory-symbol-'+firstDateCriteriaId).css('display','inline-block');
			   }else{
				   $('#mandatory-symbol-'+criteriaId).css('display','inline-block');
			   }
		   }else{
			   if(criteria.column_type == 'datetime' || criteria.column_type == 'date') {
				   $('#mandatory-symbol-'+firstDateCriteriaId).css('display','none');
			   }else{
			       $('#mandatory-symbol-'+criteriaId).css('display','none');
			   }
		   }
		   
			obj.gridErrorHandler();
		},
  		error:function (XMLHttpRequest, textStatus, errorThrown) {
			var msg = new Array();
			obj.destroyLoader('buble-face-modifier-'+criteriaId);
			msg[0] = Drupal.t('ERR256');
			var message_call = expertus_error_message(msg,'error');
			$('#show_expertus_message').show();
			$('#show_expertus_message').html(message_call);
			
			$('#modifier_id_'+criteriaId).hide();
			if(criteria.column_type != 'datetime' && criteria.column_type != 'date') {
			  $('#hiddencondtype-'+criteriaId).html(conditionFilter);
			}
			$("#mandatory_"+criteriaId).val(mandatory);
			$("#username_filter_" + criteriaId).val(userfilter);
			$('#hiddenmandatory-'+criteriaId).html(originalMandatory);
			if($('#user-filter-'+criteriaId).length > 0){
			   $('#hiddenuserfilter-'+criteriaId).html(userfilter);
			}
		   if(mandatory ==1 || userfilter ==1) {
			   if(criteria.column_type == 'datetime' || criteria.column_type == 'date') {
				   $('#mandatory-symbol-'+firstDateCriteriaId).css('display','inline-block');
			   }else{
				   $('#mandatory-symbol-'+criteriaId).css('display','inline-block');
			   }
		   }else{
			   if(criteria.column_type == 'datetime' || criteria.column_type == 'date') {
				   $('#mandatory-symbol-'+firstDateCriteriaId).css('display','none');
			   }else{
			       $('#mandatory-symbol-'+criteriaId).css('display','none');
			   }
		   }
		   
			obj.gridErrorHandler();
	   	}
	}); 	
	}catch(e){
 		//Nothing to do
	}
},
showHidemodifier : function (criteriaId) {
	try{
	$(".modifier-item-position").css("display","none");
	var getId = 'modifier_id_'+criteriaId;
	var e = document.getElementById(getId);
    if(e.style.display == 'block')
       e.style.display = 'none';
    else
       e.style.display = 'block';
	}catch(e){
 		//Nothing to do
	}
},

modifiersClosePopup : function (criteriaId) {
	try{
	if($('#hiddenmandatory-'+criteriaId).length > 0) {
		var hiddenmandatory  = $('#hiddenmandatory-'+criteriaId).html();
		if(hiddenmandatory ==1){
			$('#mandatory-'+criteriaId).attr('checked',true);
			$('#mandatory-'+criteriaId).parent().removeClass('checkbox-unselected');
			$('#mandatory-'+criteriaId).parent().addClass('checkbox-selected');
		}else {
			$('#mandatory-'+criteriaId).removeAttr('checked');
			$('#mandatory-'+criteriaId).parent().removeClass('checkbox-selected');
			$('#mandatory-'+criteriaId).parent().addClass('checkbox-unselected');
		} 
	}
	if ($('#hiddenuserfilter-'+criteriaId).length > 0) {
		var hiddenuserfilter = $('#hiddenuserfilter-'+criteriaId).html();
		if(hiddenuserfilter == 1){
			$('#user-filter-'+criteriaId).attr('checked',true);
			$('#mandatory-'+criteriaId).attr('disabled', true);
			$('#condition-filter-'+criteriaId).attr('disabled',true);
			$('#user-filter-'+criteriaId).parent().addClass('checkbox-selected');
			$('#user-filter-'+criteriaId).parent().removeClass('checkbox-unselected');
		}else {
			$('#user-filter-'+criteriaId).removeAttr('checked');
			$('#mandatory-'+criteriaId).removeAttr('disabled');
			$('#condition-filter-'+criteriaId).removeAttr('disabled');
			$('#user-filter-'+criteriaId).parent().addClass('checkbox-unselected');
			$('#user-filter-'+criteriaId).parent().removeClass('checkbox-selected');
		} 
	 }
  
	if($('#hiddencondtype-'+criteriaId).length > 0) {
		var hiddencondtype   = $('#hiddencondtype-'+criteriaId).html();
		$('#condition-filter-'+criteriaId).val(hiddencondtype);
	}	
	//console.log(hiddencondtype+'-'+hiddenmandatory+'-'+hiddenuserfilter);
	$('span.modifier_style_container').css('display','none');
	}catch(e){
 		//Nothing to do
	}
}, 


callbackDataGrid: function(data){
	try{
	var obj = $('body').data('reportsearch');
	obj.applyThemes();
	obj.destroyLoader('report_grid_section');
	var recs = parseInt($("#report_grid_container").getGridParam("records"),10);
	var html = '';
	var themekey = Drupal.settings.ajaxPageState.theme;
	$('#reports-noresults-launch'). css('display','none');
	if(data.message!=null && data.message != undefined && data.message != ""){
	  var error = new Array();
	  error[0] = data.message;
	  var $renderedMsg = expertus_error_message(error, 'error');
		$('#show_expertus_message').show();
		$('#show_expertus_message').html($renderedMsg);
	}	
    if (recs == 0) {
    	$('#reports-noresults'). css('display','block');
    	if(obj.previewMsg == 1){
    		html = Drupal.t('MSG381')+'.'; // No search results found.
    	} else {
    		html = Drupal.t('MSG509'); // Click Preview data to load results
    	}
        $("#reports-noresults").html(html);            
        $("#report_result_pager").hide();
        $('#report_grid_section .block-footer-left').show(); // show the bottom curve corner
    } else {
    	$('#reports-noresults'). css('display','none');
    	$("#reports-noresults").html("");
    	$("#report_result_pager").show();
    	if(themekey == "expertusoneV2"){
    		$('#report_grid_section .block-footer-left').hide(); // hide the bottom curve corner
    	}
    }
    
    if (recs == 0) {
    	$('#report-exp-pdf-div').css('display','none');
    }else{
    	$('#report-exp-pdf-div').css('display','block');
    }
    var reccount = parseInt($("#report_grid_container").getGridParam("reccount"), 10);
    if (recs > reccount) {
    	$('#report_result_pager').show();
    	$('#report_result_pager #first_report_result_pager').show();
    	$('#report_result_pager #prev_report_result_pager').show();
    	$('#report_result_pager #next_report_result_pager').show();
    	$('#report_result_pager #last_report_result_pager').show();
    	$('#report_result_pager .search-pagination-right').show();
    	if(themekey == "expertusoneV2")
    		$('#report_grid_section .block-footer-left').hide(); // hide the bottom curve corner
    } else {
    	$('#report_result_pager #first_report_result_pager').hide();
    	$('#report_result_pager #prev_report_result_pager').hide();
    	$('#report_result_pager #next_report_result_pager').hide();
    	$('#report_result_pager #last_report_result_pager').hide();
    	$('#report_result_pager .search-pagination-right').hide();
    	if(themekey != "expertusoneV2"){
    		$('#report_grid_section .block-footer-left').show(); // show the bottom curve corner
    	}
    }

	$('#prev_report_result_pager').click( function(e) {
		if(!$('#prev_report_result_pager').hasClass('ui-state-disabled')) {
			$("body").data("reportsearch").createLoader('report_grid_section');
		}
	});
	$('#next_report_result_pager').click( function(e) {
		if(!$('#next_report_result_pager').hasClass('ui-state-disabled')) {
			$("body").data("reportsearch").createLoader('report_grid_section');
		}
	});
	$('#report_result_pager_center .ui-pg-selbox').bind('change',function() {
		$("body").data("reportsearch").createLoader('report_grid_section');
	});			
	$("#report_result_pager_center .ui-pg-input").keyup(function(event){				
		if(event.keyCode == 13){
			$("body").data("reportsearch").createLoader('report_grid_section');
		}
	});
	
    
	if(obj.report_editable){
		if(obj.reportBuilderType == 'design_wizard'){
			$('#gview_report_grid_container .ui-jqgrid-htable tr > th > div.ui-jqgrid-sortable').click(function(event){
				if($(this).text() != ''){
					var text = $(this).text();
					var html = $(this).html();
					var tagHtml = $(this).text('');
					var inputHTML = '<input type="textbox" size="15" class="design_wiz_header" id="design_wiz_header_'+text+'" onkeydown="$(\'body\').data(\'reportsearch\').updateColumnAliasKeyDown(event,'+obj.report_id+', \''+text+'\', this.value);" onBlur="$(\'body\').data(\'reportsearch\').updateColumnAlias('+obj.report_id+', \''+text+'\', this.value)" value="'+text+'">'+$(this).html();
				    $(this).html(inputHTML);
				    $('input[id="design_wiz_header_'+text+'"]').focus();
				}
			});
		}
		
		//var display_timeout = 0;
		$("#report_grid_header_theme_update").click(function () {
			obj.stylingHeaderFooter('grid_header', 'report_grid_header_style_container');
		});
	
		//var display_timeout = 0;
		$("#report_grid_footer_theme_update").click(function () {
			obj.stylingHeaderFooter('grid_row', 'report_grid_footer_style_container');
			//$('#grid_row-font-list').focus();
		});
	}
	var gridWidth = $('#gview_report_grid_container .ui-jqgrid-bdiv').width();
	$('#gview_report_grid_container .ui-jqgrid-hdiv').css('width',parseInt(gridWidth)+'px');
	//change by ayyappans for 39574: Remove the refine section in the launched report if there are no refine filters set
	/*if(obj.reportLaunch != "launch" || obj.reportCriteria != undefined && obj.reportCriteria != '') {
		$('#gbox_report_grid_container').css('width','650px');
	}
	else {
		$('#gbox_report_grid_container').css('width','99%');
	}*/
	//TOOL TIP
	vtip();
	$('td#first_report_result_pager, td#last_report_result_pager').hide();
	}catch(e){
 		//Nothing to do
	}
},

updateColumnAliasKeyDown : function(evt,report_id, text, value){
	try{
	evt = evt || window.event;
    var charCode = evt.keyCode || evt.which;
    if (charCode == 13) {           	    	
    	$('body').data('reportsearch').updateColumnAlias(report_id, text, value);            	    
        evt.preventDefault();
        evt.stopPropagation();
        return false;           	    	
    }
	}catch(e){
 		//Nothing to do
	}
},

updateColumnAlias: function(reportId, oldAlias, newAlias){
	try{
	if(newAlias == '' || newAlias == null || newAlias == undefined){
		$('#design_wiz_header_'+oldAlias).parent().text(oldAlias);
	} else {
		var obj = this;
		var param = '';
		obj.createLoader('report_grid_section');
		url = obj.constructUrl("administration/report-search/update-column-alias/"+obj.report_id+"/"+encodeURIComponent(oldAlias)+"/"+encodeURIComponent(newAlias));
		$.ajax({
			type: "POST",
			dataType: 'json',
			async : false,
			url: url,
			data:  param,
			success: function(data){
				obj.destroyLoader('report_grid_section');
				if(data.result == 'updated'){
					$('input[id="design_wiz_header_'+oldAlias+'"]').attr('id', 'design_wiz_header_'+newAlias);
					$('input[id="design_wiz_header_'+newAlias+'"]').parent().text(newAlias);
					obj.column_alias_sql = data.column_alias_sql; 
				} else {
					$('input[id="design_wiz_header_'+oldAlias+'"]').parent().text(oldAlias);
				}
			}
		}); 			
	}
	}catch(e){
 		//Nothing to do
	}
},

loadNarrowFilter: function(){
	try{
	var obj = this;
	var param = '';
	obj.createLoader('report_criteria_form');
	url = obj.constructUrl("administration/report-search/load-available-criteria/"+obj.report_id);
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(data){
			if(data.result == 'updated'){
				var criteria_details = data.criteria_details;
				var criteria_sql = criteria_details.column_criteria_sql;
				var criteria_alias_sql = criteria_details.column_criteria_alias_sql;
				var criteriaSQLArray = criteria_sql.split(',');  
				var criteriaAliasSQLArray = criteria_alias_sql.split(',');
				var criteriaSQLLength = criteriaSQLArray.length;
				
				var html = '';
				var columnName = '';
				html+='<div class="qtip-tip-point-left"></div>';
				html+='<table cellspacing="0" cellpadding="0" id="bubble-face-table" class="bubble-table-container">';
				html+='<tbody>';
				html+='<tr>';
				html+='<td class="bubble-tl"></td>';
				html+='<td class="bubble-t"></td>';
				html+='<td class="bubble-tr"></td>';
				html+='</tr>';
				html+='<tr>';
				html+='<td class="bubble-cl"></td>';
				html+='<td valign="top" class="bubble-c">';
				html += '<div class="tip_close admin-bubble-close">&nbsp;</div><div class="report-scroll-pane">';
				html += '<div class="header">'+Drupal.t('LBL790')+'</div>';
				var criteriaAvailable = 0;
				for(c = 0; c < criteriaSQLLength; c++){
					columnName = criteriaSQLArray[c];
					columnName = columnName.replace('.','_');
					columnName = columnName.replace(/\+/g,' ');
				if($('input[data-custom ='+$.trim(columnName)+']').length == 0 ){ 
						criteriaAvailable = 1;
						html += '<div class="items">';
						html += '<span class="column_name" data="'+criteriaSQLArray[c]+'$$'+criteriaAliasSQLArray[c]+'">'+criteriaAliasSQLArray[c]+'</span>';
						html += '<span class="add_button"></span>';
						html += '</div>';
					}
				}
				if(criteriaAvailable == 0){
					html += '<div class="items">'+Drupal.t('MSG514')+'</div>';
				}
				if(Drupal.settings.ajaxPageState.theme== "expertusoneV2"){
				html+='</div><div class="add-filter-close-align"><div class="white-btn-bg-left"></div><span class="tip_close white-btn-bg-middle">'+Drupal.t('LBL123')+'</span><div class="white-btn-bg-right"></div></div></td>';
				}else
				{
					html+='</div><span class="tip_close white-btn-bg-middle">'+Drupal.t('LBL123')+'</span></td>';
				}
				html+='<td class="bubble-cr"></td>';
				html+='</tr>';
				html+='<tr>';
				html+='<td class="bubble-bl"></td>';
				html+='<td class="bubble-b">';
				html+='<table width="100%" cellspacing="0" cellpadding="0">';
				html+='<tbody>';
				html+='<tr>';
				html+='<td class="bubble-blt"></td>';
				html+='<td class="bubble-blr"></td>';
				html+='</tr>';
				html+='</tbody>';
				html+='</table>';
				html+='</td>';
				html+='<td class="bubble-br"></td>';
				html+='</tr>';
				html+='</tbody>';
				html+='</table>';
				$('#report_add_narrow_filter_popup').html(html);
				$('#report_add_narrow_filter_popup').show();
				
				$('#report_add_narrow_filter_popup .add_button').click(function(){
					var criteriaData = $(this).prev().attr('data');
					var criteriaDataArray = criteriaData.split('$$');
					obj.addNarrowFilter(escape(criteriaDataArray[0]), escape(criteriaDataArray[1]));
				});
				
			}
			obj.destroyLoader('report_criteria_form');
			$('.report-scroll-pane').jScrollPane();
		}
	}); 	
	}catch(e){
 		//Nothing to do
	}
},

addNarrowFilter: function(criteriaSQL, criteriaAliasSQL){
	try{
	var obj = this;
	var param = '';
	obj.createLoader('report_criteria_form');
	url = obj.constructUrl("administration/report-search/add-criteria/"+obj.report_id+"/"+criteriaSQL+"/"+criteriaAliasSQL);
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(data){
			if(data.result == 'updated'){
				obj.reportCriteria = data.criteria_details;
				obj.loadCriteria();
				$('#report_add_narrow_filter_popup').hide();
			}
			obj.destroyLoader('report_criteria_form');
		}
	});
	}catch(e){
 		//Nothing to do
	}
},

deleteNarrowFilter: function(criteriaData){
	try{
	var obj = this;
	var param = '';
	
    var criteriaDataArray = criteriaData.split('$$');
	var criteriaId = criteriaDataArray[0];
    var columnType = criteriaDataArray[1];
    var columnAliasName = criteriaDataArray[2];
	
	obj.createLoader('report_criteria_form');
	url = obj.constructUrl("administration/report-search/delete-criteria/"+obj.report_id+"/"+criteriaId+"/"+columnType+"/"+columnAliasName);
	var msg = new Array();
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(data){
			//Added for #0070534
			if(data.result == 'NON_SCALABLE_QUERY'){
				msg[0] = data.status == 'cre_rpt_rps_dft' ? Drupal.t('ERR255') : Drupal.t('ERR256');
				var message_call = expertus_error_message(msg,'error');
				$('#show_expertus_message').show();
				$('#show_expertus_message').html(message_call);
				obj.reportCriteria = data.criteria_details;
				obj.loadCriteria();
			}else if(data.result == 'updated'){
				obj.reportCriteria = data.criteria_details;
				obj.loadCriteria();
			}
			obj.destroyLoader('report_criteria_form');
		},
		failure: function(data){
			var msg = new Array();
			obj.destroyLoader('report_criteria_form');
			obj.loadReportLayout(obj.report_id,'report-data-source',0);
			msg[0] = Drupal.t('ERR256');
			var message_call = expertus_error_message(msg,'error');
			$('#show_expertus_message').show();
			$('#show_expertus_message').html(message_call);
			obj.gridErrorHandler();
		},
  		error:function (XMLHttpRequest, textStatus, errorThrown) {
			var msg = new Array();
			obj.destroyLoader('report_criteria_form');
			obj.loadReportLayout(obj.report_id,'report-data-source',0);
			msg[0] = Drupal.t('ERR256');
			var message_call = expertus_error_message(msg,'error');
			$('#show_expertus_message').show();
			$('#show_expertus_message').html(message_call);
			obj.gridErrorHandler();
	   	}
	});
	}catch(e){
 		//Nothing to do
	}
},

clearText: function(element_id){
	try{
		element_id = unescape(element_id);
		clrLabelId = 'clr-text-'+element_id;
		$('.clr-text').each(function(){
//			alert($(this).attr('id'));
			if($(this).attr('id') == clrLabelId){
				$(this).css('display','none');
			}
		});
		$('.criteria-textBox').each(function(){
//			alert($(this).attr('id'));
			if($(this).attr('id') == element_id){
				$(this).val('');
//				$('#'+element_id).val('');
			}
		});
		$('.criteria-date-textBox').each(function(){
//			alert($(this).attr('id'));
			if($(this).attr('id').slice(0,-1) == element_id.slice(0,-1)){
				$(this).val('');
//				$('#'+element_id).val('');
			}
		});
		
		var clrdate=$('#'+element_id).closest(".datetime").next().attr("class");
		if(clrdate=="items individual-item datetime"){
			var spitDate=clrdate.split(" ");
			$('#'+element_id).find('.'+spitDate[2]).find(".criteria-date-textBox").val("");
			var criteriaClass = (Drupal.settings.ajaxPageState.theme == 'expertusoneV2') ? 'criteria-date-textBox' : 'criteria-textBox';
			$('#'+element_id).closest('.'+spitDate[2]).next().find("."+criteriaClass).val("");
		}

	
	$('#report_criteria_apply').click();
	$('#show_expertus_message').html('');
	$('#report-exp-pdf-div'). css('display','none');
	$('#report_result_pager'). css('display','none');
	$('#reports-noresults').html('');
    $('#report_grid_section .block-footer-left').show();
	}catch(e){
 		//Nothing to do
		console.log(e);
	}
},

updateCriteriaCondition: function(criteriaId, typeValue, criteriaCondition){
	try{
	var obj = this;
	var param = '';
	obj.createLoader('report_criteria_form');
	url = obj.constructUrl("administration/report-search/update-criteria-condition/"+obj.report_id+"/"+criteriaId+"/"+criteriaCondition);
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(data){
			if(data.result == 'updated'){
				$('#list-box-filter-input-toptxt').html(typeValue);
				obj.reportCriteria = data.criteria_details;
				obj.loadCriteria();
			}
			obj.destroyLoader('report_criteria_form');
		}
	});
	}catch(e){
 		//Nothing to do
	}
},

updateMandatoryUserfilter: function(criteriaId, type, typeAction,fltUpdate){
	try{
	var obj = this;
	var param = '';
	obj.createLoader('report_criteria_form');
	url = obj.constructUrl("administration/report-search/update-mandatory-userfilter/"+obj.report_id+"/"+criteriaId+"/"+type+"/"+typeAction+"/"+fltUpdate);
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(data){
			if(data.result == 'updated'){
				//$('#list-box-filter-input-toptxt').html(typeValue);
				obj.reportCriteria = data.criteria_details;
				obj.loadCriteria();
			}
			obj.destroyLoader('report_criteria_form');
		}
	});
	}catch(e){
 		//Nothing to do
	}
},

loadCriteria: function(){
	try{
	var obj = this;
	var criteria = this.reportCriteria;
	if(criteria !='' && criteria != undefined){
		var id = ''; var name = ''; var column_name = ''; var column_type = '';
		var mandatory = ''; 
		var use_user_details ='';
		var mandatoryLabel = (Drupal.t('Mandatory')).toLowerCase();
		mandatoryLabel = mandatoryLabel.charAt(0).toUpperCase() + mandatoryLabel.slice(1);
		var condition_type = '';
		var html = '';
		var otherhtml = '';
		var checkedStatus = '';
		var criteriaColumnName = '';
		var aliasTxt ='';

		var criteriaJoinIds = '';
		var tmpColumnName = '';
		var firstDateCriteriaId = 0;
		for(var i=0; i<criteria.length;i++){
			criteriaColumnName = criteria[i].column_name;
			criteriaColumnName = criteriaColumnName.replace('.','_');
			criteria[i].name=htmlEntities(criteria[i].name);  /* Added htmlEntities function to avoid compiling javascript for security issue - By Ganeshbabuv, June 26th 2015 8:15 PM */
			checkedStatus = criteria[i].mandatory == 1 ? 'checked' : '';
			
			html+='<div class="items individual-item '+criteria[i].column_type+'" id="criteria_'+criteria[i].id+'">';
			html+='<span class="style_container individual-item-position" id="criteria_style_'+criteria[i].id+'"></span>';

			if(criteria[i].condition_type == 'cre_rpt_cnd_grt'){
				criteriaJoinIds = criteria[i].id;
			}
				
			if(criteria[i].condition_type != 'cre_rpt_cnd_les'){
				
				html+='<div class="label-header"><a class="filters-link-icon-opened " href="javascript:void(0)" id="narrow-filter-show-hide" data="'+criteria[i].id+'"></a>';
				var charNum = 13;	
				if(this.currTheme != 'expertusoneV2'){
					 charNum = 10;
				}
				html+='<div id="hide-show-header-txt" class="label' + (obj.report_editable? ' editable' : '') + '" data="'+criteria[i].id+'" style="'+this.criteria_column_style+'">';
				if(obj.report_editable == ' editable') {
					html+= '<span class="report-fillters-list-'+criteria[i].id+'" title="'+htmlEntities(criteria[i].name)+'">'+titleRestrictionFadeoutImage(criteria[i].name, 'lnrreports-search-header-filters-edit-list-name',charNum);
				} else {
					html+= '<span class="report-fillters-list-'+criteria[i].id+'" title="'+htmlEntities(criteria[i].name)+'">'+titleRestrictionFadeoutImage(criteria[i].name, 'lnrreports-search-header-filters-list-name',charNum);
				}
				//html+= '<span class="report-fillters-list-'+criteria[i].id+'" title="'+htmlEntities(criteria[i].name)+'">'+titleRestrictionFadeoutImage(criteria[i].name, 'lnrreports-search-header-filters-list-name',charNum);	
				html+='</span>';
				html+='<span class="mandatory_symbol" id="mandatory-symbol-'+criteria[i].id+'"';
				if(criteria[i].mandatory == 1 || criteria[i].use_user_details == 1){
					html += 'style="display:inline-block;"';
					obj.hasMandatoryFields = "mandatory";
				} else {
					html += 'style="display:none;"';
				}
				html += '>&nbsp;*</span>';
				html+='</div>';
				
				
					if(obj.report_editable){
						html+='<div class="hidden_text"><input maxlength="25" class="criteria_hidden_text" size="6" type="text" onblur="$(\'body\').data(\'reportsearch\').updateCriteriaName('+criteria[i].id+', this.value);"  value="'+criteria[i].name+'" /></div>';
  						html+='<div class="delete " data="'+criteria[i].id+'$$'+criteria[i].column_type+'$$'+criteria[i].column_alias_name+'"></div>';
  						/*html+='<div style="clear:both;"></div>';*/
					}
					if(obj.report_editable || criteria[i].use_user_details != 1) {
					  html+='<div class="clr-text" style="display:none;" id="clr-text-'+criteriaColumnName+'_'+i+'" data="'+criteriaColumnName+'_'+i+'" onClick="$(\'body\').data(\'reportsearch\').clearText(\''+escape(criteriaColumnName)+'_'+i+'\')">'+Drupal.t("LBL307")+'</div>';
					}  
					html+='</div>';
			}	
			
			
			html+='<div class="narrow-filter-fields-conatiner">';
			
			if(criteria[i].column_type == 'datetime' || criteria[i].column_type == 'date'){
				if(this.currTheme == 'expertusoneV2'){
					html+='<div><input type="text" id="'+criteriaColumnName+'_'+i+'" name="'+criteria[i].id+'" size="10" class="criteria-date-textBox addedit-edit-datefield" data-custom="'+criteriaColumnName+'" /></div>';
				}
				else{
					html+='<div class="input"><input type="text" id="'+criteriaColumnName+'_'+i+'" name="'+criteria[i].id+'" size="10" class="criteria-textBox" data-custom="'+criteriaColumnName+'" /></div>';				
				}
				
				/*if(obj.report_editable){
					html+='<div class="condition">&nbsp;<select name="" onChange="$(\'body\').data(\'reportsearch\').updateCriteriaCondition('+criteria[i].id+',this.value)">';
				    for(var c=0;c<obj.reportCriteriaCondition.length;c++){
						html+='<option value="'+obj.reportCriteriaCondition[c].code+'"';
						if(obj.reportCriteriaCondition[c].code == criteria[i].condition_type) { 
							html+=' SELECTED ';
						}
						html+='>'+obj.reportCriteriaCondition[c].name+'</option>';
				    }
					html+='</select></div>';
				}*/
			} else {
				var criteriaTextFieldState ='';
				var criteriaTextFieldValue ='';
				var criteriaTextFieldClass ='';
				if(!(obj.report_editable) && criteria[i].use_user_details == 1){
					  criteriaTextFieldState = 'disabled';
					  criteriaTextFieldClass = 'grey-out';
				    criteriaTextFieldValue = criteria[i].user_details;
				}
				if(this.currTheme == 'expertusoneV2'){
					
					html += '<div>';
					html +=   '<div class="filter-search-start-date-left-bg ' + criteriaTextFieldClass + '"></div>';
					html +=   '<input type="text" '+criteriaTextFieldState +
					             ' value = "'+criteriaTextFieldValue+'" id="'+criteriaColumnName+'_'+i+'"' +
					               ' name="'+criteria[i].id+'" size="10"' +
					                 ' class="criteria-textBox filter-search-start-date-middle-bg '+ criteriaTextFieldClass + ' " data-custom="'+criteriaColumnName+'"/>';
					html +=   '<a title="'+Drupal.t("LBL304")+'"   data="'+criteriaColumnName+'_'+i+'" class="narrow-text-search vtip ' + criteriaTextFieldClass + '" id="report_criteria_apply">&nbsp;</a>';					
					html +=   '<div class="filter-search-start-date-right-bg '+criteriaTextFieldClass+'"></div>';
					html +='</div>';
				}
				else{
					html+= '<div class="input">';
					html+=   '<input type="text" '+criteriaTextFieldState+' value = "'+criteriaTextFieldValue+'" id="'+criteriaColumnName+'_'+i+'" name="'+criteria[i].id+'" size="10" class="criteria-textBox '+criteriaTextFieldClass+' " data-custom="'+criteriaColumnName+'" />';
					html+=   '<a title="'+Drupal.t("LBL304")+'" data="'+criteriaColumnName+'_'+i+'" class="narrow-text-search vtip" id="report_criteria_apply">&nbsp;</a>';
					html+= '</div>';
				}
				
				/*if(obj.report_editable){
					html+='<div class="condition">&nbsp;<select DISABLED name="" onChange="$(\'body\').data(\'reportsearch\').updateCriteriaCondition('+criteria[i].id+',this.value)">';
				    for(var c=0;c<obj.reportCriteriaCondition.length;c++){
				    	if(obj.reportCriteriaCondition[c]['code'] == 'cre_rpt_cnd_con'){
							html+='<option value="'+obj.reportCriteriaCondition[c]['code']+'"';
							if(obj.reportCriteriaCondition[c]['code'] == criteria[i].condition_type) { 
								html+=' SELECTED';
							}
							html+='>'+obj.reportCriteriaCondition[c]['name']+'</option>';
				    	}
				    }
					html+='</select></div>';
				}*/
			}
			if(obj.report_editable && ((criteria[i].column_type != 'datetime' && criteria[i].column_type != 'date') || firstDateCriteriaId != 0)){
				html +='<span onclick="$(\'body\').data(\'reportsearch\').showHidemodifier(\''+criteria[i].id+'\');" class="settingicon">&nbsp;</span>';
				html += '<span id="modifier_id_'+criteria[i].id+'" class="modifier_style_container modifier-item-position" style="display:none;">';
				html += obj.modifierFormHtml(criteria[i],firstDateCriteriaId);
				html += '</span>';
				}
				if(criteria[i].column_type == 'datetime' || criteria[i].column_type == 'date'){
					if(firstDateCriteriaId == 0){
						firstDateCriteriaId = criteria[i].id;
					}else {
					  firstDateCriteriaId = 0;
					}
				}
			
			if((criteria[i].column_type == 'datetime' || criteria[i].column_type == 'date') &&  criteria[i].condition_type == 'cre_rpt_cnd_les'){			
				html+='<div class="criteria-button-container"><div class="curved-blue-button-left"></div><input value="' + Drupal.t("LBL669") + '" data="'+criteriaColumnName+'_'+i+'" class="reports-apply-btn" type="button" id="report_criteria_apply"><div class="curved-blue-button-right"></div></div><div class="clearBoth"></div>';
			}
			
			if(obj.report_editable){
				if(criteria[i].condition_type == 'cre_rpt_cnd_con' || criteria[i].condition_type == 'cre_rpt_cnd_equ' || criteria[i].condition_type == 'cre_rpt_cnd_sta'|| criteria[i].condition_type == 'cre_rpt_cnd_les'){
					criteriaJoinIds = criteriaJoinIds == '' ? criteria[i].id : criteriaJoinIds + ',' + criteria[i].id;
					var manText = '';
					var mantype = '';
					if(criteria[i].mandatory == 1 ){
						manText += Drupal.t("LBL1148");
						mantype = 'delete';
					} else {
						manText += Drupal.t("LBL1147");
						mantype = 'add';
					}

					if(criteria[i].condition_type == 'cre_rpt_cnd_con'){
						var typeValue = Drupal.t("LBL1141");
					}else if(criteria[i].condition_type == 'cre_rpt_cnd_equ'){
						var typeValue = Drupal.t("LBL1142");
					}else if(criteria[i].condition_type == 'cre_rpt_cnd_sta'){
						var typeValue = Drupal.t("LBL1143");
					}
					criteriaJoinIds = '';
				}
			}
			
			if(obj.report_editable == 0) {
				if(criteria[i].condition_type == 'cre_rpt_cnd_con' || criteria[i].condition_type == 'cre_rpt_cnd_equ' || criteria[i].condition_type == 'cre_rpt_cnd_sta' || criteria[i].condition_type == 'cre_rpt_cnd_les'){
					html+='<div class="mandatory-option-container"></div>';
				}
			}
			
			html+='<input type="hidden" id="mandatory_'+criteria[i].id+'" value="'+criteria[i].mandatory+'" />';
			html+='<input type="hidden" id="username_filter_'+criteria[i].id+'" value="'+criteria[i].use_user_details+'" />';
			html+='<input type="hidden" id="criterianame_'+criteria[i].id+'" value="'+criteria[i].name+'" />';
			html+='</div>';
			html+='<div style="clear:both"></div>';
			html+='</div>';
		}
		
		if(html != ''){
			if(obj.reportLaunch == 'launch') {
				/*if(obj.hasMandatoryFields == "mandatory") {
					$('#report_criteria_form').show();
					$('#narrow-filter-show-hide').removeClass().addClass('filters-link-icon-opened');
					obj.hasMandatoryFields = '';
				} else {
					$('#report_criteria_form').hide();
					$('#narrow-filter-show-hide').removeClass().addClass('filters-link-icon-closed');
					obj.hasMandatoryFields = '';
				}*/
				obj.reportLaunch = '';
			} else {
				$('#report_criteria_form').show();
				obj.reportLaunch = '';
			}
			$('#narrow-filter-show-hide').show();
			
			otherhtml+= '';
			
			$('#report_criteria_form').html(otherhtml+html);

			$('#report_criteria_apply').die('click');
		$('#report_criteria_apply').live('click', function(){
                $('#reports-export-xcel-container').show(); //Changes made For the Ticket Of 0029874
                $('#reports-pdf-container').show();
                $('#gview_report_grid_container').show();
                $('#reports-noresults').css('display','none');
                $('#reports-noresults-launch').css('display','none');
                var validateStatus = obj.criteriaValidate(1);
                if(validateStatus == 1){
                    $("body").data("reportsearch").createLoader('report_grid_section');
                    if($('#report_grid_container').html() == '' || $('#report_grid_container').html() == null || $('#report_grid_container').html().toUpperCase()== '<TBODY></TBODY>'){
                        obj.loadResultGrid(1);
                    } else {
                        obj.previewMsg=1;
                        obj.reloadReportResult();
                    }
                }else{
                    $('#reports-export-xcel-container').hide();//Changes made For the Ticket Of 0029874
                    $('#reports-pdf-container').hide();
                    $('#gview_report_grid_container').hide();
                    $('#reports-noresults').css('display','block');
                    $('#reports-noresults-launch').css('display','block');
                }
                //Enable Clear text for date time
                var dateId = $(this).closest(".datetime").prev(".datetime").find(".label-header .clr-text").attr('data');
                var id = $(this).attr('data');
                var dateTimeName=$(this).closest(".datetime").prev(".datetime").attr("class");
                //hide clear link if input type is empty
                var allIntfld=$("#"+id).val();
                var dateIntfld1=$("#"+dateId).val();
                var dateIntfld=$("#"+id).val();
               
                if(dateTimeName == 'items individual-item datetime'){
                    clrLabelId = 'clr-text-'+dateId;
                    $('.clr-text').each(function(){
                        if($(this).attr('id') == clrLabelId){
                            $(this).css('display','block');
                        }
                    });
//                $('#clr-text-'+dateId).css('display','block');
                    if(dateIntfld=="" && dateIntfld1=="") {
                    $("div.clr-text").not('#clr-text-'+dateId).css('display','none');
                    $('.clr-text').each(function(){
                        if($(this).attr('id') == clrLabelId){
                            $(this).css('display','none');
                        }
                    });
                    //$('#clr-text-'+dateId).css('display','none');
                    }
                }else{
                    clrLabelId = 'clr-text-'+id;
                    $('.clr-text').each(function(){
                        if($(this).attr('id') == clrLabelId){
                            $(this).css('display','block');
                        }
                    });
                //$('#clr-text-'+id).css('display','block');
                  if(allIntfld=="") {
                    $("div.clr-text").not('#clr-text-'+id).css('display','none');
                    $('.clr-text').each(function(){
                        if($(this).attr('id') == clrLabelId){
                            $(this).css('display','none');
                        }
                    });
                    //$('#clr-text-'+id).css('display','none');
                  }
                }
               
                //check If each input value is empty
                $(".criteria-textBox, .criteria-date-textBox").each(function(index, value) {
                    var chkAllId=$(this).val();
                    if(chkAllId!=""){
                    $(this).parents("div").prevAll("div.label-header").find("div.clr-text").not('#clr-text-'+id).css('display','block');     
                    }
                });
               
                if(obj.report_editable){
                    $('.clr-text').css('border-right','1px solid #CCCCCC');
                }
               
            });
			//show and hide report narrow filter left panel arrow
			$('#report_criteria_form #narrow-filter-show-hide').click(function() {
				var criteriaId = $(this).attr('data');
				var getClassName = $("#report_criteria_form #criteria_"+criteriaId+" #narrow-filter-show-hide").attr('class');
				if(getClassName == 'filters-link-icon-closed') {
					$("#report_criteria_form #criteria_"+criteriaId+" #narrow-filter-show-hide").removeClass().addClass('filters-link-icon-opened');
					$("#report_criteria_form #criteria_"+criteriaId+" .narrow-filter-fields-conatiner").show();
					if($("#report_criteria_form #criteria_"+criteriaId).hasClass('datetime') == true && $("#report_criteria_form #criteria_"+criteriaId).next().hasClass('datetime') == true){
						$("#report_criteria_form #criteria_"+criteriaId).next().show();
						$("#report_criteria_form #criteria_"+criteriaId).css('border-bottom','0px dotted #cccccc');
					}
				} else {
					$("#report_criteria_form #criteria_"+criteriaId+" #narrow-filter-show-hide").removeClass().addClass('filters-link-icon-closed');
					$("#report_criteria_form #criteria_"+criteriaId+" .narrow-filter-fields-conatiner").hide();
					if($("#report_criteria_form #criteria_"+criteriaId).hasClass('datetime') == true && $("#report_criteria_form #criteria_"+criteriaId).next().hasClass('datetime') == true){
						$("#report_criteria_form #criteria_"+criteriaId).css('border-bottom','1px dotted #cccccc');
						$("#report_criteria_form #criteria_"+criteriaId).next().hide();
					}
				}
			});
			if(obj.report_editable){
				
				$('#report_criteria_form .label').click(function(){
					$(this).hide();
					$(this).next().show();
					$(this).next().children().focus();
				});
				
				$('#report_criteria_form .criteria_hidden_text').keydown(function(event){
					if (event.keyCode == 13) {
						event.preventDefault();
						$(this).blur();
					}
				});		
				
				$('#report_criteria_form .delete').click(function(){
					var criteriaId = $(this).attr('data');
					obj.deleteNarrowFilter(criteriaId);
				});
				
				$('#report_criteria_form .drp-down-arrow').click(function(){
					var criteriaId = $(this).attr('data');
					$('#criteria_'+criteriaId+' .date-drp-down-options').toggle();
				});
				$('#report_criteria_form .user-filter-drop-down-arrow').click(function(){
					var criteriaId = $(this).attr('data');
					$('#criteria_'+criteriaId+' .use-userfilter-drp-down-options').toggle();
				});
				//
				$('#report_criteria_form .set-remove-username-filter').click(function(){
					var criteriaIdAll = $(this).attr('data');
					var criteriaIdArray = criteriaIdAll.split(',');
					var criteriaIdLen = criteriaIdArray.length;
					var criteriaIdFirst = criteriaIdArray[0];
					var criteriaId = criteriaIdArray[criteriaIdLen-1];

					var type;
					var assignText = $('#criteria_'+criteriaId+' #setAndRemoveUserNameFilter').text();
					if(assignText == Drupal.t("User")) {
						$('#criteria_'+criteriaId+' #setAndRemoveUserNameFilter').text(Drupal.t("LBL1148"));
						$('#criteria_'+criteriaIdFirst+' .mandatory_symbol').show();
						type = 'add';
					} else {
						$('#criteria_'+criteriaId+' #setAndRemoveUserNameFilter').text(Drupal.t("LBL1147"));
						$('#criteria_'+criteriaIdFirst+' .mandatory_symbol').hide();
						type = 'delete';
					}
					obj.addDeleteUsernameFilter(criteriaIdAll, type);
				});
				$('#report_criteria_form .set-remove-mandatory').click(function(){
					var criteriaIdAll = $(this).attr('data');
					var criteriaIdArray = criteriaIdAll.split(',');
					var criteriaIdLen = criteriaIdArray.length;
					var criteriaIdFirst = criteriaIdArray[0];
					var criteriaId = criteriaIdArray[criteriaIdLen-1];

					var type;
					var assignText = $('#criteria_'+criteriaId+' #setAndRemoveMandatory').text();
					if(assignText == Drupal.t("LBL1147")) {
						$('#criteria_'+criteriaId+' #setAndRemoveMandatory').text(Drupal.t("LBL1148"));
						$('#criteria_'+criteriaIdFirst+' .mandatory_symbol').show();
						type = 'add';
					} else {
						$('#criteria_'+criteriaId+' #setAndRemoveMandatory').text(Drupal.t("LBL1147"));
						$('#criteria_'+criteriaIdFirst+' .mandatory_symbol').hide();
						type = 'delete';
					}
					obj.addDeleteMandatory(criteriaIdAll, type);
				});
				
				/*$('#report_criteria_form .criteria_mandatory').click(function(){
					var criteriaId = $(this).attr('data');
					var type = $(this).is(':checked') ? 'add' : 'delete';
					obj.addDeleteMandatory(criteriaId, type);
				});*/

			} else {
				$('#report_criteria_form .label').hover(function(){
					//$(this).css('border','none');
				});
			}

			// Auto-complete fields
			for(var i=0; i<criteria.length;i++){
				criteriaColumnName = criteria[i].column_name;
				criteriaColumnName = criteriaColumnName.replace('.','_');
				if(criteria[i].column_type == 'datetime' || criteria[i].column_type == 'date'){
					$('input[id^='+criteriaColumnName+']').datepicker({
						duration: '',
						showTime: false,
						constrainInput: false,
						time24h: true,
						dateFormat: "yy-mm-dd",
						buttonImage: themepath+'/images/calendar_icon.JPG',
						buttonImageOnly: true,
						firstDay: 1,
						showOn: 'both',
						showButtonPanel: true,
						changeMonth: true,
						changeYear: true
					});
					
				} else {
						if(obj.report_editable || criteria[i].use_user_details != 1){
						$('input[id^='+criteriaColumnName+']').autocomplete(
							"/?q=learning/report-search/criteria-autocomplete/"+criteria[i].id,{
								minChars :3,
								max :50,
								autoFill :true,
								mustMatch :false,
								matchContains :false,
								inputClass :"ac_input",
								loadingClass :"ac_loading"
							}	    			
						);
					}
				}		
				
			}

			
		} else {
			$('#report_criteria_form').html('<span class="no-filters">'+Drupal.t('MSG512')+'</span>');
			$('#report_criteria_form').show();
			$('#narrow-filter-show-hide').hide();
		
			if(obj.report_editable == 0){ $('#report_criteria_form').hide(); $('#report_grid_section').css('border','none'); }
		}
	} else {
		$('#report_criteria_form').html('<span class="no-filters">'+Drupal.t('MSG512')+'</span>');
		$('#report_criteria_form').show();
		$('#narrow-filter-show-hide').hide();
	
		if(obj.report_editable == 0){ 
			//$('#report_criteria_form').hide(); -- Commented for #0020658
			$('#report_grid_section').css('border','none');
		}
	}
	$('.ui-datepicker-trigger').removeAttr('alt');
	$('.ui-datepicker-trigger').removeAttr('title');
	$("#report_criteria_form > div:last-child > div:last-child > div.mandatory-option-container").addClass("remove-border-from-last-filter");
	$('.ui-datepicker-trigger:even').parent().parent().parent().css('border','0px none');
	$('#report_criteria_form > .items').last().css({'border':'none','padding-bottom':'0px','margin-bottom':'0px','margin-top':'0px'});
	vtip();
	}catch(e){
 		//Nothing to do
	}
},

filterAndShowResults: function() {
  // Code copied from live click event handler for #report_criteria_apply above
 try{	
  var obj = this;
  $('#reports-export-xcel-container').show(); //Changes made For the Ticket Of 0029874
  $('#reports-pdf-container').show();
  $('#gview_report_grid_container').show();
  $('#reports-noresults').css('display','none');
  $('#reports-noresults-launch').css('display','none');
  var validateStatus = obj.criteriaValidate(1);
  if(validateStatus == 1){
    $("body").data("reportsearch").createLoader('report_grid_section');
    if($('#report_grid_container').html() == '' || $('#report_grid_container').html() == null ||
                                          $('#report_grid_container').html().toUpperCase()== '<TBODY></TBODY>'){
      obj.loadResultGrid(1);
    } else {
      obj.previewMsg=1;
      obj.reloadReportResult();
    }
  }else{
    $('#reports-export-xcel-container').hide();//Changes made For the Ticket Of 0029874
    $('#reports-pdf-container').hide();
    $('#gview_report_grid_container').hide();
    $('#reports-noresults').css('display','block');
    $('#reports-noresults-launch').css('display','block');
  }
  
  // Add the clear link for non-empty non-date criteria text fields
  var els = document.getElementById('report_criteria_form').elements;
  var inc = 0;
  var val = ''; var element_id = ''; var criteriaId = '';
  for (var i = 0, len = els.length; i < len; i++) {
    var elementId = els[i].id;
    if (els[i].tagName == "INPUT" && els[i].type == "text" && elementId != '' && !$('#' + elementId).hasClass('hasDatepicker') && els[i].value) {
      $('#clr-text-' + elementId).css('display','block');
      $('.clr-text').css('border-right','1px solid #CCCCCC');
    }
  } 
 }catch(e){
		//Nothing to do
	}
},

// param -> 0 - error message required / 1 - error message not required
criteriaValidate: function(errorMessageRequired){
	try{
	var els = document.getElementById('report_criteria_form').elements;
	var elementNameAndValue = '';
	var commonErrorMessage = Drupal.t('ERR101');
	var errorMessages = new Array();
	var errorMessagesArray = new Array();
	var inc = 0; 
	var val = ''; var element_id = ''; var criteriaId = '';
	for (var i = 0, len = els.length; i < len; ++i) {
		element_id = els[i].id;
		criteriaId = els[i].name;
		if (els[i].tagName == "INPUT" && els[i].type == "text" && element_id != '') {
			mandatoryCheck = ($('#mandatory_'+criteriaId).val() == 1 || $('#username_filter_' + criteriaId).val() == 1)? 1 : 0;
			if(mandatoryCheck == 1){
				val = els[i].value;
				if(val.replace(/\s/g,"") == '' && $.inArray($('#criterianame_'+criteriaId).val(), errorMessagesArray) == -1 ){
					errorMessages[inc] = ($('#criterianame_'+criteriaId).val())+' '+commonErrorMessage;					
					errorMessagesArray[inc] = $('#criterianame_'+criteriaId).val();
					inc++;
				}
			}
		}
	}
	
	if(errorMessages != ''){
		if(errorMessageRequired){
			var message_call = expertus_error_message(errorMessages,'error');
			$('#show_expertus_message').html(message_call);
			$('#show_expertus_message').show();
			
		}
		return 0;
	} else { 
		$('#show_expertus_message').html('');
		$('#show_expertus_message').hide();
		return 1;
	}
	}catch(e){
 		//Nothing to do
	}
},

addDeleteMandatory: function(criteriaId, type){
	try{
	var obj = this;
	var param = '';
	obj.createLoader('report_criteria_form');
	url = obj.constructUrl("administration/report-search/add-delete-mandatory/"+obj.report_id+"/"+criteriaId+"/"+type);
	//url = obj.constructUrl("administration/report-search/add-delete-mandatory/"+criteriaId+"/"+type);
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(data){
			//Added for #0070534
			var msg = new Array();
			if(data.result == 'NON_SCALABLE_QUERY'){
				msg[0] = data.status == 'cre_rpt_rps_dft' ? Drupal.t('ERR255') : Drupal.t('ERR256');
				var message_call = expertus_error_message(msg,'error');
				$('#show_expertus_message').show();
				$('#show_expertus_message').html(message_call);
				obj.reportCriteria = data.criteria_details;
				obj.loadCriteria();
			}else{
				obj.destroyLoader('report_criteria_form');
				obj.reportCriteria = data.criteria_details;
				obj.loadCriteria();
			}
		},
		failure: function(data){
			var msg = new Array();
			obj.destroyLoader('report_criteria_form');
			msg[0] = Drupal.t('ERR256');
			var message_call = expertus_error_message(msg,'error');
			$('#show_expertus_message').show();
			$('#show_expertus_message').html(message_call);
			obj.gridErrorHandler();
		},
  		error:function (XMLHttpRequest, textStatus, errorThrown) {
			var msg = new Array();
			obj.destroyLoader('report_criteria_form');
			msg[0] = Drupal.t('ERR256');
			var message_call = expertus_error_message(msg,'error');
			$('#show_expertus_message').show();
			$('#show_expertus_message').html(message_call);
			obj.gridErrorHandler();
	   	}
	}); 
	}catch(e){
 		//Nothing to do
	}
},

addDeleteUsernameFilter: function(criteriaId, type){
	try{
	var obj = this;
	var param = '';
	obj.createLoader('report_criteria_form');
	url = obj.constructUrl("administration/report-search/add-delete-username-filter/"+obj.report_id+"/"+criteriaId+"/"+type);
	//url = obj.constructUrl("administration/report-search/add-delete-mandatory/"+criteriaId+"/"+type);
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(data){
			obj.destroyLoader('report_criteria_form');
			obj.reportCriteria = data.criteria_details;
			obj.loadCriteria();
		}
	}); 
	}catch(e){
 		//Nothing to do
	}
},

updateCriteriaName: function(criteriaId, value){
	try{
	var obj = this;
	var param = '';
	
	if(value == ""){
		value = $('.report-fillters-list-'+criteriaId).text();
		$('#criteria_+criteriaId+').find('criteria_hidden_text').val(value);
	}	
	
	obj.createLoader('report_criteria_form');
	url = obj.constructUrl("administration/report-search/update-criteria-name/"+criteriaId+"/"+value);
	$.ajax({
		type: "POST",
		dataType: 'json',
		async : false,
		url: url,
		data:  param,
		success: function(data){
			obj.destroyLoader('report_criteria_form');
			if(data.result == 'updated'){
				$('#criteria_'+data.criteria_id+' .hidden_text').hide();
				$('#criteria_'+data.criteria_id+' .label .report-fillters-list-'+criteriaId).html(titleRestrictionFadeoutImage(value, 'lnrreports-search-header-filters-list-name',9));
				$('#criteria_'+data.criteria_id+' .label .report-fillters-list-'+criteriaId).attr("title",value);
				$('#criteria_'+data.criteria_id+' .label').show();
			}
		}
	}); 	
	}catch(e){
 		//Nothing to do
	}
},

reloadReportResult: function(){
	try{
	var els = document.getElementById('report_criteria_form').elements;
	var val = '';
	var elementNameAndValue = '';
	for (var i = 0, len = els.length; i < len; ++i) {
		if (els[i].tagName == "INPUT") {
			val = els[i].value;
			if(els[i].name != '' && els[i].name != undefined){
				elementNameAndValue += els[i].name + '~~' + val + Drupal.settings.custom.EXP_AC_SEPARATOR;
			}
		}
	}
	if(elementNameAndValue == ''){ elementNameAndValue = '0';}
	var obj = this;
	var resultURL = obj.constructUrl('learning/report-result-content/'+obj.report_id+'/' + obj.report_editable + '/' + 1 + '&inputNameValues='+encodeURIComponent(elementNameAndValue));
	$('#report_grid_container').setGridParam({url: resultURL});
	$('#show_expertus_message').hide();
    $('#report_grid_container').trigger("reloadGrid",[{page:1}]);
	}catch(e){
 		//Nothing to do
	}
},

/*
 * Autocomplete to fetch column names from the selected tables
 */
paintReportTableColsAutocomplete : function(){
	try{
	var reportTables = $('#listTables').val();
	$('#set_mainquercrit_QueryColumnName').autocomplete(
		"/?q=administration/report/search/tablecol-autocomplete",{
			extraParams : {
            'report_tables' :reportTables
        	},			
		minChars :3,
		max :50, 
		autoFill :true,
		mustMatch :false,
		matchContains :false,
		inputClass :"ac_input",
		loadingClass :"ac_loading"
	});
	}catch(e){
 		//Nothing to do
	}
},

reportPublishAndUnpublishdetails : function(statusType) {	
	 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
	    	var esignObj = {'popupDiv':'program-publish-unpublis-dialog',
	    			'esignFor':'addReportPublishAndUnpublish','statusType':statusType};
	    	 $.fn.getNewEsignPopup(esignObj); 		    	
	 }else{		
		 $("body").data("reportsearch").addReportPublishAndUnpublish(statusType);
	 }
	
},


addReportPublishAndUnpublish : function (statusType) {	
	try{
	var obj = this;
	var data = '';
	var msg = new Array();
	data  =  "&set_mainquerform_ReportTitle="+encodeURIComponent($("#set_mainquerform_ReportTitle").val());
	data +=  "&set_mainquerform_ReportDescription="+encodeURIComponent($("#set_mainquerform_ReportDescription").val());
	data +=  "&set_mainquerform_Theme_Name="+xss_validation($("#set_mainquerform_Theme_Name").val());
	data +=  "&set_mainquerform_statusType="+statusType;
	data +=  "&report_id="+obj.report_id;
	data +=  "&report_builder_type="+obj.report_builder_type;
	
	//change by ayyappans for 37727: When we edit the report the steps on should be click able so that we can go the section directly
	var array_of_checked_values = $("#report_visibility").val();
	data += "&set_mainquerform_report_visibility_type="+array_of_checked_values;
	data += "&set_report_type="+$('#set_report_type').val();
	url = obj.constructUrl("administration/report-search/addreportpublish");
	if(this.reportValidation(array_of_checked_values)){
		var obj = this;
		obj.createLoader("report-add-wizard");
		$.ajax({
			type: "POST",
			url: url,
			data:  data,
			success: function(result){
				//Added for #0070534
				obj.destroyLoader("report-add-wizard");
				if(result == 'NON_SCALABLE_QUERY'){
					msg[0] = Drupal.t('ERR255');
					var message_call = expertus_error_message(msg,'error');
					$('#show_expertus_message').show();
					$('#show_expertus_message').html(message_call);
				}else if(result=='Failure'){
					msg[0] = Drupal.t('ERR175');
					var message_call = expertus_error_message(msg,'error');
					$('#show_expertus_message').show();
					$('#show_expertus_message').html(message_call);
					return false;
				}else{
					obj.saveFlag = true;
					$('#show_expertus_message').html('');
					$('#show_expertus_message').hide();
					
					msg[0] = Drupal.t('MSG600');
					var message_call = expertus_error_message(msg,'status');
 					$('#show_expertus_message').html(message_call);
					$('#show_expertus_message').show();
					$('.catalog-pub-add-list').css('display','none');
					if( statusType == 'cre_rpt_rps_itv') {
						$('#edit-catalog-course-save-unpublish').val(Drupal.t('LBL614'));
						$("#edit-catalog-course-save-publish").attr("onClick","$(\'body\').data(\'reportsearch\').reportPublishAndUnpublishdetails(\'cre_rpt_rps_itv\')");
						$("#edit-catalog-course-save-unpublish").attr("onClick","$(\'body\').data(\'reportsearch\').reportPublishAndUnpublishdetails(\'cre_rpt_rps_atv\')");
					}
					else if( statusType == 'cre_rpt_rps_atv') {
						$('#edit-catalog-course-save-unpublish').val(Drupal.t('LBL571'));
						$("#edit-catalog-course-save-unpublish").attr("onClick","$(\'body\').data(\'reportsearch\').reportPublishAndUnpublishdetails(\'cre_rpt_rps_itv\')");
						$("#edit-catalog-course-save-publish").attr("onClick","$(\'body\').data(\'reportsearch\').reportPublishAndUnpublishdetails(\'cre_rpt_rps_atv\')");
					}
					//setTimeout(function() {
					   // $('#report-success-resultmsg-div').fadeOut('fast');
					//}, 10000); 
					return false;
				}
			},
			failure: function(data){
				var msg = new Array();
				obj.destroyLoader('report-add-wizard');
				msg[0] = Drupal.t('ERR256');
				var message_call = expertus_error_message(msg,'error');
				$('#show_expertus_message').show();
				$('#show_expertus_message').html(message_call);
				obj.gridErrorHandler();
			},
	  		error:function (XMLHttpRequest, textStatus, errorThrown) {
				var msg = new Array();
				obj.destroyLoader('report-add-wizard');
				msg[0] = Drupal.t('ERR256');
				var message_call = expertus_error_message(msg,'error');
				$('#show_expertus_message').show();
				$('#show_expertus_message').html(message_call);
				obj.gridErrorHandler();
		   	}
	    }); 
	}
	}catch(e){
 		//Nothing to do
	}
},

/*
 * Report field validation
 */
reportValidation : function(array_of_checked_values) {
	try {
	var setError = 0 ;
	var errorMsg = new Array();
	var i = 0;
	
	var reportTitle = $("#set_mainquerform_ReportTitle").val();
	var reportDescr = $("#set_mainquerform_ReportDescription").val();
	if (reportTitle !== xss_validation(reportTitle) || reportDescr !== xss_validation(reportDescr)) {
		errorMsg[0] = Drupal.t('LBL1273');
		setError = 1;
		i++;
	} 
	if (reportTitle == '') {
		errorMsg[i] = Drupal.t('LBL799') +  Drupal.t('ERR101');
		setError = 1;
		i++;
	}
	//change by ayyappans for 37727: When we edit the report the steps on should be click able so that we can go the section directly
	if (array_of_checked_values == null || array_of_checked_values.length == 0) {
		errorMsg[i] = Drupal.t('LBL800') + Drupal.t('ERR101');
		setError = 1;
	}
	if (setError) {
		var message_call = expertus_error_message(errorMsg, 'error');
		$('#show_expertus_message').show();
		$('#show_expertus_message').html(message_call);
		return false;
	}
	else {
		return true;
	}
	}catch(e){
 		//Nothing to do
	}
},

/*
 * Autocomplete to fetch column names from the selected tables
 */
paintReportCriteriaAutocomplete : function(criteriaId){
	try{
	$('#'+jqSelector(criteriaId)).autocomplete(
		"/?q=administration/report/search/criteria-autocomplete",{
			extraParams : {
            'criteria_id' :criteriaId
        	},			
		minChars :3,
		max :50, 
		autoFill :true,
		mustMatch :false,
		matchContains :false,
		inputClass :"ac_input",
		loadingClass :"ac_loading"
	});
	}catch(e){
 		//Nothing to do
	}
},

getReportResults : function (id,criteriaType) {
	try{
	var obj = this;
	var attrUrl = '';
		$('#report-launch-section-'+id).find("input").each(function() {
			attrUrl += '&'+$(this).attr('id')+'='+$("#"+jqSelector($(this).attr('id'))).val();
		});

	url = obj.constructUrl('administration/report/'+id+'&id='+id+attrUrl+'&criteria_type='+criteriaType);
	window.open(url,'ReportWindow','width=auto,height=auto,menubar=yes,status=yes,location=yes,toolbar=yes,scrollbars=yes');
	}catch(e){
 		//Nothing to do
	}
},

getQueryBuilderTables : function(jsonobj,autoCompleteTableName) {
	try{
	var obj = this;
	if(autoCompleteTableName != "" && autoCompleteTableName!=undefined) {
		//url = obj.constructUrl("administration/report/search/getreporttables-auto/"+autoCompleteTableName);
		if(autoCompleteTableName == 0) {
			$('#query_builder_tables .reportListItem').each(function(){
				$(this).show();
			});
		} else {
			$('#query_builder_tables .reportListItem').hide();
			$('#query_builder_tables div[alias*="'+autoCompleteTableName+'"]').show();
		}
	}else{
		url = obj.constructUrl("administration/report/search/getreporttables");
	
		$.ajax({
			type: "POST",
			url: url,
			async: false,
			data:  '',
			success: function(result){
				var tableCount	= $(result).length;
				var totalPages	= tableCount/20;
				var masterHtml = '';
				
			    var loop=1;
			    var pageCount=1;
		    	$(result).each(function(index, domEle) {
		    		var html = '';
		    		var setId = "data={'rootTableId':'"+result[index].dbtable+"'}";
		    		html+='<div id="space_tbl_'+result[index].dbtable+'" alias="'+result[index].dbtable_display+'" data="'+setId+'" class="reportListItem ui-draggable">';
		    		html+='<div id="tbl_'+result[index].dbtable+'" value="tbl_'+result[index].dbtable_display+'" class="tag_cell ui-widget-content table-title-container" ><div id="tbl_container"><span class="vtip" id="tbl_name" title="'+htmlEntities(result[index].dbtable_display)+'" value="'+result[index].dbtable+'">'+titleRestrictionFadeoutImage(result[index].dbtable_display,'lnrreports-search-display-table-name',16)+'</span><span id="add_icon" class="add-icon" onclick="toggleDisplay(\'col_tbl_'+result[index].dbtable+'\',\''+result[index].dbtable+'\')"></span>';
		    		html+='<div class="report-table-header-details"><span id="col_tbl_'+result[index].dbtable+'" value="col_tbl_'+result[index].dbtable_display+'" class="table-content-container scroll-pane-report-wizard scroll-pane-report-related-tables-'+result[index].dbtable+'" style="display:none; z-index:1;" >';
		    		html+='<div class="description">'+result[index].dbtable_description+'</div>';
		    		html+='<input type="hidden" name="rel_tbl_'+result[index].dbtable+'" id="rel_tbl_'+result[index].dbtable+'" value="'+result[index].related_table_names+'" />';
		    		html+='<input type="hidden" name="rel_alias_tbl_'+result[index].dbtable+'" id="rel_alias_tbl_'+result[index].dbtable+'" value="'+result[index].related_table_alias_names+'" />';
		    		html+='<input type="hidden" name="rel_fields_'+result[index].dbtable+'" id="rel_fields_tbl_'+result[index].dbtable+'" value="'+result[index].related_table_column_relation+'" />';
		    		var arrColumnName = result[index].dbcolumns;
		    		var arrColumnNameDisplay = result[index].dbcolumns_display;
		    		var arrColumnNameMapList = result[index].dbcolumns_map_list;
		    		// obejct convert to string result[index].dbcolumns.toString()
		    		// get the type alert(typeof columnName);
		    		//var arrColumnName = columnName.split(",");
		    		var comTabCol = '';
		    		var comTabColDisp = '';
		    		if(arrColumnName!= undefined && arrColumnName.length > 0){
		    			html+='<ul>';
			    		for (j=0,j1=0;j<arrColumnName.length;j++)  {
			    			comTabCol = result[index].dbtable +'.'+arrColumnName[j];
			    			comTabColDisp = arrColumnNameDisplay[j];
			    			comTabColMapList = arrColumnNameMapList[j]; 
			    			html+='<li class="'+result[index].dbtable +'" id="qtipcol_'+j+result[index].dbtable+'"><span id="'+comTabCol+'" title="'+htmlEntities(comTabColDisp)+'" class="'+result[index].dbtable +''+arrColumnName[j]+' vtip table-accordion-content">'+titleRestrictionFadeoutImage(comTabColDisp,'lnrreports-search-table-accordion-content',15)+'</span>';
			    			if(obj.currTheme == 'expertusoneV2'){
			    			html+='<span class="report-table-column-check checkbox-unselected"><input name="check_report_columns" data="'+comTabColMapList+'" id="check_'+comTabCol+'" value="'+comTabCol+'" type="checkbox" onclick="checkboxSelectedUnselectedCommon(this);"></span>';
			    			}else{
			    			html+='<span class="report-table-column-check"><input name="check_report_columns" data="'+comTabColMapList+'" id="check_'+comTabCol+'" value="'+comTabCol+'" type="checkbox"></span>';
			    			}
			    			html+='</li>';
			    		}
			    		html+='</ul>';
		    		}
		    		html+='</span></div></div>';
		    		html+='</div>';
		    		html+='</div>';
		    		//change by ayyappans for 39371: Issue in Reports
		    		var hiddenDiv = $(html);
		    		hiddenDiv.find('.scroll-pane-report-wizard').removeClass('scroll-pane-report-related-tables-'+result[index].dbtable);
		    		hiddenDiv = "<div id='hidden_tbl_"+result[index].dbtable+"' style='display: none;'>"+hiddenDiv.html()+"</div>";
					masterHtml += html;
					masterHtml += hiddenDiv;
		    	});
		    	masterHtml+='</div>';
				
		    	$("#query_builder_tables").html(masterHtml);
				
				$(document).ready(function() {
				    $(".reportListItem").draggable({
				        revert: true
				    });

				    var relatedTableNames	= '';
					var relatedAliasTableNames	= '';
					var firstDrag=true;
				    
				    $("#cart_items").droppable({
				        accept: ".reportListItem",
				        activeClass: "drop-active",
				        drop: function(event, ui) {
				    		var mouseEvent = ui.offset;
				            var itemid = ui.draggable.attr("id");
				            var parentTableId = itemid.replace('space_', ''); 

				            var rootId = eval($(ui.draggable).attr("data"));
					        var tableId = rootId.rootTableId;
					        //var tableAliasId = $('#tbl_'+tableId).attr('value');
					        var tableAliasId = $(ui.draggable).attr("alias");
					        
					        if($('#cart_items .icart').length == 0){
					        	obj.parentTableId = parentTableId;
					        }
					        //INITITALIES THE CUSTOM SCROLL BAR PLUGINS
					        $("body").data("reportsearch").setJsScrollpane('scroll-pane-drop-container');
							$("body").data("reportsearch").showRelatedTableInContainer(parentTableId, tableAliasId);
							$("body").data("reportsearch").setAndRemoveDraggable();
							//VALIDATE THE CART_ITEM CONTAINER HEIGHT
							$("body").data("reportsearch").setJsScrollpane('scroll-pane-drop-container');
							
							$('body').data('reportsearch').validateTableRelations();

							//CUSTOM TOOL TIP FOR SHOWING THE RESTRICTED CHARACTED
							  vtip();
							  
							var dragOptions = {
							  opacity: 0.85,
							  cursorAt: { left: 7, top: 7 },
							  containment: "#cart_items",
                              scroll: false,
                              snapMode: "inner",
                              snapTolerance: 30,
                              snap: "#cart_items",
                stop: function(event, ui) {
                        jsPlumb.repaint(ui.helper);
                      }
						  };
							jsPlumb.draggable(jsPlumb.getSelector(".window"), dragOptions);
				        }
				    });
				});
			}
	    });
	}
	$("body").data("reportsearch").setJsScrollpane('scroll-pane');
	}catch(e){
 		//Nothing to do
	}
},

setAndRemoveDraggable : function(){
	try{
	var obj= this;
	
	var parentTableId = '';
	var containerTableList = new Array();
	var c = 0;
	$('#query_builder_tagsDrop .icart .table-title-container').each(function(){
		containerTableList[c] = $(this).attr('id');
		c++;
	});

	$("#query_builder_tables .reportListItem").draggable({ disabled: true });
	
	for(var j=0; j<c; j++){
		parentTableId = containerTableList[j];
		var relationTableList = $("#rel_"+parentTableId).val();
		var relationTableListArray = relationTableList.split(',');
		var relationTableLen = relationTableListArray.length;
		for(var i=0; i<relationTableLen; i++){
			$("#space_"+relationTableListArray[i]).draggable({ disabled: false });
		}
	}
	if($('#cart_items .jspContainer .jspPane .icart').length == 0){
		$("#query_builder_tables .reportListItem").draggable({ disabled: false });
	}
	}catch(e){
 		//Nothing to do
	}
},

removeAllInterlink: function(){
	try{
	var existingTableList = $("#listTables").val();
	var existingTableListArray = existingTableList.split(',');
	
	for(var i = 0; i< existingTableListArray.length; i++){
		jsPlumb.detachAllConnections(existingTableListArray[i]);
	}
	
	$('body').data('reportsearch').validateTableRelations();
	}catch(e){
 		//Nothing to do
	}
},


interlinkRelatedTables : function(window1, window2){
	try{
	var hoverColor = '#ec9f2e';
	if(this.currTheme == 'expertusoneV2'){
	  hoverColor = '#EC008C';
	}
	 var labelsText = $('body').data('reportsearch').getTableFieldRelation(window1, window2);
	 jsPlumb.connect({
			source:'line_tbl_'+window1, 
			target:'line_tbl_'+window2, 
			anchors:["BottomCenter", "TopCenter"], 
			paintStyle: { strokeStyle: "gray", lineWidth: 2 },
			hoverPaintStyle:{strokeStyle: hoverColor },
			connector:"Straight", 
			endpointsOnTop:true,
			overlays:[ ["Label", {
							cssClass:"component label",		    			        				 
							label : labelsText 
                     }]    			    
			]});
	 
	 $('#cart_items ._jsPlumb_connector').each(function(){
		 var relHtml = $(this).next().html();
		 $(this).next().hide();
		 
		 $(this).attr('title', relHtml);
		 
		 var connector1Html = $(this).prev().attr('title');
		 if(connector1Html ==''){
			 $(this).prev().attr('title', relHtml);
		 } 

		 var connector2Html = $(this).prev().prev().attr('title');
		 if(connector2Html ==''){
			 $(this).prev().prev().attr('title', relHtml);
		 } 

	 });
	 
	 vtip();
	}catch(e){
 		//Nothing to do
	}
},

getTableFieldRelation: function(window1, window2){
 try{	
	var relatedField = $('#rel_fields_tbl_'+window1).val();
	var relatedFieldArray = relatedField.split(',');
	var relatedFieldJoinArray = new Array();
	var relatedTableFieldsArray = new Array();
	var table1 = '';
	var table2 = '';
	var relCount = 0;
	for(var i = 0; i < relatedFieldArray.length; i++){
		relatedTableFieldsArray = relatedFieldArray[i].split('=');
		
		table1Array = relatedTableFieldsArray[0].split('.');
		table2Array = relatedTableFieldsArray[1].split('.');
		
		table1 = table1Array[0]; 
		table2 = table2Array[0];
		
		if( (window1 == table1 && window2 == table2) || (window1 == table2 && window2 == table1)){
			relatedFieldJoinArray[relCount] = relatedFieldArray[i];
			relCount++;
		}
		
	}
	return relatedFieldJoinArray.join('\n');
 }catch(e){
		//Nothing to do
	}
},

getWindowPosition : function(){
	try{
	var windowArray = new Array();
	var count = 0, position = '';
	$('.window').each(function(){
		
		var childPos = $(this).offset();
		var parentPos = $(this).parent().offset();
		position = {
		    top: childPos.top - parentPos.top,
		    left: childPos.left - parentPos.left
		}
		windowArray[count] = $(this).attr('id')+ '-' +position.left+'-'+position.top;
		count++;
	});
	var windowsPosition = windowArray.join(',');
	$("#windowPositions").val(windowsPosition)
	}catch(e){
 		//Nothing to do
	}
},

setWindowPosition : function(windowsPosition){
	try{
	if(windowsPosition != '' && windowsPosition != null){
		var windowPositionArray = windowsPosition.split(',');
		var windowPositionLen = windowPositionArray.length;
		var positionArray = new Array();
		for(var i=0; i<windowPositionLen; i++){
			positionArray = windowPositionArray[i].split('-');
			var parentPos = $("#"+positionArray[0]).parent().offset();
			position = {
				    top: parseInt(positionArray[2]) + parseInt(parentPos.top),
				    left: parseInt(positionArray[1]) + parseInt(parentPos.left)
			}
			$("#"+positionArray[0]).offset({ top: position.top, left: position.left});
			// To remove all previous connections
			jsPlumb.detachAllConnections(positionArray[0]);
		}
		$('body').data('reportsearch').validateTableRelations();
	}
	}catch(e){
 		//Nothing to do
	}
},

showRelatedTableInContainer : function(parentTableId, tableAliasId){
	try{
	//change by ayyappans for 39371: Issue in Reports
	var parentHtml = $('#hidden_'+parentTableId).html();
	var html = '<div class="item icart window component" id="line_'+parentTableId+'">';
	html = html + '<div class="divrm"><div class="query-tables-left"><div class="query-tables-right"><div class="query-tables-middle"></div></div></div>';
	if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
	html = html + '<div class="droppable-table-list-fields"><a onclick="$(\'body\').data(\'reportsearch\').removeItemFrmContainer(\''+parentTableId+'\', \''+tableAliasId+'\',\'parent\'); return false;" class="remove space_'+parentTableId+'"><img src="sites/all/themes/core/expertusoneV2/expertusone-internals/images/spacer.gif" width="20" height="20" /></a>';
	}else{
		html = html + '<div class="droppable-table-list-fields"><a onclick="$(\'body\').data(\'reportsearch\').removeItemFrmContainer(\''+parentTableId+'\', \''+tableAliasId+'\',\'parent\'); return false;" class="remove space_'+parentTableId+'"><img src="sites/all/themes/core/expertusone/expertusone-internals/images/spacer.gif" width="20" height="20" /></a>';	
	}
	html = html + parentHtml;
	html = html +'</div><div class="query-tables-footer-left"><div class="query-tables-footer-right"><div class="query-tables-footer-middle"></div></div></div></div>';
	$("#cart_items > .jspContainer > .jspPane").append(html);
	$("#cart_items").find("#col_"+parentTableId).css('display','block');
	
	$("#space_" + parentTableId).css('display','none');
	$('body').data('reportsearch').selectedTableList(parentTableId, tableAliasId, 'add');
	
	var existingTableList = $("#listTables").val();
	var existingTableArray = existingTableList.split(',');
	var tableCount = existingTableArray.length;
	var window1 = parentTableId.replace('tbl_','');
	
	if(tableCount == 1){
		$('#line_'+parentTableId).css({'top':0,'left':150});
	} else {
		var tableRow = parseInt(tableCount / 2);
		
		if(tableCount % 2 == 0){ // Even
			$('#line_'+parentTableId).css({'top': 30,'left':15, 'display':'inline-block'});
		} else {
			$('#line_'+parentTableId).css({'top': 30,'left':15, 'display':'inline-block'});
		}
	}
	/* 	change by ayyappans
	 * 	34269: UI issue in Discussions, Reports, and Delete Pop up
	 * 	2. In Reports if having two or more filds in the table means showing below more empty space
	 * 	Fix: jScrollPane is initialized if the list items are more than 5
	 * */
	if($('#cart_items #col_tbl_'+window1).find('ul li').length > 5 ) {
		$("body").data("reportsearch").setJsScrollpane('tableCustomscrollbar', window1);
	}
	
	$("body").data("reportsearch").setJsScrollpane('scroll-pane-drop-container');
	}catch(e){
 		//Nothing to do
	}
},

removeItemFrmContainer : function(tableId, tableAliasId, type){
	try{
	$('body').data('reportsearch').selectedTableList(tableId, tableAliasId, 'remove');
	
	// To remove parent line
	jsPlumb.detachAllConnections('line_'+tableId);

	$('#space_'+tableId).draggable({ disabled: false });
	$('#cart_items #'+tableId).parent().parent().parent().remove();

	$('#space_'+tableId).css({'display':'block','left':'0px','top':'0px'});
	
    var relationTableList = $('#rel_'+tableId).val();
    var relationTableAliasList = $('#rel_alias_'+tableId).val();

    // Commenting the remove of child tables / related tables - Can be used in future
    /*
    if(relationTableList != ''){
	    var relationTableListArray = relationTableList.split(','); 
	    var relationTableAliasListArray = relationTableAliasList.split(',');
	    var relationTableListLen = relationTableListArray.length; 

		var containerTableList = new Array();
		var c = 0;
		$('#query_builder_tagsDrop .icart .table-title-container').each(function(){
			containerTableList[c] = $(this).attr('id');
			c++;
		});
	    
	    var relationTable = '';
	    var relationTableAlias = '';
	    var relationToRelationArray = new Array();
	    for(var i=0; i<relationTableListLen; i++){
	    	relationTable = relationTableListArray[i];
	    	relationTableAlias = relationTableAliasListArray[i];
	    	relationToRelationArray = $('#rel_'+relationTable).val().split(',');
	    	var relationAvailable = 0;
	    	for(var j=0; j<relationToRelationArray.length;j++){
		    	if($.inArray(relationToRelationArray[j], containerTableList) >= 0){
		    		relationAvailable = 1;
				}
			}
			if(relationAvailable == 0){
		    	$('body').data('reportsearch').selectedTableList(relationTable, relationTableAlias, 'remove');
		    	$('#space_'+relationTable).draggable({ disabled: false });
		    	
		    	// To remove child line
		    	//jsPlumb.detachAllConnections('line_'+relationTable);
		    	
				$('#cart_items #'+relationTable).parent().parent().parent().remove();
				$('#space_'+relationTable).css({'display':'block','left':'0px','top':'0px'});
			}
		}		
    }
    */
    $("body").data("reportsearch").setAndRemoveDraggable();
	$("body").data("reportsearch").setJsScrollpane('scroll-pane-drop-container');
	//$('body').data('reportsearch').removeAllInterlink();
	}catch(e){
 		//Nothing to do
	}
},

selectedTableList : function(tableId, tableAliasId, type){
	try{
    var existingTableList = $("#listTables").val();
    var existingTableAliasList = $("#listTablesDisplay").val();
    var existingTableListArray = new Array();
    var existingTableAliasListArray = new Array();
    var existingTableCount = 0;
    if(existingTableList != ''){
	    existingTableListArray = existingTableList.split(',');
	    existingTableAliasListArray = existingTableAliasList.split(',');
	    existingTableCount = existingTableListArray.length;
    }
	tableId = tableId.replace('tbl_', '');
	tableAliasId = tableAliasId.replace('tbl_', '');
	if(type == 'add'){
		if($.inArray(tableId, existingTableListArray) == -1){
		    existingTableListArray[existingTableCount] = tableId;
		    existingTableAliasListArray[existingTableCount] = tableAliasId;
	    }
	} else {
		var removeTableIndex = $.inArray(tableId, existingTableListArray); 
		var removeTableAiasIndex = $.inArray(tableAliasId, existingTableAliasListArray);
		
		if ( ~removeTableIndex ) existingTableListArray.splice(removeTableIndex, 1);
		if ( ~removeTableAiasIndex ) existingTableAliasListArray.splice(removeTableAiasIndex, 1);
		
	}
	
	selectedTableList = existingTableListArray.join(',');
	selectedTableAliasList = existingTableAliasListArray.join(',');
	
	$("#listTables").val(selectedTableList);
	$("#listTablesDisplay").val(selectedTableAliasList);
	
	//JAVASCRIPT CUSTOM SCROLL BAR FUNCTION CALL
	//$("body").data("reportsearch").setJsScrollpane('tableCustomscrollbar',tableId);
	}catch(e){
 		//Nothing to do
	}
},

validateTableRelations : function(){
	try{
	var parentTableId = '';
	var newRelatedTableFieldsArray = new Array();
	var newRelatedTableFieldsCount = 0;
	
	var relatedTables = $('#listTables').val();
	var relatedTablesArray = relatedTables.split(',');
	var interlinkedTablesArray = new Array();
	var iCount = 0;
	for(var j=0; j<relatedTablesArray.length; j++){
	
		parentTableId = relatedTablesArray[j]; 
		var relatedTableFields = $('#rel_fields_tbl_'+parentTableId).val();
		if(relatedTableFields != ''){
			var relatedTableFieldsArray = relatedTableFields.split(',');
			var relatedTablesFieldsLen = relatedTableFieldsArray.length;
			var relatedTableFieldRel = '';
			
			var relatedTableFieldRel1 = '';
			var relatedTableFieldRel2 = '';
			
			var relatedTableFieldRelArray = new Array();
			var table1Exist = 0; var table2Exist = 0;
			
			for(var i=0; i<relatedTablesFieldsLen; i++){
				table1Exist = 0; table2Exist = 0;
				relatedTableFieldRel = relatedTableFieldsArray[i];
				if(relatedTableFieldRel != '' && relatedTableFieldRel != null){
					relatedTableFieldRelArray = relatedTableFieldRel.split('=');
					relatedTableFieldRel1 = relatedTableFieldRelArray[0];
					relatedTableFieldRel2 = relatedTableFieldRelArray[1]; 
					
					if(relatedTableFieldRel1 !='' && relatedTableFieldRel2 !=''){
						relatedTableName1Array = relatedTableFieldRel1.split('.');
						relatedTableName2Array = relatedTableFieldRel2.split('.');
						
						relatedTableName1 = $.trim(relatedTableName1Array[0]);
						relatedTableName2 = $.trim(relatedTableName2Array[0]);
						
						if($.inArray(relatedTableName1, relatedTablesArray) >= 0){
							table1Exist = 1;
						}
						if($.inArray(relatedTableName2, relatedTablesArray) >= 0){
							table2Exist = 1;
						}
						
						if(table1Exist && table2Exist){
							newRelatedTableFieldsArray[newRelatedTableFieldsCount] = relatedTableFieldRel;
							newRelatedTableFieldsCount++;
							if($.inArray(relatedTableName1+'-'+relatedTableName2, interlinkedTablesArray) == -1 && $.inArray(relatedTableName2+'-'+relatedTableName1, interlinkedTablesArray) == -1){
								interlinkedTablesArray[iCount] = relatedTableName1+'-'+relatedTableName2;
								iCount++;
								$('body').data('reportsearch').interlinkRelatedTables(relatedTableName1, relatedTableName2);
							}
						}
					}
				}
			}
		}
	}
	$("#Relcoloumn").val('');
	$("#Relcoloumn").val(newRelatedTableFieldsArray.join(','));	
	}catch(e){
 		//Nothing to do
	}
},

validateReportQueryBuilder:function(pageFrom){
	try{
	var obj = this;
	var errormsg =new Array();
	if($('#listTables').val()==''){
		errormsg[0] = Drupal.t('ERR172'); //Select at least one table from the list.
		var message_call = expertus_error_message(errormsg,'error');
		$('#show_expertus_message').show();
		$('#show_expertus_message').html(message_call);
		obj.current_step_div = 'report-data-source';
		return false;
	}else if(getSelectedColumns()==''){
		errormsg[0] = Drupal.t('ERR173'); //Select at least one column from the selected tables.
		var message_call = expertus_error_message(errormsg,'error');
		$('#show_expertus_message').show();
		$('#show_expertus_message').html(message_call);
		obj.current_step_div = 'report-data-source';
		return false;
	}else{
	  jsPlumb.repaintEverything();
		//Create new record and store the report information on slt_report_query_builder table
		$('body').data('reportsearch').validateTableRelations();
		$('body').data('reportsearch').getWindowPosition();
		
		var data='';
		data =  "&table_sql="+$('#listTables').val();
		data += "&table_alias_sql="+$('#listTablesDisplay').val();
		data += "&column_sql="+getSelectedColumns();
		data += "&column_alias_sql="+getSelectedAliasColumns();
		data += "&report_details_id="+obj.report_id;
		data += "&rel_column="+$("#Relcoloumn").val();
		data += "&map_list_items="+getSelectedColumnsMap();
		data += "&window_positions="+$("#windowPositions").val();
		if(pageFrom == undefined) {
			pageFrom = 'report-data-source';
		}
		ajaxCall = true;	//change by ayyappans for 40028: Unable to create design wizard report when clicking next option double click.
		url = obj.constructUrl("administration/report/search/edit-report-data");
		obj.createLoader('report-add-wizard');
		$.ajax({
			type: "POST",
			url: url,
			data:  data,
			dataType: 'json',
			success: function(result){
				repQryState = result.result;
				obj.loadReportLayout(result.report_id, pageFrom, 0);
				obj.current_step_div = 'report-builder-section'; 
				obj.report_id = result.report_id;
				ajaxCall = false;
				obj.destroyLoader('report-add-wizard');
			}
	    });

	    $(".previousLink-inactive").removeClass().addClass("previousLink");
		if(obj.report_id==undefined) {
			obj.editstatus = '';
			$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-selected');
			$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-div-completed');
		}
		else {
			$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-div-completed');
			obj.editstatus = 'edit';
			if(obj.publish_title == ''){
				$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-selected');
			} else {
				$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-edit-selected');
			}
		}
	    
	    
	}
	}catch(e){
 		//Nothing to do
	}
},
navigateHeader:function(action, gotoStep, editStatus){
	try{
	$('#show_expertus_message').html('');
	$('#show_expertus_message').hide();
	var obj= this;
	var errmsg = new Array();
	errmsg[0] = Drupal.t('ERR166');
	if(action=='previous'){
		if(obj.current_step_div=='report-builder-section'){

			if(obj.report_builder_type == 'design_wizard') {
				obj.current_step_div = 'report-data-source';	
			} else if(obj.report_builder_type == 'query_builder'){
				obj.current_step_div = 'report-query-builder-section';
			}
			
			$('#'+obj.current_step_div).show();
			$('#report-builder-section').hide();
			$('#report-query-details').hide();	
			$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-div-selected');
			$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div');
			$(".previousLink").removeClass().addClass("previousLink-inactive");
			$(".nextLink-inactive").removeClass().addClass("nextLink");
			if(obj.editstatus == 'edit') {
				$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-div-edit-selected');
				if(obj.publish_title == '') {
					$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-grey-end-prev');
				}
				else
					$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-next-div-completed');
			}
		}else if(obj.current_step_div=='report-query-details'){
			$("#report-add-wizard").dialog({height:'auto'});
			
			obj.current_step_div = 'report-builder-section';
			$('#'+obj.current_step_div).show();
			$('#report-data-source').hide();
			$('#report-query-builder-section').hide();
			$('#report-query-details').hide();
			$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-div-completed');
			$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-selected');
			$('.breadcrump-container #report-query-details-breadcrump').removeClass().addClass('reports-title-div-end');
			$(".nextLink-inactive").removeClass().addClass("nextLink");
			$("body").data("reportsearch").loadReportDetails();
			if(obj.editstatus == 'edit') {
				$('.breadcrump-container #report-query-details-breadcrump').removeClass().addClass('reports-title-div-edit-end');
				if(obj.publish_title == '') {
					$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-selected');
					$('.breadcrump-container #report-query-details-breadcrump').removeClass().addClass('reports-title-div-end');
				}
				else
					$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-edit-selected');
			}
			
		}
	}else if(action=='next'){
		if(obj.current_step_div=='report-data-source' || obj.current_step_div=='report-query-builder-section'){
			if(obj.current_step_div=='report-data-source'){
				$('#show_expertus_message').hide();
				obj.current_step_div = 'report-builder-section';
				$('body').data('reportsearch').validateReportQueryBuilder();
			} else if(obj.current_step_div=='report-query-builder-section' && ($('#qry_builder_area').val()=='' || $('#qry_builder_area').val() == Drupal.t('MSG497'))){
				var message_call = expertus_error_message(errmsg,'error');
				$('#show_expertus_message').show();
				$('#show_expertus_message').html(message_call);
				$('#report-add-wizard .previousLink').css('background','none');
			}else{
				$('#show_expertus_message').hide();
				obj.current_step_div = 'report-builder-section';
				$('body').data('reportsearch').setQueryBuilder();
				if(obj.current_step_div != 'report-query-builder-section') {
					$(".previousLink-inactive").removeClass().addClass("previousLink");
				}				
			}
		}else if(obj.current_step_div=='report-builder-section'){
			if(ajaxCall) {	//change by ayyappans for 40028: Unable to create design wizard report when clicking next option double click.
				return false;
			}
			$("#report-add-wizard").dialog({height:'auto'});
			obj.current_step_div = 'report-query-details';
			$('#'+obj.current_step_div).show();
			$('#report-builder-section').hide();
			$('#report-data-source').hide();
			$('#report-query-builder-section').hide();
			$('.breadcrump-container #report-query-details-breadcrump').removeClass().addClass('reports-title-div-end-selected');
			$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-completed');
			$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-next-div-completed');
			$(".nextLink").removeClass().addClass("nextLink-inactive");
			
			if(obj.reportSelectedCategory!=''){
				$('#set_report_type').val(obj.reportSelectedCategory);
			}
			$("body").data("reportsearch").loadReportDetails();
			$(".previousLink-inactive").removeClass().addClass("previousLink");
		}
	}
	//change by ayyappans for 37727: When we edit the report the steps on should be click able so that we can go the section directly
	else if(action == 'show' && editStatus != 'new' && (obj.current_step_div != gotoStep)) {
		$('#show_expertus_message').hide();
		switch(gotoStep) {
		case 'report-data-source': case 'report-query-builder-section':
			$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-div-selected');
			$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div');
			$(".previousLink").removeClass().addClass("previousLink-inactive");
			$(".nextLink-inactive").removeClass().addClass("nextLink");
			if(obj.editstatus == 'edit') {
				$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-div-edit-selected');
				if(obj.publish_title == '') {
					$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-grey-end-prev');
					$('.breadcrump-container #report-query-details-breadcrump').removeClass().addClass('reports-title-div-end');
				}
				else {
					$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-next-div-completed');
					$('.breadcrump-container #report-query-details-breadcrump').removeClass().addClass('reports-title-div-edit-end');
				}
			}
			$('#report-builder-section').hide();
			$('#report-query-details').hide();
			$('#'+gotoStep).show();
			obj.current_step_div = gotoStep;
			break;
		case 'report-builder-section':
			if(obj.report_builder_type == 'design_wizard') {
				if($('#listTables').val()==''){
					var message_call = expertus_error_message([Drupal.t('ERR172')],'error');
					$('#show_expertus_message').show();
					$('#show_expertus_message').html(message_call);
					obj.current_step_div = 'report-data-source';
					return false;
				}else if(getSelectedColumns()==''){
					var message_call = expertus_error_message([Drupal.t('ERR173')],'error');
					$('#show_expertus_message').show();
					$('#show_expertus_message').html(message_call);
					obj.current_step_div = 'report-data-source';
					return false;
				}
				else {
					if(obj.current_step_div != 'report-query-details') {
						$('body').data('reportsearch').validateReportQueryBuilder(obj.current_step_div);
					}
					else {
						obj.loadReportLayout(obj.report_id, obj.current_step_div, 0);
					}
				}
			} else if(obj.report_builder_type == 'query_builder') {
				if($('#qry_builder_area').val() == '' || $('#qry_builder_area').val() == Drupal.t('MSG497')) {
					var message_call = expertus_error_message(errmsg,'error');
					$('#show_expertus_message').show();
					$('#show_expertus_message').html(message_call);
					return false;
				}
				else {
					if(!$('body').data('reportsearch').setQueryBuilder(obj.current_step_div)) {
						return false;
					}
				}
			}
			
			if(obj.current_step_div == 'report-query-details'){
				$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-div-completed');
				$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-selected');
				$('.breadcrump-container #report-query-details-breadcrump').removeClass().addClass('reports-title-div-end');
				$(".nextLink-inactive").removeClass().addClass("nextLink");

				if(obj.editstatus == 'edit') {
					$('.breadcrump-container #report-query-details-breadcrump').removeClass().addClass('reports-title-div-edit-end');
					if(obj.publish_title == '') {
						$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-selected');
						$('.breadcrump-container #report-query-details-breadcrump').removeClass().addClass('reports-title-div-end');
					}
					else {
						$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-edit-selected');
					}
				}

			}
			$(".previousLink-inactive").removeClass().addClass("previousLink");
			obj.current_step_div = 'report-builder-section';
			break;
		case 'report-query-details':
			if(obj.report_builder_type == 'query_builder' && ($('#qry_builder_area').val() == '' || $('#qry_builder_area').val() == Drupal.t('MSG497'))) {
				var message_call = expertus_error_message(errmsg,'error');
				$('#show_expertus_message').show();
				$('#show_expertus_message').html(message_call);
				return false;
			}
			else if(obj.current_step_div == 'report-query-builder-section') {
				if(!$('body').data('reportsearch').validateQueryBuilder()) {
					return false;
				}
			}
			else if(obj.report_builder_type == 'design_wizard') {
				if($('#listTables').val() == ''){
					var message_call = expertus_error_message([Drupal.t('ERR172')],'error');
					$('#show_expertus_message').show();
					$('#show_expertus_message').html(message_call);
					return false;
				}else if(getSelectedColumns() == ''){
					var message_call = expertus_error_message([Drupal.t('ERR173')],'error');
					$('#show_expertus_message').show();
					$('#show_expertus_message').html(message_call);
					return false;
				}
				else if(obj.current_step_div == 'report-data-source'){
					$('body').data('reportsearch').getWindowPosition();
					$('body').data('reportsearch').saveWindowPositions();
				}
			}
			$('.breadcrump-container #report-query-details-breadcrump').removeClass().addClass('reports-title-div-end-selected');
			$('.breadcrump-container #report-builder-section-breadcrump').removeClass().addClass('reports-title-div-completed');
			$('.breadcrump-container #report-data-source-breadcrump').removeClass().addClass('reports-title-next-div-completed');
			$(".nextLink").removeClass().addClass("nextLink-inactive");

			if(obj.reportSelectedCategory!=''){
				$('#set_report_type').val(obj.reportSelectedCategory);
			}
			$("body").data("reportsearch").loadReportDetails();
			$(".previousLink-inactive").removeClass().addClass("previousLink");
			$('#report-data-source').hide();
			$('#report-query-builder-section').hide();
			$('#report-builder-section').hide();
			$('#report-query-details').show();
			obj.current_step_div = gotoStep;
			break;
		}
	}
	}catch(e){
 		//Nothing to do
	}
},
//change by ayyappans for 37727: When we edit the report the steps on should be click able so that we can go the section directly
saveWindowPositions: function() {
	var data='';
	var obj = this;
	data =  "&table_sql="+$('#listTables').val();
	data += "&table_alias_sql="+$('#listTablesDisplay').val();
	data += "&column_sql="+getSelectedColumns();
	data += "&column_alias_sql="+getSelectedAliasColumns();
	data += "&report_details_id="+obj.report_id;
	data += "&rel_column="+$("#Relcoloumn").val();
	data += "&map_list_items="+getSelectedColumnsMap();
	data += "&window_positions="+$("#windowPositions").val();

	url = obj.constructUrl("administration/report/search/edit-report-data");
	obj.createLoader('report-add-wizard');
	$.ajax({
		type: "POST",
		url: url,
		data:  data,
		dataType: 'json',
		success: function(result){
			/*obj.loadReportLayout(result.report_id, 'report-data-source', 0);
			obj.report_id = result.report_id;*/
			repQryState = result.result;
			obj.destroyLoader('report-add-wizard');
		}
    });
},
validateQueryBuilder: function(fromStep){
	try{
		var obj = this, queryStatus = true;
		var timestamp = new Date().getTime();
		//param +=  "&qryval="+$("#qry_builder_area").val()+'&timestamp='+timestamp;
		var param = {
        qryval : $("#qry_builder_area").val(),
        timestamp: timestamp
    	};
		var url = obj.constructUrl("administration/report-search/qry-build/"+obj.report_id);
		obj.createLoader('report-add-wizard');
		$.ajax({
			type: "POST",
			dataType: 'json',
			async : false,
			url: url,
			data:  param,
			success: function(result){
				obj.destroyLoader('report-add-wizard');
				if(result.Err != 'Success'){
					queryStatus = false;
					var error = new Array();
					error[0] = result.Err;
					var message_call = expertus_error_message(error,'error');
					$('#show_expertus_message').show();
					$('#show_expertus_message').html(message_call);
					obj.current_step_div = 'report-query-builder-section';
					$('#report-data-source').hide();
					$('#report-query-details').hide();
					$('#report-builder-section').hide();
					$('#report-query-builder-section').show();
				} /*else {
					obj.current_step_div = 'report-query-details';
				}*/
				repQryState = result.result;
			},
			failure: function(data){
				var msg = new Array();
				obj.destroyLoader('report-add-wizard');
				msg[0] = Drupal.t('ERR256');
				var message_call = expertus_error_message(msg,'error');
				$('#show_expertus_message').show();
				$('#show_expertus_message').html(message_call);
				obj.gridErrorHandler();
			},
	  		error:function (XMLHttpRequest, textStatus, errorThrown) {
				var msg = new Array();
				obj.destroyLoader('report-add-wizard');
				msg[0] = Drupal.t('ERR256');
				var message_call = expertus_error_message(msg,'error');
				$('#show_expertus_message').show();
				$('#show_expertus_message').html(message_call);
				obj.gridErrorHandler();
		   	}
		});
		return queryStatus;
	} catch(e) {
	}
}
});

$.extend($.ui.reportsearch.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true,catStart:true}});
})(jQuery);

$(function() {
	try{
	$("body").reportsearch();
	}catch(e){
 		//Nothing to do
	}
});
$('body').bind('click', function(event) {
	try{
	if(event.target.className != 'drp-down-arrow'){
		$('.date-drp-down-options').hide();
	}
	}catch(e){
 		//Nothing to do
	}
});
//user-filter-drop-down-arrow 
$('body').bind('click', function(event) {
	try{
	if(event.target.className != 'user-filter-drop-down-arrow'){
		$('.use-userfilter-drp-down-options').hide();
	}
	}catch(e){
 		//Nothing to do
	}
});
function setDragAndDrop() {
try{	
$( ".tag_cell" ).draggable();
var relatedTableNames	= '';
var relatedAliasTableNames	= '';
var firstDrag=true;
$( "#query_builder_dropColBox" ).droppable({
	drop: function( event, ui ) {
		var relatedTablesArray= new Array();
		var relatedAliasTablesArray = new Array();

		$("#col_" + $(ui.draggable).attr("id")).show();
		$("#" + $(ui.draggable).attr("id")).removeClass("table-title-container");
		$("#space_" + $(ui.draggable).attr("id")).css({"height":"27px"});
		$("#" + $(ui.draggable).attr("id")).css({"position":"absolute"});
		$("#" + $(ui.draggable).attr("id") + " span#add_icon").addClass("add-icon");
		$("#col_" + $(ui.draggable).attr("id") + " .description").css('display','none');
		$("#" + $(ui.draggable).attr("id") + " div#tbl_container").addClass("content-container");
		$("#" + $(ui.draggable).attr("id") + " div.query-tables-left").remove();
		$("#" + $(ui.draggable).attr("id") + " div.query-tables-right").remove();
		$("#" + $(ui.draggable).attr("id") + " div.query-tables-middle").remove();
		$("#" + $(ui.draggable).attr("id") + " div.query-tables-footer-left").remove();
		$("#" + $(ui.draggable).attr("id") + " div.query-tables-footer-right").remove();
		$("#" + $(ui.draggable).attr("id") + " div.query-tables-footer-middle").remove();
		$("#" + $(ui.draggable).attr("id") + " div#tbl_container").before('<div class="clearBoth query-tables-left"><div class="query-tables-right"><div class="query-tables-middle"></div></div></div>');
		$("#" + $(ui.draggable).attr("id") + " div#tbl_container").after('<div class="clearBoth query-tables-footer-left"><div class="query-tables-footer-right"><div class="query-tables-footer-middle"></div></div></div>');
		
		$("#col_" + $(ui.draggable).attr("id")).css({"overflow-x":"hidden"});
		$("#col_" + $(ui.draggable).attr("id")).css({"height":"100px"});
		$("#col_" + $(ui.draggable).attr("id")).css({"width":"150px"});
		$("#col_" + $(ui.draggable).attr("id") + " ul>li").css({"list-style-type":"none"});
		$("#col_" + $(ui.draggable).attr("id") + " ul").css({"padding-left":"0"});
		
		$(this).data('jdi', 'yes');
		
		var p = $("#" + $(ui.draggable).attr("id")).position();
		var relatedTables		= $("#rel_" + $(ui.draggable).attr("id")).val();

		if(relatedTables!=''){
			$("#" + $(ui.draggable).attr("id")).css({"top":"150px"});
			$("#" + $(ui.draggable).attr("id")).css({"left":"250px"});
			p.top = 150;
			p.left = 250;
			
			relatedTablesArray	= relatedTables.split(",");
			var relatedAliasTables		= $("#rel_alias_" + $(ui.draggable).attr("id")).val();
			relatedAliasTablesArray	= relatedAliasTables.split(",");
		
			var top		= p.top;
			var left	= p.left+300;
		}

		if(relatedTablesArray.length>0){
			for(var relLoop=0;relLoop<relatedTablesArray.length;relLoop++){
				$("#col_" + relatedTablesArray[relLoop]).show();
				$("#" + relatedTablesArray[relLoop]).removeClass("table-title-container");
				$("#space_" + relatedTablesArray[relLoop]).css({"height":"27px"});
				$("#" + relatedTablesArray[relLoop]).css({"position":"absolute"});
				$("#" + relatedTablesArray[relLoop] + " span#add_icon").addClass("add-icon");
				$("#col_" + relatedTablesArray[relLoop] + " .description").css('display','none');
				$("#" + relatedTablesArray[relLoop] + " div#tbl_container").addClass("content-container");
				$("#" + relatedTablesArray[relLoop] + " div.query-tables-left").remove();
				$("#" + relatedTablesArray[relLoop] + " div.query-tables-right").remove();
				$("#" + relatedTablesArray[relLoop] + " div.query-tables-middle").remove();
				$("#" + relatedTablesArray[relLoop] + " div.query-tables-footer-left").remove();
				$("#" + relatedTablesArray[relLoop] + " div.query-tables-footer-right").remove();
				$("#" + relatedTablesArray[relLoop] + " div.query-tables-footer-middle").remove();
				$("#" + relatedTablesArray[relLoop] + " div#tbl_container").before('<div class="clearBoth query-tables-left"><div class="query-tables-right"><div class="query-tables-middle"></div></div></div>');
				$("#" + relatedTablesArray[relLoop] + " div#tbl_container").after('<div class="clearBoth query-tables-footer-left"><div class="query-tables-footer-right"><div class="query-tables-footer-middle"></div></div></div>');
				$(this).data('jdi', 'yes');
				if(relLoop<1){
					top = top-100;
				}else{
					top	= top+100;
				}

				$("#col_" + relatedTablesArray[relLoop]).css({"overflow-x":"hidden"});
				$("#col_" + relatedTablesArray[relLoop]).css({"height":"100px"});
				$("#col_" + relatedTablesArray[relLoop]).css({"width":"150px"});
				$("#col_" + relatedTablesArray[relLoop] + " ul>li").css({"list-style-type":"none"});
				$("#col_" + relatedTablesArray[relLoop] + " ul").css({"padding-left":"0"});
				
				$("#" + relatedTablesArray[relLoop]).css({"top":top+"px"});
				$("#" + relatedTablesArray[relLoop]).css({"left":left+"px"});
				
				relatedTableNames +=','+relatedTablesArray[relLoop].substring(4);
				relatedAliasTableNames +=','+relatedAliasTablesArray[relLoop].substring(4);
				//showing checkboxes only for the related tables of dragged table fields
				$("#"+ relatedTablesArray[relLoop] +" .report-table-column-check").css({'display':'block'});
				$("#"+ relatedTablesArray[relLoop] +" .table-content-container .table-accordion-content").css({'float':'left'});
				
			}
		}
		var currentTable = $(ui.draggable).attr("id").substring(4);
		var currentTableDisplay = $(ui.draggable).attr("value").substring(4);
		
		var listTable = '';
		var listTableDisplay = '';
		var getValues = $("#listTables").val();
		var getValuesDisplay = $("#listTablesDisplay").val();
		if(getValues != "") {
			var getValuesArray=getValues.split(",");
			if(jQuery.inArray(currentTable, getValuesArray) == '-1'){
				listTable = getValues + ',' + currentTable;
				listTableDisplay = getValuesDisplay + ',' + currentTableDisplay;
			} else {
				listTable = getValues;
				listTableDisplay = getValuesDisplay;
			}
		} else {
			listTable = currentTable;
			listTableDisplay = currentTableDisplay;
		}
		//alert(relatedTableNames);
		listTable	+= relatedTableNames;
		listTableDisplay += relatedAliasTableNames;
		
		$("#listTables").val(listTable);
		$("#listTablesDisplay").val(listTableDisplay);
		
		getPosition();
		//assign fields name, with its relation to 'Relcolumn' to draw relationship line
		$('#Relcoloumn').val($("#rel_fields_" + $(ui.draggable).attr("id")).val());
		//showing checkboxes only for dragged table fields
		$("#"+ $(ui.draggable).attr("id")+" .report-table-column-check").css({'display':'block'});
		$("#"+ $(ui.draggable).attr("id")+" .table-content-container .table-accordion-content").css({'float':'left'});
		firstDrag=false;
	},
	out: function(event, ui) {
		var relatedTablesArray= new Array();
		var relatedAliasTablesArray = new Array();
		
		var relatedTables		= $("#rel_" + $(ui.draggable).attr("id")).val();
		if(relatedTables!=''){
			relatedTablesArray	= relatedTables.split(",");
		}
		if(relatedTablesArray.length>0){
			if(jQuery.inArray($(ui.draggable).attr("id"),relatedTablesArray)==-1){
				for(var relLoop=0;relLoop<relatedTablesArray.length;relLoop++){
					$("#col_" + relatedTablesArray[relLoop]).hide();
					$("#" + relatedTablesArray[relLoop]).addClass("table-title-container");
					$("#col_" + relatedTablesArray[relLoop] + " .description").css({'display':'block'});
					$("#" + relatedTablesArray[relLoop] + " div#tbl_container").removeClass("content-container");
					$("#" + relatedTablesArray[relLoop] + " div.query-tables-left").remove();
					$("#" + relatedTablesArray[relLoop] + " div.query-tables-right").remove();
					$("#" + relatedTablesArray[relLoop] + " div.query-tables-middle").remove();
					$("#" + relatedTablesArray[relLoop] + " div.query-tables-footer-left").remove();
					$("#" + relatedTablesArray[relLoop] + " div.query-tables-footer-right").remove();
					$("#" + relatedTablesArray[relLoop] + " div.query-tables-footer-middle").remove();

					$(this).data('jdi', 'no')
				}
			}
		}
		$("#col_" + $(ui.draggable).attr("id")).hide();
		$("#" + $(ui.draggable).attr("id")).addClass("table-title-container");
		$("#col_" + $(ui.draggable).attr("id") + " .description").css({'display':'block'});
		$("#" + $(ui.draggable).attr("id") + " div#tbl_container").removeClass("content-container");
		$("#" + $(ui.draggable).attr("id") + " div.query-tables-left").remove();
		$("#" + $(ui.draggable).attr("id") + " div.query-tables-right").remove();
		$("#" + $(ui.draggable).attr("id") + " div.query-tables-middle").remove();
		$("#" + $(ui.draggable).attr("id") + " div.query-tables-footer-left").remove();
		$("#" + $(ui.draggable).attr("id") + " div.query-tables-footer-right").remove();
		$("#" + $(ui.draggable).attr("id") + " div.query-tables-footer-middle").remove();
		$(this).data('jdi', 'no');
		$("#selColoumn").val('');
			
	},
	deactivate: function(event, ui) {
		var relatedTablesArray= new Array();
		var relatedAliasTablesArray = new Array();
		

		if($(this).data('jdi') == "no") {
			$("#"+ $(ui.draggable).attr("id")+" .report-table-column-check").css({'display':'none'});
			$("#"+ $(ui.draggable).attr("id")+" .table-content-container .table-accordion-content").css({'float':'none'});
			$("#"+$(ui.draggable).attr("id") ).css({"position":"relative", "top":"0px", "left": "0px"});
			$("#col_" + $(ui.draggable).attr("id")).css({"overflow":"visible"});
			$("#col_" + $(ui.draggable).attr("id")).css({"height":"auto"});
			$("#col_" + $(ui.draggable).attr("id")).css({"width":"auto"});
			$("#col_" + $(ui.draggable).attr("id") + " ul>li").css({"list-style-type":"disc"});
			$("#col_" + $(ui.draggable).attr("id") + " ul").css({"padding-left":"2em"});
			
			
			removeRelation($(ui.draggable).attr("id"));
			var getValues 			= $("#listTables").val();
			var getValuesDisplay 	= $("#listTablesDisplay").val();
			var getValuesArray		= getValues.split(",");
			var getValuesDisplayArray	= getValuesDisplay.split(",");
			var currentTable 		= $(ui.draggable).attr("id").substring(4);
			var currentTableDisplay = $(ui.draggable).attr("value").substring(4);
			
			getValuesArray = jQuery.grep(getValuesArray, function(value) {
			    return value != currentTable;
			});
			var relatedTables		= $("#rel_" + $(ui.draggable).attr("id")).val();
			if(relatedTables!=''){
				relatedTablesArray	= relatedTables.split(",");
			}
			if(relatedTablesArray.length>0){
				if((jQuery.inArray($(ui.draggable).attr("id"),relatedTablesArray))==-1){
					for(var relLoop=0;relLoop<relatedTablesArray.length;relLoop++){
						$("#"+relatedTablesArray[relLoop] ).css({"position":"relative", "top":"0px", "left": "0px"});
						$("#"+ relatedTablesArray[relLoop] +" .report-table-column-check").css({'display':'none'});
						$("#"+ relatedTablesArray[relLoop] +" .table-content-container .table-accordion-content").css({'float':'none'});
						$("#col_" + relatedTablesArray[relLoop]).css({"overflow":"visible"});
						$("#col_" + relatedTablesArray[relLoop]).css({"height":"auto"});
						$("#col_" + relatedTablesArray[relLoop]).css({"width":"auto"});
						$("#col_" + relatedTablesArray[relLoop] + " ul>li").css({"list-style-type":"disc"});
						$("#col_" + relatedTablesArray[relLoop] + " ul").css({"padding-left":"2em"});
						
					}
					getValuesArray = jQuery.grep(getValuesArray, function(value) {
					    if(jQuery.inArray(value,relatedTablesArray)==-1){
					    	return;
					    }
					});
				}
			}
			var listTable = getValuesArray.join(",");
			$("#listTables").val(listTable);
			
			getValuesDisplayArray = jQuery.grep(getValuesDisplayArray, function(value) {
			    return value != currentTableDisplay;
			});
			var relatedAliasTables		= $("#rel_alias_" + $(ui.draggable).attr("id")).val();
			if(relatedAliasTables!=''){
				relatedAliasTablesArray	= relatedAliasTables.split(",");
			}
			if(relatedTablesArray.length>0){
				if(jQuery.inArray($(ui.draggable).attr("id"),relatedTablesArray)==-1){
					getValuesDisplayArray = jQuery.grep(getValuesDisplayArray, function(value) {
					    if(jQuery.inArray(value,relatedAliasTablesArray)==-1){
					    	return;
					    }
					});
				}
			}
			var listTableDisplay = getValuesDisplayArray.join(",");
			$("#listTablesDisplay").val(listTableDisplay);
			
			getPosition();
			$(this).data('jdi', '');
		} else {
			reSetRelation($(ui.draggable).attr("id"));
		} 
	}
});
}catch(e){
		//Nothing to do
}
};

//get the position for edit purpose

function getPosition() {
	try{
	if($("#listTables").val() != '') {
		var arrListTables= ($("#listTables").val()).split(",")
		var tableOffset = '';
		var tblPosition = '';
		var adstop = '';
		var posstyle = '';
		$.each(arrListTables, function(index, value) { 
			tableOffset = $("#tbl_"+value).offset();
			//adstop = tableOffset.top + $("div#report-add-wizard").scrollTop();
			posstyle = $("#tbl_"+value).attr('style')
			tblPosition += '{"dbtable":"'+value+'", "style":"'+posstyle+'"}';
			if((arrListTables.length-1) != index)
			tblPosition += ', ';
		});
		$("#tbl_position").val(tblPosition);
		} else {
			$("#tbl_position").val('');
		}
	}catch(e){
 		//Nothing to do
	}
}

//set relationship	

function setRelation(id) {
try{	
var selCol = $("#selColoumn").val();
if(selCol != '') {
	var joinTables = (selCol+','+id).split(",");
	joinTables.sort();
	var Col1 = joinTables[0].split(".");
	var Col2 = joinTables[1].split(".");
	if(selCol != '' && Col1[0] != Col2[0]) {
		relCol =  joinTables[0] +"="+ joinTables[1];
		relCols = $("#Relcoloumn").val();
		if(relCols != '') {
			var getRelCols=relCols.split(",");
			if(jQuery.inArray(relCol, getRelCols) == '-1')
				var relCol = relCols + ',' + relCol;
			else
				var relCol = relCols;	
		} 
		
	    drawline1(joinTables[0], joinTables[1]);
		$("#Relcoloumn").val(relCol);
		$("#selColoumn").val('');
		return false
	} else if(Col1[0] == Col2[0]) {

	}
}
$("#selColoumn").val(id)
}catch(e){
		//Nothing to do
}
}

//reset all relationship line in drop within box

function reSetRelation(id) {
try{
colIds = '';
var arrColIds = new Array();
var k = 0;
$('#col_'+id).find("span").each(function() {
   	arrColIds[k] = $(this).attr('id');
	k++;
});
relCols = $("#Relcoloumn").val();
var arrRelCols = relCols.split(",");
if(relCols != "") {
	var listvalues = '';
	var arrNewRelCols = new Array();
	var splitRemCols = '';
	var source = '';
	var target = '';
	for (j=0,j1=0;j<arrRelCols.length;j++) {
		for (i=0;i<arrColIds.length;i++)  {	
			if(arrColIds[i] != "") {
				if(jQuery.inArray(arrColIds[i], arrRelCols[j].split("=")) > '-1') {
					arrNewRelCols[j1] = arrRelCols[j];
					//clear line when drop out from box
					splitRemCols = arrRelCols[j].split("=");
	        		var relIds = (arrRelCols[j]);
	        		clearLine(relIds,'reset');
	        		//end line when drop out from box
	        		//redraw the line when drop in box
	        		var drawRelTableCol1 = splitRemCols[0].split(".");
	        		var drawRelTableCol2 = splitRemCols[1].split(".");
	        		var drawRelTable1 = drawRelTableCol1[0];
	        		var drawRelTable2 = drawRelTableCol2[0];
	        		drawline1(drawRelTable1, drawRelTable2);
	        		//end redraw the line when drop in box
	        		j1++;
				}
			}
		}
	}
	}	
}catch(e){
		//Nothing to do
}
}

//remove all relationship line and relationship when drop out 

function removeRelation(id) {
try{
colIds = '';
var arrColIds = new Array();
var k = 0;
$('#col_'+id).find("span").each(function() {
	arrColIds[k] = $(this).attr('id');
	k++;
	//remove the coloumn in layout section
	$('#remcolumnlabel'+jqSelector($(this).attr('id'))).remove();
});
relCols = $("#Relcoloumn").val();
var arrRelCols = relCols.split(",")
if(relCols != "") {
	var listvalues = '';
	var arrNewRelCols = new Array();
	var splitRemCols = '';
	var source = '';
	var target = '';
	for (j=0,j1=0;j<arrRelCols.length;j++)  {
		for (i=0;i<arrColIds.length;i++)  {	
			if(arrColIds[i] != "") {
				if(jQuery.inArray(arrColIds[i], arrRelCols[j].split("=")) > '-1') {
					arrNewRelCols[j1] = arrRelCols[j];
					//clear line when drop out from box
					var relIds = (arrRelCols[j]);
	        		clearLine(relIds);
	        		//end line when drop out from box
					j1++;
				}
			}
		}
	}
	var arrNewRemoveRelCols = $.grep(arrRelCols, function(n, i){
		  return $.inArray(n, arrNewRelCols) == -1;
		});
	$("#Relcoloumn").val(arrNewRemoveRelCols.join(','));
	
}	
}catch(e){
		//Nothing to do
}
}

// Clear the relationship line	
function clearLine(classname, from)  {
try{	
	relCols = $("#Relcoloumn").val();
	if(relCols != "" && from == "single") {
		var arrRelCols = relCols.split(",")
		arrRelCols = jQuery.grep(arrRelCols, function(value) {
						    return value != classname;
						});
		$("#Relcoloumn").val(arrRelCols.join(','));
	}
	splitRemCols = classname.split("=");
	var drawRelTableCol1 = splitRemCols[0].split(".");
	var drawRelTableCol2 = splitRemCols[1].split(".");
	var drawRelTable1 = drawRelTableCol1[0];
	var drawRelTable2 = drawRelTableCol2[0];
	$('.'+jqSelector(drawRelTable1+"="+drawRelTable2)).remove();
}catch(e){
		//Nothing to do
}
}

// Escapes special characters and returns a valid jQuery selector
function jqSelector(str){
	try{
	return str.replace(/([;&,\.\+\*\~':"\!\^#$%@\[\]\(\)=>\|])/g, '\\$1');
	}catch(e){
 		//Nothing to do
	}
}

function drawline1(source, target){
try{
	var jg = new jsGraphics("Canvas"); // Use the "Canvas" div for drawing
	popUpOffSet = $("#report-add-wizard").offset();
	
	var relIds = (source+'='+target);

	jg.setStroke(2);
	jg.setColor('red');
	jg.setDivId(relIds);
	sourceOffset = $("#line_"+source).offset();
	targetOffset = $("#line_"+target).offset();

	//jg.drawLine((sourceOffset.left-80),(sourceOffset.top-popUpOffSet.top)+75,(targetOffset.left-popUpOffSet.left),(targetOffset.top-popUpOffSet.top)+75);
	jg.drawLine((sourceOffset.left),(sourceOffset.top-popUpOffSet.top),(targetOffset.left-popUpOffSet.left),(targetOffset.top-popUpOffSet.top));
	jg.paint();
}catch(e){
		//Nothing to do
}
}




function setDragDropReportBuilder() {
	try{
	$("#sortable").sortable();
    $("#toolLabel").draggable({
    	revert: true
    });

    if($("body").data("reportsearch").headerArray == '') {
    	$("body").data("reportsearch").headerArray = [];
    } 
    
    $("#tf_dropHeaderBox").droppable({
        accept: "#toolLabel",
        drop: function(event, ui) {
            var item = ui.draggable.html();
            var itemid = ui.draggable.attr("id");

            if(jQuery.inArray(itemid, $("body").data("reportsearch").headerArray) == '-1') {

            	var html = '<div class="headerlabel" id="headerlabel--'+itemid+'" onclick="labelNameChange(\'headerlabel--'+itemid+'\')">';
            	html = html + ''+item+'</div>';

            	html = html +'<div class="headerlabel" style="display:none;" id="divId--headerlabel--'+itemid+'">';
            	html = html + '<input type="text" id="textId--headerlabel--'+itemid+'" name="textId--headerlabel--'+itemid+'" onblur="labelNameReset(\'headerlabel--'+itemid+'\', \'headerArray\')" /></div>';
            	
           		$("#tf_dropHeaderBox").append(html);
           		
           		$("body").data("reportsearch").headerArray.push(itemid);
            } else {
            	alert("Already label added in header section");
            }
        },
			out: function(event, ui) {
		}
    });

    if($("body").data("reportsearch").footerArray == '') {
    	$("body").data("reportsearch").footerArray = [];
    } 

    $("#tf_dropFooBox").droppable({
          accept: "#toolLabel",
          drop: function(event, ui) {
              var item = ui.draggable.html();
              var itemid = ui.draggable.attr("id");

              if(jQuery.inArray(itemid, $("body").data("reportsearch").footerArray) == '-1') {
                  
              	var html = '<div class="footerlabel" id="footerlabel--'+itemid+'" onclick="labelNameChange(\'footerlabel--'+itemid+'\')">';
              	html = html + ''+item+'</div>';

              	html = html +'<div class="footerlabel" style="display:none;" id="divId--footerlabel--'+itemid+'">';
              	html = html + '<input type="text" id="textId--footerlabel--'+itemid+'" name="textId--footerlabel--'+itemid+'" onblur="labelNameReset(\'footerlabel--'+itemid+'\', \'footerArray\')" /></div>';
              	
             		$("#tf_dropFooBox").append(html);

             	$("body").data("reportsearch").footerArray.push(itemid);
              } else {
            	  alert("Already label added in footer section");
              }
              
          },
			out: function(event, ui) {
  		}
      });   

    if($("body").data("reportsearch").criteriaArray == '') {
    	$("body").data("reportsearch").criteriaArray = [];
    } 

    $("#tf_dropCriteriaBox").droppable({
          accept: "#toolLabel",
          drop: function(event, ui) {
              var item = ui.draggable.html();
              var itemid = ui.draggable.attr("id");

              if(jQuery.inArray(itemid, $("body").data("reportsearch").criteriaArray) == '-1') {

              	var html = '<div class="criterialabel" id="criterialabel--'+itemid+'" onclick="labelNameChange(\'criterialabel--'+itemid+'\')">';
              	html = html + ''+item+'</div>';

              	html = html +'<div class="criterialabel" style="display:none;" id="divId--criterialabel--'+itemid+'">';
              	html = html + '<input type="text" id="textId--criterialabel--'+itemid+'" name="textId--criterialabel--'+itemid+'" onblur="labelNameReset(\'criterialabel--'+itemid+'\', \'criteriaArray\')" /></div>';
              	
             		$("#tf_dropCriteriaBox").append(html);
             		
             		$("body").data("reportsearch").criteriaArray.push(itemid);
              } else {
            	  alert("Already label added in criteria section");
              }
              
          },
			out: function(event, ui) {

  		}
      }); 

    $(".feildsdrag").draggable({
		revert: true
	});

    if($("body").data("reportsearch").fieldsArray == '') {
    	$("body").data("reportsearch").fieldsArray = [];
    } 

    $("#tf_dropColBox").droppable({
          accept: ".feildsdrag",
          drop: function(event, ui) {
              var item = ui.draggable.html();
              var itemid = ui.draggable.attr("id");

              if(jQuery.inArray(itemid, $("body").data("reportsearch").fieldsArray) == '-1') {
              
              	var html = '<div class="columnlabel" title="'+itemid+'" id="columnlabel--'+itemid+'" onclick="labelNameChange(\'columnlabel--'+itemid+'\')">';
              	html = html + ''+item+'</div>';

              	html = html +'<div class="columnlabel" style="display:none;" id="divId--columnlabel--'+itemid+'">';
              	html = html + '<input type="text" id="textId--columnlabel--'+itemid+'" name="textId--columnlabel--'+itemid+'" onblur="labelNameReset(\'columnlabel--'+itemid+'\', \'fieldsArray\')" /></div>';
              	
              	html = '<li id="remcolumnlabel'+itemid+'">'+ html +'</li>';

              	html = $("#sortable").append(html);

         		$("#tf_dropColBox").append(html);	
         		$("#sortable").sortable();
         		
         		$("body").data("reportsearch").fieldsArray.push(itemid);
              } else {
                  alert("Already report column added in report section");
              }
             		
        },
		out: function(event, ui) {

  		}
      }); 
	}catch(e){
 		//Nothing to do
	}
}

function labelNameChange(id) {
	try{
	var id = jqSelector(id)
	$("#divId--" + id).show();
	$("#" + id).hide();
	$("#textId--" + id).val($("#" + id).html());
	$("#textId--" + id).focus();
	}catch(e){
 		//Nothing to do
	}
}

function labelNameReset(id, from) {
	try{
	var arrlistCol = id.split("--");
	var id = jqSelector(id)
	$("#divId--" + id).hide();
	$("#" + id).show();
	$("#" + id).html($("#textId--" + id).val());
	
	
	if($("#textId--" + id).val() == '') {
		$("#"+id).remove();
		$("#divId--"+id).remove();
		if(from == "fieldsArray") {
			$("body").data("reportsearch").fieldsArray = jQuery.grep($("body").data("reportsearch").fieldsArray, function(value) {
		    	return value != arrlistCol[1];	
			});
		}
		if(from == "criteriaArray") {
			$("body").data("reportsearch").criteriaArray = jQuery.grep($("body").data("reportsearch").criteriaArray, function(value) {
		    	return value != arrlistCol[1];	
			});
		}
		if(from == "footerArray") {
			$("body").data("reportsearch").footerArray = jQuery.grep($("body").data("reportsearch").footerArray, function(value) {
		    	return value != arrlistCol[1];	
			});
		}
		if(from == "headerArray") {
			$("body").data("reportsearch").headerArray = jQuery.grep($("body").data("reportsearch").headerArray, function(value) {
		    	return value != arrlistCol[1];	
			});
		}
	}
	}catch(e){
 		//Nothing to do
	}
}

function displaySection(from, to) {
	try{
	$('#show_expertus_message').html('');
	$('#show_expertus_message').hide();
	if(to == "report-data-source")
		$("#Canvas").show();
	else 
		$("#Canvas").hide();
	if(to == "report-builder-section") {
		$("#accordion").accordion("destroy");
		var listTable = $("#listTables").val();
		var listTableDisplay = $("#listTablesDisplay").val();
		if(listTable != "") {
			var arrlistTable = listTable.split(",");
			var arrlistTableDisplay = listTableDisplay.split(",");
			
			var html = '';
			for (j=0;j<arrlistTable.length;j++)  {
				html+='<h4><a href="javascript:void(0)" class="tableTitle" id="tbl_'+arrlistTable[j]+'">'+arrlistTableDisplay[j]+'</a></h4>'
				html+='<div class="clsReportTables">';
				html+='<span id="col_tbl_'+arrlistTable[j]+'">';
				$('#col_tbl_'+arrlistTable[j]).find("span").each(function() {
					html+='<span class="feildsdrag" id="'+$(this).attr('id')+'">'+$("#"+jqSelector($(this).attr('id'))).html()+'</span>';
				});
				html+='</span>';
				html+='</div>';
			}
			
			$("#accordion").html(html);
			setDragDropReportBuilder();
			$("#accordion").accordion({active: false, collapsible: true});
			$("#accordion, .clsReportTables").css({"height":"auto"});

		}
	}
	
	$("."+from).hide();
	$("#"+to).show();
	
	if(to=='report-criteria') 
		$('body').data('reportsearch').paintReportTableColsAutocomplete();
	 if(to=='report-data-source'){
	    paginationNextPrev();
		
	}

	$("[id*='_breadcrump']").removeClass("reports-title-div-selected").addClass("reports-title-div");
	$("#"+to+"_breadcrump").removeClass("reports-title-div").addClass("reports-title-div-selected");
	
	var listTable = $("#listTables").val();
	if(listTable != "") {
		var arrlistTable = listTable.split(",");
		for (j=0;j<arrlistTable.length;j++) {
			$("#tbl_" + arrlistTable[j]).removeClass("table-title-container");
			$("#tbl_" + arrlistTable[j]).css({"position":"absolute"});
			$("#tbl_" + arrlistTable[j] + " span#add_icon").addClass("add-icon");
			$("#tbl_" + arrlistTable[j] + " div#tbl_container").addClass("content-container");
			$("#tbl_" + arrlistTable[j] + " div#tbl_container").before('<div class="clearBoth query-tables-left"><div class="query-tables-right"><div class="query-tables-middle"></div></div></div>');
			$("#tbl_" + arrlistTable[j] + " div#tbl_container").after('<div class="clearBoth query-tables-footer-left"><div class="query-tables-footer-right"><div class="query-tables-footer-middle"></div></div></div>');
		}
		reSetRelation('tbl_'+arrlistTable[0]);
	}
	
	// start default header info display.
	if($("body").data("reportsearch").sectionpop == "add") {
		var headerlabel = '';
		headerlabel = '<div onclick="labelNameChange(\'headerlabel--toolLabel\')" id="headerlabel--toolLabel" class="headerlabel" style="display: inline;">'+$("#set_mainquerform_ReportTitle").val()+'</div>'
		headerlabel +='<div id="divId--headerlabel--toolLabel" style="display: none;" class="headerlabel"><input type="text" onblur="labelNameReset(\'headerlabel--toolLabel\', \'headerArray\')" name="textId--headerlabel--toolLabel" id="textId--headerlabel--toolLabel" value="'+$("#set_mainquerform_ReportTitle").val()+'"></div>';
		$("#tf_dropHeaderBox").html(headerlabel);
		$("body").data("reportsearch").headerArray = [];
		$("body").data("reportsearch").headerArray.push('toolLabel');
	}
	// end default header info display.
	}catch(e){
 		//Nothing to do
	}
}
function toggleDisplay(id,parentId) {
	try{
//		40058: In Report design wizard in the tables unable to expand the tables when clicking expand and collapse icon continuously
		$("#" + id).stop(true, true).slideToggle('fast', function() {
			if ($("#" + id).is(':hidden')){
				if(Drupal.settings.ajaxPageState.theme == "expertusoneV2") {
					$('#'+parentId+' .add-icon').css("background", "url(/sites/all/themes/core/expertusoneV2/expertusone-internals/images/ExpertusIcons_v2.png) no-repeat -122px -813px");
				}else{
					$('#'+parentId+' .add-icon').css("background", "url(/sites/all/themes/core/expertusone/expertusone-internals/images/ExpertusIcons.png) no-repeat -122px -813px");	 
				}
			}else{
				if(Drupal.settings.ajaxPageState.theme == "expertusoneV2") {
					$('#'+parentId+' .add-icon').css("background", "url(/sites/all/themes/core/expertusoneV2/expertusone-internals/images/ExpertusIcons_v2.png) no-repeat -122px -805px");
				}else{
					$('#'+parentId+' .add-icon').css("background", "url(/sites/all/themes/core/expertusone/expertusone-internals/images/ExpertusIcons.png) no-repeat -122px -805px");	
				}
			}
			$("body").data("reportsearch").setJsScrollpane('tableRelated','scroll-pane-report-related-tables-'+parentId);
			$("body").data("reportsearch").setJsScrollpane('scroll-pane');
		});
	}catch(e){
 		//Nothing to do
	}
}
function validateReport(wizardTitle){
	try{
	if(wizardTitle=='report-query-details'){
		validateReportQueryDetails();
	}else if(wizardTitle=='report-data-source'){
		validateReportQueryBuilder();
	}
	}catch(e){
 		//Nothing to do
	}
}
function validateReportQueryDetails(){
	try{
	var error = false;
	var errmsg = Drupal.t('ERR174');
	if($('#set_mainquerform_ReportTitle').val()=='')
		error=true;
	else if($('#set_mainquerform_status').val()=='0')
		error=true;
	else if($('#set_mainquerform_category').val()=='0')
		error=true;
	if(error==true){
		var message_call = expertus_error_message(errmsg,'error');
		$('#show_expertus_message').show();
		$('#show_expertus_message').html(message_call);
		return false;
	}else{
		$('#show_expertus_message').hide();
		displaySection('maintabs','report-data-source');
	}
	}catch(e){
 		//Nothing to do
	}
}
function showTablePageDiv(totalPages,currentPage){
	try{
	for(i=1;i<=totalPages;i++){
		$('#report_table_page_'+i).hide();
		$("#report-table-page-no-"+i).removeClass("report-table-page-active-cls").addClass("report-table-page-cls");
	}
	$('#report_table_page_'+currentPage).show();
	$("#report-table-page-no-"+currentPage).removeClass("report-table-page-cls").addClass("report-table-page-active-cls");
	}catch(e){
 		//Nothing to do
	}
}
function getSelectedColumns(){
	try{
	var selectedValues = $("#query_builder_tagsDrop input:checked[name=check_report_columns]").map(function() {
		return $(this).val();
	}).get().join(",");

	return selectedValues;
	}catch(e){
 		//Nothing to do
	}
}
function getSelectedColumnsMap(){
	try{
	var selectedValues = $("#query_builder_tagsDrop input:checked[name=check_report_columns]").map(function() {
		return $(this).attr('data');
	}).get().join(",");

	return selectedValues;
	}catch(e){
 		//Nothing to do
	}
}
function getSelectedAliasColumns(){
	try{
	var selectedColumns	= getSelectedColumns().split(",");
	var selectedAliasColumns='';
	if(selectedColumns!=''){
		selectedAliasColumns = $.map(selectedColumns, function(val,index) {
			return $("#"+jqSelector(val)).attr('title');
		}).join(",");
	}
	return selectedAliasColumns;
	}catch(e){
 		//Nothing to do
	}
}
function remove(el) {
	try{
	$("#"+el).css('visibility','visible');
    setTimeout(function() { $('.icart').remove(); }, 300);
	}catch(e){
 		//Nothing to do
	}
}
$.fn.getTextWidth = function(text, style) {
    if (!$.fn.getTextWidth.fakeEl) $.fn.getTextWidth.fakeEl = $('<span>').appendTo(document.body);
    $.fn.getTextWidth.fakeEl.text(text || this.val() || this.text()).attr('style', style || this.attr('style'));
    return $.fn.getTextWidth.fakeEl.hide().width();
};