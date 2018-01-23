(function($) {
	
$.widget("ui.lnrreportssearch", {
	_init: function() {
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var searchStr = this.searchActionCheck();
		var pageUrl = window.location.search;
		if (pageUrl != '?q=admincalendar') // disable search results on adm calendar
			this.renderSearchResults(searchStr);
		}catch(e){
			// to do
		}
	},

	searchActionParameters: function(sortbytxt){
		try{
		var searchStr = '';

		/*-------Report Status-------*/
		var report_status = '';
		$('.reports-status').each(function(){
			if($(this).is(':checked')){
				report_status += report_status!=''?'|':'';
				report_status += $(this).val();
			}
		});

		/*-------Report Roles-------*/
		var report_roles = '';
		$('.reports-roles').each(function(){
			if($(this).is(':checked')){
				report_roles += report_roles!=''?'|':'';
				report_roles += $(this).val();
			}
		});
		/*-------Report Manage-------*/
		var report_manage = '';
		$('.reports-manage').each(function(){
			if($(this).is(':checked')){
				report_manage += report_manage!=''?'|':'';
				report_manage += $(this).val();
			}
		});
		/*-------Report Types-------*/
		var report_types = '';
		$('.reports-types').each(function(){
			if($(this).is(':checked')){
				report_types += report_types!=''?'|':'';
				report_types += $(this).val();
			}
		});

		/*-------Title-------*/
		var title 	  = $('#reports_searchtext').val();
			if((title.toLowerCase()) == (Drupal.t('LBL304').toLowerCase()))
				title='';

		/*-------Location-------*/
		var report_name 	= $('#srch_criteria_report_title').val();
		if(report_name == 'Type a report name' || report_name == undefined)
			report_name = '';
		else
			$('#location-clr').css('display','block');

		/*-------Sort By-------*/
		if (sortbytxt!=null && sortbytxt!=undefined) this.sortbyValue = sortbytxt;
		var sortby = (typeof this.sortbyValue == 'undefined')? '' : this.sortbyValue;

		searchStr	= '&title='+encodeURIComponent(title)+'&report_status='+report_status+'&report_manage='+report_manage+'&report_roles='+report_roles+'&report_types='+report_types+'&report_name='+report_name+'&sortby='+sortby;

		return searchStr;
		}catch(e){
			// to do
		}
	},

	searchAction : function(sortbytxt,className) {
		try{
		var searchStr = this.searchActionParameters(sortbytxt);

    	this.createLoader('lnr-reports-search');

		$('#paintLearnerReportsResults').setGridParam({url: this.constructUrl('learning/report-search/all/none'+searchStr)});
	    $("#paintLearnerReportsResults").trigger("reloadGrid",[{page:1}]);

	    this.checkboxValidation();
	    var ulClsName = 'sort-by-links';
	    if(this.currTheme == "expertusoneV2"){
	    	ulClsName = 'narrow-search-sortbar-sortlinks';
	    }
	    //Highlight sort type VJ
	    if(className!= null) {
			$('.'+ulClsName +' li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
			});
			$('.'+className).addClass('sortype-high-lighter');
	    }
	    else {
			$('.'+ulClsName +' li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
			});

			if(this.sortbyValue!=''){
				if(this.sortbyValue == 'ZA')
					$('.type2').addClass('sortype-high-lighter');
				else if(this.sortbyValue == 'Time')
					$('.type3').addClass('sortype-high-lighter');
				else if(this.sortbyValue == 'ClassStartDate')
					$('.type4').addClass('sortype-high-lighter');
				else if(this.sortbyValue == 'Mandatory')
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

	renderSearchResults : function(searchStr){
		try{
		this.createLoader('lnr-reports-search');
		var obj = this;
		var urlStr = (searchStr != '') ? searchStr : 'none';
		var objStr = '$("#lnr-reports-search").data("lnrreportssearch")';
		if(this.currTheme == "expertusoneV2"){
			var gridWidth 		= 720;
			var detailsWidth 	= 520;
			var actionWidth 	= 150;
			var iconsWidth 		= 74;
		}else{
			var gridWidth 		= 771;
			var detailsWidth 	= 625;
			var actionWidth 	= 110;
			var iconsWidth 		= 54;
		}
		$("#paintLearnerReportsResults").jqGrid({
			url:obj.constructUrl("learning/report-search/all/none"+urlStr),
			datatype: "json",
			mtype: 'GET',
			colNames:['Details','Action'],
			colModel:[ {name:'Details',index:'Details', title:false, width:detailsWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResults},
			           {name:'Action',index:'Action', title:false, width:actionWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResultsAction}],
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
			loadComplete:obj.callbackLoader
		});
		// .navGrid('#pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});

		/* To highlight the default sort order - added by Rajkumar U*/
		var ulClsName = 'sort-by-links';
	    if(this.currTheme == "expertusoneV2"){
	    	ulClsName = 'narrow-search-sortbar-sortlinks';
	    }
		$('.'+ulClsName +' li').each(function() {
			$(this).find('a').removeClass('sortype-high-lighter');
		});

		if(this.sortbyValue!=''){
			if(this.sortbyValue == 'ZA')
				$('.type2').addClass('sortype-high-lighter');
			else if(this.sortbyValue == 'Time')
				$('.type3').addClass('sortype-high-lighter');
			else if(this.sortbyValue == 'ClassStartDate')
				$('.type4').addClass('sortype-high-lighter');
			else if(this.sortbyValue == 'Mandatory')
				$('.type5').addClass('sortype-high-lighter');
			else
				$('.type1').addClass('sortype-high-lighter');
		} else {
			$('.type1').addClass('sortype-high-lighter');
		}
		}catch(e){
			// to do
		}
	},

	callbackLoader : function(response, postdata, formid, updateShowMore)
	{
		try{
		$('#paintLearnerReportsResults').show();
		var recs = parseInt($("#paintLearnerReportsResults").getGridParam("records"),10);
        if (recs == 0) {
        	 $('#reports-no-records'). css('display','block');
            var html = Drupal.t("MSG381");
            $("#reports-no-records").html(html);
        } else {
        	$('#reports-no-records'). css('display','none');
        	$("#reports-no-records").html("");
        }
       /* if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
    	   if(response.total > 1 ) {
    		   // $('#pager').show();
    		   	$('#pg_pager #pager_center #first_pager').show();
   				$('#pg_pager #pager_center #last_pager').show();
    			$('#pg_pager #pager_center #next_pager').show();
    			$('#pg_pager #pager_center #prev_pager').show();
    			$('#pg_pager #pager_center #sp_1_pager').parent().show();
    	   }
    	   else{
    		   //$('#pager').hide();
    		   	$('#pg_pager #pager_center #first_pager').hide();
   				$('#pg_pager #pager_center #last_pager').hide();
    			$('#pg_pager #pager_center #next_pager').hide();
    			$('#pg_pager #pager_center #prev_pager').hide();
    			$('#pg_pager #pager_center #sp_1_pager').parent().hide();
    			$('.narrow-search-results table').find('tr:last-child td').css("border-bottom","none");
    	   }
    	   if (recs < 11) {
    		   $('#report-search-footer').show();
    		   $('#pager').hide();
    		   $('.narrow-search-results table').find('tr:last-child td').css("border-bottom","none");
    	   }
    	   else{
    		   $('#report-search-footer').hide();
    		   $('#pager').show();
    	   }
       }
       else {
	  	   if (response.total > 1 ) {
	  		   // $('#pager').css('display','block');
			     //$('#pg_pager').css('display','block');
           $('#pg_pager #pager_center #first_pager').show();
           $('#pg_pager #pager_center #last_pager').show();
           $('#pg_pager #pager_center #next_pager').show();
           $('#pg_pager #pager_center #prev_pager').show();
           $('#pg_pager #pager_center #sp_1_pager').parent().show();
		     }
		     else {
			     // $('#pager').css('display','none');
			     //$('#pg_pager').css('display','none');
           $('#pg_pager #pager_center #first_pager').hide();
           $('#pg_pager #pager_center #last_pager').hide();
           $('#pg_pager #pager_center #next_pager').hide();
           $('#pg_pager #pager_center #prev_pager').hide();
           $('#pg_pager #pager_center #sp_1_pager').parent().hide();
         }
         if (recs < 11) {
           $('#report-search-footer').show();
           $('#pager').hide();
           $('#paintLearnerReportsResults tr:not(.search-register-btn tr):last-child > td').css("border-bottom", "none");
  		   }
  	  	 else {
           $('#report-search-footer').hide();
  	  		 $('#pager').show();
           $('#paintLearnerReportsResults tr:not(.search-register-btn tr):last-child > td ').css("border-bottom", "1px dotted #CCCCCC");
	  	   }
       } */

        Drupal.attachBehaviors();

        var obj = $("#lnr-reports-search").data("lnrreportssearch");

        $("#lnr-reports-search").data("lnrreportssearch").destroyLoader('lnr-reports-search');
        $("#lnr-reports-search").data("lnrreportssearch").destroyLoader('searchReportsRecordsPaint');


	   if($("#lnr-reports-search").data("lnrreportssearch").defaults.start) {
		    $("#search-filter-content").html(response.filter);
		    $("#find-trng-sort-display").show();
		   // $("#pager").show();
		    $("#lnr-reports-search").data("lnrreportssearch").defaults.start = false;
		    // $('.narrow-search-results table').find('tr:last-child td').css("border-bottom","none");
		    /*
		    * if search text from home page through search autocomplete inputbox then Catalog page Type checkboxes should be unchecked.
		    * same if click CATALOG link then checkboxes should be checked based on results (By default delivery type results only will be displayed).
		    */
		    var req=window.location.search;
			req=req.substring(3,req.length);
			var reqArr=req.split("/");
			if(reqArr.length<=2){
				$("#lnr-reports-search").data("lnrreportssearch").checkBoxSelected(response);
			}
			/*----*/
			if ($('#group_list').val() > 4) {
				$('#paintGroupContent').css('height', '90px');
				$('#paintGroupContent').jScrollPane({});
			}
	    } else {
		   $("#lnr-reports-search").data("lnrreportssearch").unselectFilter(response);

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
		vtip();
	  // $(".ui-jqgrid-bdiv").css("overflow","hidden");
	    $('.limit-title').trunk8(trunk8.report_title);
		$('.limit-desc').trunk8(trunk8.report_desc);
			updateShowMore = ((updateShowMore == undefined || updateShowMore == null) ? true : updateShowMore);
			if(updateShowMore) {
				$("#paintLearnerReportsResults").showmore({
					showAlways: true,
					'grid': "#paintLearnerReportsResults",
					'gridWrapper': '#paint-narrow-search-results',
					'showMore': '#admin-reports-show-more'
				});
			}
		}catch(e){
			// to do
		}
	},

	deleteReport : function(reportId){
		try{
		// Remove the delete message wizard
		$('#delete-msg-wizard').remove();

		var obj = this;
		this.createLoader('lnr-reports-search');

		url = obj.constructUrl("administration/report-search/delete/"+reportId);
		$.ajax({
			type: "POST",
			url: url,
			success: function(result){
			/*committed for 0030567*/
				//$('#paintLearnerReportsResults').setGridParam({url: obj.constructUrl('learning/report-search/all/none')});
				$("#paintLearnerReportsResults").trigger("reloadGrid",[{current:true}]);				
			}
		});
		}catch(e){
			// to do
		}
	},

	publishUnpublishReport : function(type, reportId){
		try{
		var obj = this;
		this.createLoader('lnr-reports-search');
		url = obj.constructUrl("administration/report-search/publish-unpublish/"+type+"/"+reportId);
		$.ajax({
			type: "POST",
			url: url,
			success: function(result){
				if (typeof $("#lnr-reports-search").data("lnrreportssearch").refreshLastAccessedReportResult != 'undefined' && $("#lnr-reports-search").data("lnrreportssearch").refreshLastAccessedReportResult() == false) {
					$("#paintLearnerReportsResults").trigger("reloadGrid",[{current:true}]);
				}
			}
		});
		}catch(e){
			// to do
		}
	},

	printSearchResults : function() {
		try{
		var sortbytxt = null;
		var searchStr = this.searchActionParameters(sortbytxt);
		var printPath = 'learning/report-search/print';
		window.location = this.constructUrl(printPath + '&' + searchStr);
		}catch(e){
			// to do
		}
	},

	exportSearchResults : function() {
		try{
		var sortbytxt = null;
		var searchStr = this.searchActionParameters(sortbytxt);
		var exportPath = 'learning/report-search/export';
		window.location = this.constructUrl(exportPath + '&' + searchStr);
		}catch(e){
			// to do
		}
	},
	moreListDisplay : function(recLen,dispId) {
		for(i=0;i<=recLen;i++){
			$('#'+dispId+'_'+i).css("display","block");
		}
		$('#'+dispId+'_more').css("display","none");
		$('#'+dispId+'_short').css("display","block");
	},
	shortListDisplay : function(recLen,dispId){
		for(i=0;i<=recLen;i++){
			if(i<=4){
				$('#'+dispId+'_'+i).css("display","block");
				}
			else {
				$('#'+dispId+'_'+i).css("display","none");
				}
			}
		$('#'+dispId+'_short').css("display","none");
		$('#'+dispId+'_more').css("display","block");
	},

	displayDeleteWizard : function(title,objectId,objectType,wSize){
		try{
		var wSize = (wSize) ? wSize : 300;
		title = decodeURIComponent(title);
	    $('#delete-msg-wizard').remove();
	    var html = '';
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';

	    html+= '<tr><td class="delete-msg-txtcolor" color="#333333" style="text-align: justify;"><span class="vtip" style="width:244px;text-align:justify;overflow: hidden;word-break: break-all;display:block;">'+title+''+'?'+'</span></td></tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

	    if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
	    	var esignObj = {'popupDiv':'delete-object-dialog','esignFor':'DeleteAdminObject','objectId':objectId,'objectType':objectType};
	    	closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj);  };
	    }else{
	    	closeButt[Drupal.t('Yes')]=function(){ $('#lnr-reports-search').data('lnrreportssearch').deleteReport(objectId); };
	    }

	    $("#delete-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:wSize,
	        resizable:false,
	        modal: true,
	        title:Drupal.t("LBL286"),//"title",
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:false,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });

	    $('.ui-dialog').wrap("<div id='delete-object-dialog'></div>");

	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
	    if(this.currTheme == "expertusoneV2"){
	    	changeDialogPopUI();
	    	$('.expertusV2PopupContainer').wrapAll('<div class="reports-delete-popup" />');
	    }
	    $("#delete-msg-wizard").show();

		$('.ui-dialog-titlebar-close').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		}catch(e){
			// to do
		}
	},

	checkBoxSelected : function(response) {
		try{
		$(".dt-others").each(function() {
			var previousObj = this;
			$.each(response.delivery_type, function(key, value) {
				if($(previousObj).val() == key)
					$(previousObj).attr('checked', 'true');
			});
		});

		this.checkboxValidation();
		}catch(e){
			// to do
		}
	},

	checkboxValidation : function() {
		try{
		$('#searchopts-content').find('input[type=checkbox]').each(function() {
			if($(this).is(':checked')){
				if($(this).val() != 'All'){
					$(this).parent().next('label').removeClass('highlight-light-blue');
					$(this).parent().next('label').addClass('highlight-light-gray');
					$(this).parent().removeClass('checkbox-unselected');
					$(this).parent().addClass('checkbox-selected');
				}
			} else {
				if($(this).val() != 'All'){
					$(this).parent().next('label').removeClass('highlight-light-gray');
					$(this).parent().next('label').addClass('highlight-light-blue');
					$(this).parent().removeClass('checkbox-selected');
					$(this).parent().addClass('checkbox-unselected');
				}
			}

		});
		}catch(e){
			// to do
		}
	},

	typeCheckboxUnSelect : function(){
		try{

		$(".dt-others").each(function() {
			$(this).removeAttr("checked");
		});
		}catch(e){
			// to do
		}
	},

	unselectFilter : function(response) {
		try{

		}catch(e){
			// to do
		}

	},

	paintLPSearchResults : function(cellvalue, options, rowObject) {
		try{
		return rowObject['details'];
		}catch(e){
			// to do
		}
	},

	paintLPSearchResultsAction : function (cellvalue, options, rowObject) {
		try{
		return rowObject['action'];
		}catch(e){
			// to do
		}
	},

	reportEnterKey : function(){
		try{
		 obj = this;
		 $("#srch_criteria_location").keyup(function(event){
			 if(event.keyCode == 13){
				 obj.searchAction();
			 }
		 });
		}catch(e){
			// to do
		}
	},

	paintReportAutocomplete : function(){
		try{
		$('#srch_criteria_report_title').autocomplete(
			"/?q=learning/report-autocomplete",{
			minChars :3,
			max :50,
			autoFill :true,
			mustMatch :false,
			matchContains :false,
			inputClass :"ac_input",
			loadingClass :"ac_loading"
		});
		}catch(e){
			// to do
		}
	},

	tagEnterKey : function(){
		try{
		 obj = this;
		 $("#srch_criteria_report_title").keyup(function(event){
			 if(event.keyCode == 13){
				 obj.searchAction();
			 }
		 });
		}catch(e){
			// to do
		}
	},

	clearField : function (txt) {
		try{
		$('#srch_criteria_report_title').val('Type a report name').css('color','#999999').css('fontSize','11px').css('fontStyle','italic');
		$('#location-clr').css('display','none');
		this.searchAction();
		}catch(e){
			// to do
		}
	},

	showHide : function (strOne,strTwo) {
		try{
		$('#'+strTwo).toggle();
		var classShowHide = $('#'+strOne).hasClass('cls-show');
		if(classShowHide){
			$('#'+strOne).removeClass('cls-show');
			$('#'+strOne).addClass('cls-hide');
		}else{
			$('#'+strOne).removeClass('cls-hide');
			$('#'+strOne).addClass('cls-show');
		}
		}catch(e){
			// to do
		}
	},

	searchActionCheck : function() {
		try{
		var req=window.location.search;
		req=req.substring(3,req.length);
		var reqArr=req.split("/");
		var searchStr = '';
		if(this.currTheme == "expertusoneV2"){
		  var passSearchValue = Drupal.t('LBL304');
		}else{
		  var passSearchValue = Drupal.t("LBL304").toUpperCase();
		}

		if(reqArr.length>2){
			searchStr = (reqArr[2]!=null && reqArr[2]!=undefined && reqArr[2]!='undefined')?reqArr[2]:'';
			reqTitleSearch=searchStr.split("|");
			if(reqTitleSearch!='' && reqTitleSearch!=null && reqTitleSearch!=undefined && reqTitleSearch!='undefined') {
				 passSearchTitle = reqTitleSearch[0];
				 passSearchValue = reqTitleSearch[1];
			}
		}

		$('#reports_searchtext').val(unescape(passSearchValue));

		if((passSearchValue.toLowerCase())==(Drupal.t('LBL304').toLowerCase()))
			return '';
		else
			return unescape(passSearchValue);
		}catch(e){
			// to do
		}
	},

	hightlightedText : function(ID,textType) {
		try{
		$("#"+ID).blur(function(){
   			if(Drupal.t($("#"+ID).val()) != Drupal.t(textType)) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(Drupal.t(textType)).css('color','#999999').css('fontSize','11px').css('fontStyle','normal');
    		}
		});
		$("#"+ID).focus(function(){
   			if(Drupal.t($("#"+ID).val()) != Drupal.t(textType)) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if(Drupal.t($("#"+ID).val()) == Drupal.t(textType)) {
        		$(this).val('').css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
    		}
		});
		$("#"+ID).change(function(){
   			if(Drupal.t($("#"+ID).val()) != Drupal.t(textType)) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(Drupal.t(textType)).css('color','#999999').css('fontSize','11px').css('fontStyle','normal');
    		}
		});
		}catch(e){
			// to do
		}
	},

	displayTagTip : function(elementid, messagecontent){
	 try{
		if(!document.getElementById("tooltip"+elementid)) {
			$('#'+elementid).qtip({
				 content: '<div id="tooltip'+elementid+'" class="tooltiptop"></div><div class="tooltipmid"><div style="width:220px;">'+messagecontent+'</div></div><div class="tooltipbottom"></div>',
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
				position: { adjust: { x: -75, y: 0 } },
				style: {
					width: 325,
					background: 'none',
					'font-size' : 12,
					color: '#333333'
				}
			});
		}
	 }catch(e){
			// to do
		}
	},

	jqSelector : function(str){
		try{
		return str.replace(/([;&,\.\+\*\~':"\!\^#$%@\[\]\(\)=>\|])/g, '\\$1');
		}catch(e){
			// to do
		}
	},

	callReportExportProcess : function(reportId,type){
		try{
        var obj = this;

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

    	var sortname = $("#report_grid_container").jqGrid('getGridParam','sortname');
    	var sortOrder = $("#report_grid_container").jqGrid('getGridParam','sortorder');

         var reportEditable = true;
         var preview = 0;

		if(type == 'PDF' || type == 'CSV'){
		 EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("report-builder-section");
         var countType = (type == 'PDF') ? "PDFcount" : "CSVcount";
         var extn_type = (type == 'PDF') ? "PDF" : "CSV";
         curl = obj.constructUrl('learning/report-result-content-export/'+reportId+'/'+reportEditable+'/'+preview+'/'+sortname+'/'+sortOrder+'/'+countType+'&inputNameValues='+encodeURIComponent(elementNameAndValue));
			$.ajax({
				type: "POST",
				url: curl,
				success: function(result){
				       EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("report-builder-section");
						if(result == "exceed_max_limit"){
							var msg = new Array();
						 	msg[0] = Drupal.t('@extensions export could not be generated due to large number of records. Please use scheduling to generate your report.', {'@extensions': extn_type});
						 	var message_call = expertus_error_message(msg,'error');
						 	$('#show_expertus_message').show();
						 	$('#show_expertus_message').html(message_call);
						}else{
							url = obj.constructUrl('learning/report-result-content-export/'+reportId+'/'+reportEditable+'/'+preview+'/'+sortname+'/'+sortOrder+'/'+type+'&inputNameValues='+encodeURIComponent(elementNameAndValue));
	         				window.location = url;
						}					
				}
			});
         }else{
         	url = obj.constructUrl('learning/report-result-content-export/'+reportId+'/'+reportEditable+'/'+preview+'/'+sortname+'/'+sortOrder+'/'+type+'&inputNameValues='+encodeURIComponent(elementNameAndValue));
          	window.location = url;
         }
		}catch(e){
			// to do
		}
	},
	refreshLastAccessedReportResult: function() {
		try {
			var rowFound = false;
			// console.log('refreshLastAccessedReportResult', console.trace());
			var grid = $('#paintLearnerReportsResults');
			var gridRow = grid.jqGrid('getGridParam', 'lastAccessedRow');
			if(gridRow !== undefined && gridRow != null) {
				// console.log(gridRow, gridRow.id);
				var options = {
					data: {
						jqgrid_row_id: gridRow.id,
						page: 1,
						rows: 1
					}
				};
				grid.jqGrid('updateRowByRowId', options);
				rowFound = true;	// return true to stop the grid reload
				grid.jqGrid('setGridParam', {lastAccessedRow: null});
			} else {
				// grid.trigger("reloadGrid",[{page:1}]);
				rowFound = false;	// return false so that reload of grid happens as no last accessed grid row found
			}
		} catch(e) {
			rowFound = false;
			// window.console.log(e, e.stack);
		}
		return rowFound;
	}

});

$.extend($.ui.lnrreportssearch.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true}});

})(jQuery);

