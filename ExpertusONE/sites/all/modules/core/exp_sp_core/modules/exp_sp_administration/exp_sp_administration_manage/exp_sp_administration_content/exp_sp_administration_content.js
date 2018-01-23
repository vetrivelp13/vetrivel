var isPopupOpen=0;
function selContentHostedType(selVal) {
	try{
	var cType = $("#edit-content-type").val();	
	if((cType == 'lrn_cnt_typ_knc') || (cType == 'lrn_cnt_typ_vod')) {
		if($("#edit-hosted-type-file").attr("checked")) {
			$("#content_url_control").hide();
			$("#upload_browse_only").show();
			$("#content_browse_control").show();
		} else {
			$("#content_url_control").show();
			$("#content_browse_control").hide();
		}
	}
	else {
		$("#upload_browse_only").show();
		$("#upload_browse_and_url").hide();
		$("#edit-content-data").attr('size','35');
	}
	}catch(e){
		// to do
	}
}

//Content : While do click on text label, the textbox should be appear with dottet line
function toEditContentLessonTitle(id,act) {
	try{
	var callEvent ='blur';
	if(act == 'label') {
		
		if($.trim($("#content_title"+id).val()) == '') {
			$("#content_title"+id).val($("#content_title_label_"+id).attr("title"));
		}
		$("#content_title_label_"+id).hide();
		$("#content_title_label_edit_"+id).show();
		$("#content_title"+id).focus();
		callEvent = 'click';
	} else {
		$("#content_title_label_edit_"+id).hide();
		if($.trim($("#content_title"+id).val()) != ''){
		  $("#content_title_label_"+id).html($("#content_title"+id).val());
  	}
		$("#content_title_label_"+id).show();
	}
	var lessonId = 'content_title'+id;
	$("#"+lessonId).addClass("edit-attachment-txtbox");
	$("#"+lessonId).removeClass("non-editable-txt");
	$("#edit-lesson-title-container").find(".attachment-txt-box").each(function(){
	    var gId = $(this).attr("id");
	    if((gId  == lessonId) && (callEvent == 'click')) {
	    	$("#"+gId).removeClass("edit-attachment-txtbox");
	    	$("#"+gId).addClass("edit-attachment-txtbox");	    	
	    	$("#"+gId).removeClass("non-editable-txt");
	    } else {
	    	$("#"+gId).removeClass("edit-attachment-txtbox");
	    	$("#"+gId).removeClass("non-editable-txt");
	    	$("#"+gId).addClass("non-editable-txt");
	    }	    
	});
	}catch(e){
		// to do
	}
}
function contentTypeSelect() {	
	try{
	var cType = $("#edit-content-type").val();	
	if((cType == 'lrn_cnt_typ_knc') || (cType == 'lrn_cnt_typ_vod')) {
		$("#edit-hosted-type-file").attr("checked",true);
		$("#content_browse_control").show();
		$("#content_url_control").hide();
		$("#edit-content-data").attr('size','17');
		$("#upload_browse_and_url").show();
		if(cType == 'lrn_cnt_typ_vod'){
		  $("#upload_msg_part").show();
		 }
		else{
		  $("#upload_msg_part").hide();
		}
	}else {
		$("#content_browse_control").show();
		$("#upload_browse_only").show();
		$("#upload_browse_and_url").hide();
		$("#edit-content-data").attr('size','35');
		$("#upload_msg_part").hide();
	}	
	}catch(e){
		// to do
	}
}

//Attachment: In Edit attachment, to prevent default enter key submit 
function toEditLessonKeyDown(evt) {
	try{
	evt = evt || window.event;
    var charCode = evt.keyCode || evt.which;
    if (charCode == 13) {
    	$("#content_save_btn").click();
    	evt.preventDefault();
        return false;
    }
	}catch(e){
		// to do
	}
}
function sizeOfUploadButton(defaultContentType) {
	try{
	if($.browser.msie){
		if(defaultContentType == 'lrn_cnt_typ_knc' || defaultContentType == 'lrn_cnt_typ_vod') {
			$('#exp-sp-administration-content-addedit-form #content_browse_control div .form-file').attr("size","26");
		}else {
			$('#exp-sp-administration-content-addedit-form #content_browse_control div .form-file').attr("size","46");
		}
	}
	}catch(e){
		// to do
	}
}