$(function() {
	try{
	$( "#lnr-reports-search" ).lnrreportssearch();

	$('#first_pager').click( function(e) {
		if( Drupal.settings.ajaxPageState.theme == "expertusoneV2")
		{
			$('.narrow-search-sortbar-sortlinks').hide(); //hide sortby option
		}
		if(!$('#first_pager').hasClass('ui-state-disabled')) {
			$('#paintLearnerReportsResults').hide();
			$('#gview_paintLearnerReportsResults').css('min-height','100px');
			$("#lnr-reports-search").data("lnrreportssearch").createLoader('searchReportsRecordsPaint');
		}
	});
	$('#prev_pager').click( function(e) {
		if( Drupal.settings.ajaxPageState.theme == "expertusoneV2")
		{
			$('.narrow-search-sortbar-sortlinks').hide(); //hide sortby option
		}
		if(!$('#prev_pager').hasClass('ui-state-disabled')) {
			$('#paintLearnerReportsResults').hide();
			$('#gview_paintLearnerReportsResults').css('min-height','100px');
			$("#lnr-reports-search").data("lnrreportssearch").createLoader('searchReportsRecordsPaint');
		}
	});
	$('#next_pager').click( function(e) {
		if( Drupal.settings.ajaxPageState.theme == "expertusoneV2")
		{
			$('.narrow-search-sortbar-sortlinks').hide(); //hide sortby option
		}
		if(!$('#next_pager').hasClass('ui-state-disabled')) {
			$('#paintLearnerReportsResults').hide();
			$('#gview_paintLearnerReportsResults').css('min-height','100px');
			$("#lnr-reports-search").data("lnrreportssearch").createLoader('searchReportsRecordsPaint');
		}
	});
	$('#last_pager').click( function(e) {
		if( Drupal.settings.ajaxPageState.theme == "expertusoneV2")
		{
			$('.narrow-search-sortbar-sortlinks').hide(); //hide sortby option
		}
		if(!$('#last_pager').hasClass('ui-state-disabled')) {
			$('#paintLearnerReportsResults').hide();
			$('#gview_paintLearnerReportsResults').css('min-height','100px');
			$("#lnr-reports-search").data("lnrreportssearch").createLoader('searchReportsRecordsPaint');
		}
	});
	$('#pager_center .ui-pg-selbox').bind('change',function() {
		$('#paintLearnerReportsResults').hide();
		$('#gview_paintLearnerReportsResults').css('min-height','100px');
		$("#lnr-reports-search").data("lnrreportssearch").createLoader('searchReportsRecordsPaint');
	});
	$("#pager_center .ui-pg-input").keyup(function(event){
		if(event.keyCode == 13){
			$('#paintLearnerReportsResults').hide();
			$('#gview_paintLearnerReportsResults').css('min-height','100px');
			$("#lnr-reports-search").data("lnrreportssearch").createLoader('searchReportsRecordsPaint');
		}
	});
	if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
		$('#lnr-reports-search .page-show-prev').bind('click',function() {
			if(parseInt($("#pg_pager .page_count_view").attr('id')) < 0){
				$("#pg_pager .page_count_view").attr('id','0');
			}else{
				$('#paintLearnerReportsResults').hide();
				$('#gview_paintLearnerReportsResults').css('min-height','100px');
				$("#lnr-reports-search").data("lnrreportssearch").createLoader('searchReportsRecordsPaint');
			}
		});

		$('#lnr-reports-search .page-show-next').bind('click',function() {
			if(parseInt($("#pg_pager .page_count_view").attr('id')) >= parseInt($("#pg_pager .page-total-view").attr('id'))){
				$("#pg_pager .page_count_view").attr('id',($("#pg_pager .page_count_view").attr('id')-1));
			}else{
				$('#paintLearnerReportsResults').hide();
				$('#gview_paintLearnerReportsResults').css('min-height','100px');
				$("#lnr-reports-search").data("lnrreportssearch").createLoader('searchReportsRecordsPaint');
			}
		});
	 }



	$("#reports_searchtext").keyup(function(event){
		if(event.keyCode == 13){
			$("#lnr-reports-search").data("lnrreportssearch").searchAction();
			$('.ac_results').css('display', 'none');
		}
	});

	$('#reports-search-txt').click(function() {
		$("#lnr-reports-search").data("lnrreportssearch").searchAction();
	});

	if($('#reports_searchtext')) {
		$('#reports_searchtext').autocomplete(
				"/?q=learning/report-autocomplete",{
				minChars :3,
				max :50,
				autoFill :true,
				mustMatch :false,
				matchContains :false,
				inputClass :"ac_input",
				loadingClass :"ac_loading"
		});
	}
	//change by ayyappan for 40545: Dropdown elements aren't disappered
	$('body').click(function (event) {
		try{
			if(event.target.id!='pub-unpub-action-btn') {
				$('.catalog-pub-add-list').hide();
			}
		}catch(e){
			// to do
		}
	});
	}catch(e){
		// to do
	}
});
/*Initization for report scheduling jqgrid*/
Drupal.behaviors.reportSchedulesPopup = {
  attach: function (context, settings) {
	  try{
    $('.datagrid-items-list:not(.exp-sched-popup-inited)').addClass('exp-sched-popup-inited').each(function () {
    	try{
    	var datagridDivId = $(this).attr('id');
    	var reportId = $(this).data('item-id');
    	$('body').data('mulitselectdatagrid').createLoader(datagridDivId);
    	$('body').data('mulitselectdatagrid').loadDataGrid('edit', 'ReportSchedulesList', '', reportId, 'cre_rpt_rss', '');
    	}catch(e){
			// to do
		}
    });

    $('.schedule-exports-icon:not(.exp-sched-popup-inited)').addClass('exp-sched-popup-inited').each(function () {
    	try{
    	$(this).click(function(event) {
        	var reportId = $(event.currentTarget).data('item-id');
        	var url = $('body').data('mulitselectdatagrid').constructUrl("administration/report-schedule/exportoption/" + reportId + "/" + "CSV");
        	window.location = url;
    	});
    	}catch(e){
			// to do
		}
    });

    $('.addedit-edit-mail_to:not(.exp-sched-popup-inited)').addClass('exp-sched-popup-inited').each(function () {
     try{
      var defaultText = $(this).data('default-text');
      var fieldValue = $(this).val();
      if (fieldValue == '' || fieldValue == defaultText) {
        $(this).removeClass('reports-nonempty-field');
        $(this).addClass('reports-empty-field');
        $(this).val(defaultText);
      }
      else {
        $(this).removeClass('reports-empty-field');
        $(this).addClass('reports-nonempty-field');
      }

      $(this).blur(function(event) {
        var defaultText = $(event.currentTarget).data('default-text');
        var fieldValue = $(event.currentTarget).val();
        if (fieldValue == '' || fieldValue == defaultText) {
          $(event.currentTarget).removeClass('reports-nonempty-field');
          $(event.currentTarget).addClass('reports-empty-field');
          $(event.currentTarget).val(defaultText);
        }
        else {
          $(event.currentTarget).removeClass('reports-empty-field');
          $(event.currentTarget).addClass('reports-nonempty-field');
        }
      });

      $(this).focus(function(event) {
        var defaultText = $(event.currentTarget).data('default-text');
        var fieldValue = $(event.currentTarget).val();
        if (fieldValue == defaultText) {
          $(event.currentTarget).val('');
        }
        $(event.currentTarget).removeClass('reports-empty-field');
        $(event.currentTarget).addClass('reports-nonempty-field');
      });

      $(this).change(function(event) {
        var defaultText = $(event.currentTarget).data('default-text');
        var fieldValue = $(event.currentTarget).val();
        if (fieldValue == '' || fieldValue == defaultText) {
          $(event.currentTarget).removeClass('reports-nonempty-field');
          $(event.currentTarget).addClass('reports-empty-field');
          $(event.currentTarget).val(defaultText);
        }
        else {
          $(event.currentTarget).removeClass('reports-empty-field');
          $(event.currentTarget).addClass('reports-nonempty-field');
        }
      });
     }catch(e){
			// to do
		}
    });

    $('#field-scroll-wrapper:not(.exp-sched-popup-inited)').addClass('exp-sched-popup-inited').each(function () {
     try{
      $(this).jScrollPane({});
     }catch(e){
			// to do
		}
    });

/*    $('.addedit-edit-date-criteria-dropdown:not(.exp-sched-popup-inited)').addClass('exp-sched-popup-inited').each(function () {
      var thisId = $(this).attr('id');
      var dateFieldId = thisId.replace('-dropdown', '');
      if ($(this).val() == 'Date') {
        $('#' + dateFieldId).datepicker({dateFormat: "yy-mm-dd",
                                         showOn: "both",
                                         buttonImage: themepath + "/expertusone-internals/images/calendar_icon.JPG",
                                         buttonImageOnly: true});
      }
      else {
        $('#' + dateFieldId).datepicker("destroy");
      }
      // Add change event handler
      $(this).change(function(event) {
          var thisId = $(event.currentTarget).attr('id');
          var dateFieldId = thisId.replace('-dropdown', '');
          $('#' + dateFieldId).val(''); // Clear the date field
          if ($(event.currentTarget).val() == 'Date') {
            $('#' + dateFieldId).datepicker({dateFormat: "yy-mm-dd",
                                             showOn: "both",
                                             buttonImage: themepath + "/expertusone-internals/images/calendar_icon.JPG",
                                             buttonImageOnly: true});
          }
          else {
            $('#' + dateFieldId).datepicker("destroy");
          }
        });
    }); */
      $('.addedit-date-criteria-container:not(.exp-sched-popup-inited)').addClass('exp-sched-popup-inited').each(function () {
    	  try{
    	  var thisId = $(this).attr('id');
    	  var radioName = $('#' + thisId + ' .date-criteria-radio-container').attr('name');
    	  if (  $("input[type='radio'][name='" + radioName + "']:checked").val() == 'date') {
    		 /* $('#' + thisId + ' .date-criteria-date-class').datepicker({ dateFormat: "mm-dd-yy",
						    	                                          showOn: "both",
						    	                                          buttonImage: themepath + "/expertusone-internals/images/calendar_icon.JPG",
						    	                                          buttonImageOnly: true});*/
    	       $('#' + thisId + ' .date-criteria-date-class').removeClass("addedit-readonly-textfield");
 	           $('#' + thisId + ' .date-criteria-days-class').addClass("addedit-readonly-textfield");
 	           $('#' + thisId + ' .date-criteria-days-type').addClass("select-greyed-out-text");
 	           $('#' + thisId + ' .date-criteria-days-type').prop('disabled', true);
 	           $('#' + thisId + ' .date-criteria-days-class').prop('disabled', true);
 	           $('#' + thisId + ' .date-criteria-date-class').prop('disabled', false);
    	      }
    	  else {
    		    //$('#' + thisId + ' .date-criteria-date-class').datepicker("destroy");
    	        $('#' + thisId + ' .date-criteria-date-class').addClass("addedit-readonly-textfield");
 	    	    $('#' + thisId + ' .date-criteria-days-class').removeClass("addedit-readonly-textfield");
 	    	    $('#' + thisId + ' .date-criteria-days-type').removeClass("select-greyed-out-text");
 	    	    $('#' + thisId + ' .date-criteria-days-type').prop('disabled', false);
 	    	    $('#' + thisId + ' .date-criteria-date-class').prop('disabled', true);
 	    	    $('#' + thisId + ' .date-criteria-days-class').prop('disabled', false);
    	      }
    	  $("input[type='radio'][name='" + radioName + "']").change(function() {
    		  this.currTheme = Drupal.settings.ajaxPageState.theme;
    		  if ($(this).val() == 'date') {
    			  /*$('#' + thisId + ' .date-criteria-date-class').datepicker({ dateFormat: "mm-dd-yy",
															                  showOn: "both",
															                  buttonImage: themepath + "/expertusone-internals/images/calendar_icon.JPG",
															                  buttonImageOnly: true});*/
    			   if(this.currTheme == "expertusoneV2"){
    				   $('#' + thisId + ' .ui-datepicker-trigger').css("display","none");
    			   }else{
    				   $('#' + thisId + ' .ui-datepicker-trigger').css("display","block");
    			   }
    	           $('#' + thisId + ' .date-criteria-date-class').removeClass("addedit-readonly-textfield");
    	           $('#' + thisId + ' .date-criteria-days-class').addClass("addedit-readonly-textfield");
    	           $('#' + thisId + ' .date-criteria-days-class').val('');
    	           $('#' + thisId + ' .date-criteria-days-type').val('DB');
    	           $('#' + thisId + ' .date-criteria-days-type').addClass("select-greyed-out-text");
    	           $('#' + thisId + ' .date-criteria-days-type').prop('disabled', true);
    	           $('#' + thisId + ' .date-criteria-days-class').prop('disabled', true);
    	           $('#' + thisId + ' .date-criteria-date-class').prop('disabled', false);
    	       }
    	       else {
    	    	   $('#' + thisId + ' .ui-datepicker-trigger').css("display","none");
    	    	   $('#' + thisId + ' .date-criteria-date-class').addClass("addedit-readonly-textfield");
    	    	   $('#' + thisId + ' .date-criteria-days-class').removeClass("addedit-readonly-textfield");
    	    	   $('#' + thisId + ' .date-criteria-date-class').val('mm-dd-yyyy');
    	    	   $('#' + thisId + ' .date-criteria-days-type').removeClass("select-greyed-out-text");
    	    	   $('#' + thisId + ' .date-criteria-days-type').prop('disabled', false);
    	    	   $('#' + thisId + ' .date-criteria-date-class').prop('disabled', true);
    	    	   $('#' + thisId + ' .date-criteria-days-class').prop('disabled', false);
    	       }
    	  });
    	  }catch(e){
  			// to do
  		}
      });



    /*$('.addedit-edit-sort_by:not(.exp-sched-popup-inited)').addClass('exp-sched-popup-inited').each(function () {
      if ($(this).val() == 0) {
        $('.addedit-edit-sort_order').val(0);
        $('.addedit-edit-sort_order').removeClass('select-normal-text');
        $('.addedit-edit-sort_order').addClass('select-greyed-out-text');
        $('.addedit-edit-sort_order').prop('disabled', true);
      }

      // Add change event handler
      $(this).change(function(event) {
        if ($(this).val() == 0) {
          $('.addedit-edit-sort_order').val(0);
          $('.addedit-edit-sort_order').removeClass('select-normal-text');
          $('.addedit-edit-sort_order').addClass('select-greyed-out-text');
          $('.addedit-edit-sort_order').prop('disabled', true);
        }
        else {
          $('.addedit-edit-sort_order').prop('disabled', false);
        }
      });
    });*/
	  }catch(e){
			// to do
		}
  }
};
(function($) {
    $.fn.scheduleParametersCallVtip = function(data) {
       vtip();
    };
})(jQuery);