/* function to set the selected version as default/current version */
function setVersionDefault(contentId,versionId){
	try {
	$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");
	
	var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("administration/manage/content/check-version-transfer-conflict/"+contentId+"/"+versionId);
	$.ajax({
			type: "POST",
			url: url,
			data:  '',
			success: function(result){
				var resultObj = JSON.parse(result);
				if(resultObj.newitems!=null){
					pubContentVersion(contentId,versionId,resultObj.newitems);
					$("body").data("mulitselectdatagrid").destroyLoader('paint-class-search-results-datagrid');
				}
				else if(resultObj.inprogress!=null){
					$('#select-class-equalence-dialog').remove();
					$('body').data('learningcore').callMessageWindow(Drupal.t('Content'),Drupal.t('MSG995'));/*Viswanathan modified the MSG string for #77773*/
					var prevZindex = $('#modalContent').css('z-index');
					$('#select-class-equalence-dialog .ui-widget-content').css('z-index', prevZindex+10);
					$('.ui-widget-overlay').css('z-index', prevZindex+1);
					$("body").data("mulitselectdatagrid").destroyLoader('paint-class-search-results-datagrid');
					return;
				}
				else{
					var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("ajax/administration/manage/content/set-default-version/"+contentId+"/"+versionId);
					$.ajax({
							type: "POST",
							url: url,
							data:  '',
							success: function(result){
								var currentPage = $('#admin-version-list-'+contentId+'-pagination_toppager .ui-pg-input').val();
								$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");
								$("#admin-version-list-"+contentId+"-pagination").trigger("reloadGrid",[{page:currentPage}]);
							}
				    });
				}
			}
    });
	
	
	
	
	
	
	}catch(e){
		// to do
	}
}

/* function to act on version transfer conflict while publishing selected version  */
function pubContentVersion(contentId,versionId,jobId){
	try{
		var wSize =  300;
		var title = Drupal.t('MSG993')+'"? <br/>'+Drupal.t('MSG994');/*Viswanathan modified the MSG string for #77773*/
	    $('#version-conf-wizard').remove();
	    var html = '';	    
	    html+='<div id="version-conf-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
		
	    html+= '<tr><td class="delete-msg-txtcolor" color="#333333" style="padding: 30px 15px; text-align:left;">'+title+'</td></tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#version-conf-wizard').remove();};
	   	closeButt[Drupal.t('Yes')]=function(){
	   		$("#version-conf-wizard").remove();
			var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("ajax/administration/manage/content/cancel-version-transfer/"+contentId+"/"+versionId+"/"+jobId);		
			$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
						var currentPage = $('#admin-version-list-'+contentId+'-pagination_toppager .ui-pg-input').val();
						$("#admin-version-list-"+contentId+"-pagination").trigger("reloadGrid",[{page:currentPage}]);
						$("body").data("mulitselectdatagrid").destroyLoader('paint-class-search-results-datagrid');
						
					}
		    });
	   	};
	    
	    $("#version-conf-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:wSize,
	        resizable:false,
	        modal: true,
	        title:Drupal.t("LBL3110"),//"title",
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:false,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
	
	    $('.ui-dialog').wrap("<div id='conf-object-dialog'></div>");
        var prevZindex = $('#modalContent').css('z-index');
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
	    this.currTheme = Drupal.settings.ajaxPageState.theme;
	    if(this.currTheme == "expertusoneV2"){
	    	changeDialogPopUI();
	    	$('.expertusV2PopupContainer').wrapAll('<div class="reports-delete-popup" />');
	    }
	    $("#version-conf-wizard").show();
	    $('#conf-object-dialog .ui-widget-content').css('z-index', prevZindex+10);
	    $('.ui-widget-overlay').css('z-index', prevZindex+1);
	    $('.ui-dialog-titlebar-close').click(function(){
	    	$("#version-conf-wizard").remove();
	    });
		
	
	}catch(e){
		// to do
	}
}

/* function to delete the selected version  */
function deleteContentVersion(contentId,versionId,conttitle){
	try{
		var wSize =  300;
		var title = Drupal.t('MSG357')+' "'+conttitle+'"? <br/>'+Drupal.t('LBL1281');
	    $('#delete-msg-wizard').remove();
	    var html = '';	    
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
		
	    html+= '<tr><td class="delete-msg-txtcolor" color="#333333" style="padding: 30px 15px; text-align:left;">'+title+'</td></tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};
	   	closeButt[Drupal.t('Yes')]=function(){
	   		$("#delete-msg-wizard").remove();
	   		$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");
			var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("ajax/administration/manage/content/delete-version/"+versionId);		
			$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
						var currentPage = $('#admin-version-list-'+contentId+'-pagination_toppager .ui-pg-input').val();	
						$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");
						$("#admin-version-list-"+contentId+"-pagination").trigger("reloadGrid",[{page:currentPage}]);
						
						
					}
		    });
	   	};
	    
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
        var prevZindex = $('#modalContent').css('z-index');
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
	    this.currTheme = Drupal.settings.ajaxPageState.theme;
	    if(this.currTheme == "expertusoneV2"){
	    	changeDialogPopUI();
	    	$('.expertusV2PopupContainer').wrapAll('<div class="reports-delete-popup" />');
	    }
	    $("#delete-msg-wizard").show();
	    $('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+10);
        $('.ui-widget-overlay').css('z-index', prevZindex+1);
		$('.ui-dialog-titlebar-close').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		
	
//		$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");
//		var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("ajax/administration/manage/content/delete-version/"+versionId);		
//	    //var obj = this;
//		$.ajax({
//				type: "POST",
//				url: url,
//				data:  '',
//				success: function(result){
//					var currentPage = $('#admin-version-list-'+contentId+'-pagination_toppager .ui-pg-input').val();	
//					$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");
//					$("#admin-version-list-"+contentId+"-pagination").trigger("reloadGrid",[{page:currentPage}]);
//					
//					
//				}
//	    });
	}catch(e){
		// to do
	}
}


/* call to jqgrid to display list of versions available for the content */
function contentVersionListDisplay(contentId){
	try{
	var pagerId	= '#admin-version-list-'+contentId+'-pagination_toppager';
	var objStr = '$("#root_admin").data("narrowsearch")';
	
	//CREATE LOADER ICON
	$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");
	// jqgrid table --> $("#admin-version-list-"+contentId+"-pagination")
	$("#admin-version-list-"+contentId+"-pagination").jqGrid({
		url: EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("administration/manage/content/version-list/"+contentId),
		datatype: "json",
		mtype: 'GET',
		//colNames:['Detail'],
		colModel:[ {name:'Detail',index:'detail', title: false, width:500, 'widgetObj':objStr, sortable: false, formatter: displayVersionDetail}],
		rowNum:5,
		rowList:[5,10,15],
		pager: pagerId,
		viewrecords:  true,
		multiselect: false,
		emptyrecords: "",
		toppager: false,
		height: "auto",
		width: 650,
		loadtext: "",
		recordtext: "",
		pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
		loadui:false,
		loadComplete:callbackVersionListDataGrid
		}).navGrid('#pager-datagrid-'+contentId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
	$("#admin-version-list-"+contentId+'-pagination_toppager').hide();					
	//setQtipPosition();
	}catch(e){
		// to do
	}
}


function callbackVersionListDataGrid(response, postdata, formid) {
	try {
	var listRows 	= 	5;
	//var curObj	 	=	this;
	var curObjStr 	= 	$("#root-admin").data("narrowsearch");
	$('#admin-version-list-'+response.contentId+'-pagination').show();
	$("#gview_admin-version-list-"+response.contentId+"-pagination").find(".ui-jqgrid-bdiv > div:first").css("position","static");
	if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
		$("#gview_admin-version-list-"+response.contentId+"-pagination").find(".ui-jqgrid-bdiv > div table.ui-jqgrid-btable div.edit-class-list table.edit-content-version-list-container").css("border-bottom","0").css("padding-bottom","0").css("display","block");
	}
	else{
		$("#gview_admin-version-list-"+response.contentId+"-pagination").find(".ui-jqgrid-bdiv > div table.ui-jqgrid-btable div.edit-class-list table.edit-content-version-list-container").css("border-bottom","1px dotted #CCCCCC").css("padding-bottom","5px").css("display","block");
	}
	$("#gview_admin-version-list-"+response.contentId+"-pagination").find(".ui-jqgrid-bdiv > div table.ui-jqgrid-btable tbody tr:last-child table.edit-content-version-list-container").css("border","none");
	var element = document.getElementById('modal-content');
	/*if(element.clientHeight == element.scrollHeight){
		$("#gbox_admin-version-list-"+response.contentId+"-pagination").css('width','840px');
		$("#gbox_admin-version-list-"+response.contentId+"-pagination").css('overflow','hidden');
	}*/
	// Hide the pagination, if the record count in the view learning table is equal to or less than
	var recordCount = $('#admin-version-list-'+response.contentId+"-pagination").jqGrid('getGridParam', 'records');

    if (recordCount == 0) {
    	$("#admin-version-list-"+response.contentId+"-pagination"). css('display','block');
        var html = Drupal.t('MSG381')+'.';
        $("#admin-version-list-"+response.contentId+"-pagination").html('<tr><td class="border-style-none" width="840px"><div id="add-edit-class-norecords" class="no-records-msg">'+html+'</div></td></tr>');
        $('#admin-version-list-'+response.contentId+"-pagination").css('text-align','center');
        $('.border-style-none').css('border','0');
    } else if(recordCount > listRows){
        $("#admin-version-list-"+response.contentId+"-pagination_toppager").show();
        $('#add-edit-class-norecords').css('display','none');
	} else {
		$("#admin-version-list-"+response.contentId+"-pagination_toppager").hide();
		$('#add-edit-class-norecords').css('display','none');
	}
	
	initGridVersionPagination(response.contentId);
	$("body").data("mulitselectdatagrid").destroyLoader('paint-class-search-results-datagrid');					

	$('#admin-version-list-pagination-'+response.contentId+' tr').click(function(event){
		event.stopPropagation();
	});
	
	$('.edit-class-list-container').last().css('border','0px none');
	var backdrop_height = $(document).height();
	$('#modalBackdrop').css('height',backdrop_height+'px');
	//Vtip-Display toolt tip in mouse over
	vtip();
	
	Drupal.ajax.prototype.commands.CtoolsModalAdjust();
	}catch(e){
		// to do
	}
}

function initGridVersionPagination(contentId) {
	try {
	//NEXT PREVIOUS LOADER SETTING
	$('#prev_admin-version-list-'+contentId+'-pagination_toppager').click(function(e) {
						if (!$('#prev_admin-version-list-'+contentId+'-pagination_toppager').hasClass('ui-state-disabled')) {
							$('#admin-version-list-'+contentId+'-pagination').hide();
							$('#gview_admin-version-list-'+contentId+'-pagination').css('min-height','50px');
							$("body").data("mulitselectdatagrid").createLoader('paint-class-search-results-datagrid');
						}
					});
	$('#next_admin-version-list-'+contentId+'-pagination_toppager').click(function(e) { 
						if (!$('#next_admin-version-list-'+contentId+'-pagination_toppager').hasClass('ui-state-disabled')) {
							$('#admin-version-list-'+contentId+'-pagination').hide();
							$('#gview_admin-version-list-'+contentId+'-pagination').css('min-height','50px');
							$("body").data("mulitselectdatagrid").createLoader('paint-class-search-results-datagrid');
						}
					});

	$(".ui-pg-input").keyup(function(event) {
						if (event.keyCode == 13) {
							$('#admin-version-list-'+contentId+'-pagination').hide();
							$('#gview_admin-version-list-'+contentId+'-pagination').css('min-height','50px');
							$("body").data("mulitselectdatagrid").createLoader('paint-class-search-results-datagrid');
						}
					});
	}catch(e){
		// to do
	}
}


function displayVersionDetail(cellvalue, options, rowObject){
	try{
	return rowObject['detail'];
	}catch(e){
		// to do
	}
}


function loadDataGrid(mode, type, searchKeyword, contentId, versionId, excludedId, gridObj){
	try{
	var deleteOption = (mode == 'edit' || mode == 'view_only') ? true : false;
	mode = (mode == 'view_only') ? 'view' : mode;
	var gridmode = (mode == 'edit') ? true : false;
	var multiselectOption = (mode == 'edit') ? false : true;
	var objStr = '';
	var obj = $('body').data('mulitselectdatagrid');
	obj.mode = mode;
	obj.type = type;
	obj.entityId = contentId;
	obj.entityType = versionId;
	obj.uniqueId = type+'-'+contentId+'-'+versionId;
	uniqueId = type+'-'+contentId+'-'+versionId;
	
	var multiselectCheckbox = ' ';
	multiselectCheckbox = '<input type="checkbox" id="multiselect-selectall-'+uniqueId+'" class="multiselect-selectall" onclick ="checkboxSelectedUnselectedCommon(this);" />';

    /* part for displaying jqgrid for users inside the content move users form */
	var searchType = $('#search_all_moveuser_type-hidden').val();
	this.currTheme = Drupal.settings.ajaxPageState.theme;
	   if (this.currTheme == "expertusoneV2"  &&  navigator.userAgent.indexOf("Chrome")>0) {
		   var gridWidth=480;
	    }
	   else
	   {
		   var gridWidth=500;
	   }
	$("#datagrid-container-"+uniqueId).jqGrid({
		url: EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("administration/content/grid-moveusers/"+mode+"/"+type+"/"+encodeURIComponent(searchKeyword)+"/"+contentId+"/"+versionId+"/"+excludedId),
		datatype: "json",
		mtype: 'GET',
		postData: {searhType : searchType},
		colNames:[ Drupal.t('LBL054'), Drupal.t('LBL691'),  Drupal.t('LBL262'), Drupal.t('LBL102'), multiselectCheckbox],
		colModel:[ 
		          //37977: sorting option is missing to Username and Full name
		           { name:'UserName',index:'UserName', title: false, width:70, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true },
		           { name:'FullName',index:'FullName', title: false, width:90, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true },
		           { name:'ClassTitle',index:'ClassTitle', title: false, width:90, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true },
		           { name:'Status',index:'Status', title: false, width:70, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true },
		           {name:'MultiselectCheck',index:'MultiselectCheck', title: true, width:17, 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, hidden: multiselectOption}],
		rowNum: 6,
		rowList:[10,25,50],
		pager: '#pager-datagrid-'+uniqueId,
		viewrecords:  true,
		multiselect: false,
		emptyrecords: "",
		sortorder: "desc",
		toppager: true,
		botpager: false,
		height: 'auto',
		width: gridWidth,
		loadtext: "",
		recordtext: "",
		pgtext : "{0} of {1}",
		//pgtext : "",
		loadui:false,
		loadComplete:obj.callbackDataGrid
	}).navGrid('#pager-datagrid-'+uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});								
	$("#datagrid-container-"+uniqueId+"_toppager_center .search-pagination-right").find("span:first-child").remove();
	if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
	$('#datagrid-container-'+uniqueId+"_MultiselectCheck").find("div").css("margin-left","2px");
	}
	 if(navigator.userAgent.indexOf("Chrome")>0){
			$('#gview_datagrid-container-'+uniqueId+' .ui-jqgrid-bdiv #datagrid-container-'+uniqueId+' tr.jqgfirstrow td:nth-child(4)').css('width','92px');
	 }
	}catch(e){
		// to do
	}
}


function searchVersionDataGrid(mode, type, searchKeyword, contentId, versionId, excludedId){
	try {
	var uniqueId = type+'-'+contentId+'-'+versionId;
	$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId);
	searchKeyword = searchKeyword.replace(/\//g, "|");
	var dataGridURL = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("administration/content/grid-moveusers/"+mode+"/"+type+"/"+encodeURIComponent(searchKeyword)+"/"+contentId+"/"+versionId+"/"+excludedId);
	/* clicking on the search icon in move users qtip */
	var searchType = $('#search_all_moveuser_type-hidden').val();
	$('#datagrid-container-'+uniqueId).setGridParam({url: dataGridURL,postData: { searhType : searchType} });
    $('#datagrid-container-'+uniqueId).trigger("reloadGrid",[{page:1}]);
	}catch(e){
		// to do
	}
}

function moreBrowseSearchHideShow(){
	try{
                 $('#select-list-dropdown-list').slideToggle();
                 $('#select-list-dropdown-list li:last').css('border-bottom','0px none'); 
                // $('#upload_browse_and_url').find('#search-list-title-keyword').css('z-index','25');
	}catch(e){
		// to do
	}
}

function moreBrowseSearchTypeText(dCode,dText) {
	try{
                // $('#select-list-dropdown-list').hide();
                 $('#select-list-dropdown').text(dCode);
                 //var displayText = Drupal.t('LBL036') + ' ' + dCode;   
                 $('#hosted_type').val(dText);
	}catch(e){
		// to do
	}        
}

function convertVideoForMobileAccess(){
	try{
	$.ajax({
		url: "/VideoConversionForMobile.php"
		});
	}catch(e){
		// to do
	}
}

$('#content-addedit-form').live("click",function(e){	
	if($('#select-list-dropdown-list').css("display","block")){
	 if (!$(e.target).hasClass('select-list-dropdown-link')) { 	
 		$('#select-list-dropdown-list').css("display","none");
 		} 	
	 }
});

$('#edit-content-data').live("click",function(){
	try{
			$(this).change(function(){
			    $('#BrowserVisible').find('#FileField').focus()	;
			    var files = this.files;
		        if (files && files.length) {
		            $('#BrowserVisible').find('#FileField').val(files[0].name);
		        }
		        else {
		        	$('#BrowserVisible').find('#FileField').val($(this).val().replace("C:\\fakepath\\", ""));
		        }
		    });
	}catch(e){
		// to do
	}
});
//}    

$('body').click(function (event){
	try{
	if(event.target.id!='pub-unpub-action-btn')  {
		$('.catalog-pub-add-list,.catalog-pub-add-list-move').hide();
		} 
	if(event.target.id!='admin-dropdown-arrow'){
		$('#select-moveuser-dropdown-list dropdown-link-font ').hide();
		} 
	if(event.target.id!='transfer-dd-arrow'){
		//$('#transfer-dd-list').hide();
		$('#transfer-dd-list').css('visibility','hidden');
		} 
	}catch(e){
		// to do
	}
});

function setPosition(obj,id,title){
	try{
	var x = $('#'+id).offset().top;
	var y = $(obj).offset().top;
	$('#'+id).html(actionLink + " " +title);
	$(obj).css('position',"relative");
	$(obj).css('top',x-y+"px");
	}catch(e){
		// to do
	}
}

function contentShowHideDropDown(){
	try{
	var st = $('#transfer-dd-list').css('visibility');
	if(st == 'hidden'){
		$('#transfer-dd-list').css('visibility','visible');
	}else{
		$('#transfer-dd-list').css('visibility','hidden');
	}
	}catch(e){
		// to do
	}
}

function transUserList(){
  try{	
  //$('#transfer-dd-list').toggle();
  if(isPopupOpen != 1){
	contentShowHideDropDown();
	  var widthTrUsr=$('#transfer-dd-list').width();
	  if(widthTrUsr < "50")
	  {
		  $('#transfer-dd-list').css("left","108px");
		  $('.transfer-dd-list-arrow').css("left",widthTrUsr/2+'px');
	  }
	  else if(widthTrUsr > "100" && widthTrUsr < "160")
	  {
		  $('#transfer-dd-list').css("left","auto");
		  $('.transfer-dd-list-arrow').css("left",widthTrUsr/2+'px');
	  }
	  else if(widthTrUsr > "160" && widthTrUsr <= "200")
	  {
		  $('#transfer-dd-list').css("left","35px");
		  $('.transfer-dd-list-arrow').css("left",widthTrUsr/2+'px');
	  }
  }
  return false;
  }catch(e){
		// to do
	}
}

(function($) {
  Drupal.ajax.prototype.commands.expVideoPreviewQtip = function(ajax, response, status) {
    try {
      // Remove any open qtip popup from DOM
      $(".active-qtip-div").remove();
      
      //Initialize variables
      var qtipObjJSON = $('#' + ajax.element.id).attr('data');
      var qtipObj = eval('(' + qtipObjJSON + ')');

var popupHolderId = qtipObj.holderId;
      var popupId = qtipObj.id;
      var renderedHeight = (typeof qtipObj.renderedHeight == 'undefined')? 230 : qtipObj.renderedHeight; //setting some rendered height when not set

      // Add the qtip popup to DOM
      $('#' + popupHolderId).html(response.html);
      
      // Determine whether to place the qtip above or below the clicked element
      var qtipPlacement = 'above';
      var triggerPos = $('#' + ajax.element.id).offset();
      var availableSpaceAbove = triggerPos.top - $(window).scrollTop();
      if (availableSpaceAbove < 350) {
        qtipPlacement = 'below';
      }
      
      // Detemine position of qtip arrow and qtip box
      //Initialize
      var qtipArrowPos = {};
      var qtipBoxPos = {};
      var triggerWidth = $('#' + ajax.element.id).width();
      var triggerHeight = $('#' + ajax.element.id).height();

      if (qtipPlacement == 'above') {
        // Add the bottom pointing arrow image (bottom-qtip-tip)
        $('#' + popupHolderId).append('<div id="arrow-' + popupId + '" class="bottom-qtip-tip active-qtip-div qtip-arrow" style="display:none"></div>');

        // Calculate the qtip arrow left position
        var downArrowImageWidth = $('#' + popupHolderId + ' .bottom-qtip-tip').width();
        var downArrowLeftAdjust = (downArrowImageWidth - triggerWidth)/2;
        qtipArrowPos.left = triggerPos.left - downArrowLeftAdjust;
       
        // Calculate the qtip arrow top position
        var downArrowImageHeight = $('#' + popupHolderId + ' .bottom-qtip-tip').height();
        qtipArrowPos.top = triggerPos.top - (downArrowImageHeight - 15); // the image needs to overlap the triggering element a bit
                                                                         // as arrow tip is not at bottom of image.
        // Calculate the qtip box left position
        qtipBoxPos.left = triggerPos.left - downArrowLeftAdjust - 60;
        
        // Calculate the qtip box top position
        if ($(".page-administration-contentauthor-video").size()>0) {
        	qtipBoxPos.top = qtipArrowPos.top - renderedHeight + 12-26;
        }else {
        	qtipBoxPos.top = qtipArrowPos.top - renderedHeight + 12 ;
        }
         //the box slightly overlaps the arrow
      }
      else { // qtip placement is below the clicked link
        // Add the up pointing arrow image (bottom-qtip-tip-up)
        $('#' + popupHolderId).append('<div id="arrow-' + popupId + '"class="bottom-qtip-tip-up active-qtip-div qtip-arrow" style="display:none"></div>');
        
        // Calculate the qtip arrow left position
        var upArrowImageWidth = $('#' + popupHolderId + ' .bottom-qtip-tip-up').width();
        var upArrowLeftAdjust = ((upArrowImageWidth - triggerWidth)/2);
        qtipArrowPos.left = triggerPos.left - upArrowLeftAdjust;
        
        // Calculate the qtip arrow top position
        var upArrowImageHeight = $('#' + popupHolderId + ' .bottom-qtip-tip-up').height();
        qtipArrowPos.top = triggerPos.top + triggerHeight - 5; // the image needs to overlap the triggering element a bit
                                                               // as arrow tip is not at top of image.
        // Calculate the qtip box left position
        qtipBoxPos.left = triggerPos.left - upArrowLeftAdjust - 60;

        // Calculate the qtip box top position
        qtipBoxPos.top = qtipArrowPos.top + upArrowImageHeight - 24; //the box slightly overlaps the arrow
      }

      $('#arrow-' + popupId).css({'position': 'absolute', 'z-index' : '1003'})
                            .show() //jQuery offset() does not work correctly on a hidden div
                            .offset(qtipArrowPos);
      $('#' + popupId).css({'position': 'absolute', 'z-index' : '1002'})
                      .show() //jQuery offset() does not work correctly on a hidden div
                      .offset(qtipBoxPos);

      Drupal.attachBehaviors();

	 
    }
    catch(e){
      //Nothing to do
	  // console.log(e);
    }
  
  };
  Drupal.ajax.prototype.commands.initializeVideoJSPlayer = function(ajax, response, status) {
    try {
		disposeVideoJSPlayer(response.video_id);
		if(videojs !== undefined && response.video_id !== undefined) {
			videojs(response.video_id, {}, function(){
			// Player (this) is initialized and ready.
				var vodPlayer = this;
				vodPlayer.play();
				vodPlayer.on('mouseout', function(){ 
					this.controls(false);
				});

				vodPlayer.on('mouseover', function(){
					this.controls(true);
				});
			});
		}
	}
    catch(e){
      //Nothing to do
	  // console.log(e);
    }
  
  };
jQuery('.version-content-transfer-user-txt').live("click",function(){
	$('#transfer-dd-arrow').click();
}) ;
$('.video-preview-popup .qtip-close-button').live("click",function(){
	disposeVideoJSPlayer('all');
}) ;
})(jQuery);

Drupal.ajax.prototype.commands.displayContentConfirmationmessagewizard = function(ajax, response, status) {
    try {
    	
    	this.currTheme = Drupal.settings.ajaxPageState.theme;
    	var cId = response.cId;
    	var cvId = response.cvId;
    	var trgElmt = response.trgElmt;
    	var uniqueId = 'ContentMoveUsers-'+cId+'-'+cvId;
	    $('#contentmoveuser-msg-wizard').remove();
	    $('input[name="hidden_move_content_users_'+cId+'-'+cvId+'"]').attr('value',0);
	    var html = '';
	    html+='<div id="contentmoveuser-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    
	    if($('#multiselect-selectall-ContentMoveUsers-'+cId+'-'+cvId).attr("checked") || trgElmt == 'Move_All'){
	    	var title = Drupal.t('MSG776') + ' ' + Drupal.t('MSG777') + '. ' + Drupal.t('MSG728');
	    }else{
	    	var title = Drupal.t('MSG778') + ' ' + Drupal.t('MSG777') + '?';
	    }
    	

	   	html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);

	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ 
												    $('input[name="hidden_completedvalue_'+uniqueId+'"]').attr('value',0);
												    $('#contentmoveuser-msg-wizard').remove(); 
												    $.fn.refreshVersionList(cId);
												    };

         if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
	    	var esignObj = {'popupDiv':'catalog-publish-unpublis-dialog',
	    			'esignFor':'displayContentConfirmationmessagewizard','catalogId':dupId,'catalogType': 'Class','actionStatus': 'lrn_cls_sts_atv', 'rowObj': this};
	    	closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj);$(this).dialog('destroy');$(this).dialog('close');$('#contentmoveuser-msg-wizard').remove();$.fn.refreshVersionList(cId);};
         }else{
        	 
        	 closeButt[Drupal.t('Yes')]=function(){ $('input[name="hidden_move_content_users_'+cId+'-'+cvId+'"]').attr('value',2); 
										        	 $(this).dialog('destroy');$(this).dialog('close');
										        	 $('#contentmoveuser-msg-wizard').remove(); $('input[name="hidden_completedvalue_'+uniqueId+'"]').attr('value',0);
										        	 $('input[name = "'+ trgElmt +'"]').click(); $.fn.refreshVersionList(cId);
										        	 $.fn.refreshMoveUsersQtip(cId,cvId);
        	 };
         }

	    $("#contentmoveuser-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:550,
	        resizable:false,
	        modal: true,
	        title:Drupal.t('LBL947'),
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
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    //$(".removebutton").text(Drupal.t("LBL109"));
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

	    if ($.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
	    }
	    $("#contentmoveuser-msg-wizard").show();

		$('.ui-dialog-titlebar-close').click(function(){
	        $("#contentmoveuser-msg-wizard").remove();
	    });
	    changeDialogPopUI();
    }
    catch(e){
      //Nothing to do
    }
  };
  
  function setCompleteUserValue(uniqueId){
		$('input[name="hidden_completedvalue_'+uniqueId+'"]').attr('value',0);
		$.each($('#datagrid-container-'+uniqueId).find('.checkbox-selected'), function(){
		  if(($(this).find('.completed_sts').attr('checked')) == true){
			  $('input[name="hidden_completedvalue_'+uniqueId+'"]').attr('value',1);
		  }
	});
  }
  
