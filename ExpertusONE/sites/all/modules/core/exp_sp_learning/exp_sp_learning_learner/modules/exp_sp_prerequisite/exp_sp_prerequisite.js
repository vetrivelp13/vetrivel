(function($) {

$.widget("ui.prerequisite", {
	_init: function() {
		try{
		var self = this;
		this.referObj = '$("body").data("prerequisite")';
		}catch(e){
			// to do
		}
	},
	
	// To render a popup
	render_popup : function(id) {
		try{
		var html = "<div id='prereqCatalog"+id+"' style='height:250px;width:600px;overflow:visible;'></div>";
		$("#prereqCatalog"+id).remove();
		$('body').append(html);
		
		var nHtml = "<div id='show_expertus_message' style='display:none;'></div><div id='lnr-prerequisite-container'><table id='lnr-prerequisite'></table></div>";		
		$('#prereqCatalog'+id).html(nHtml);
		
		var closeButt = {};
		var prewidth = 780;
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback==true){
		    var iwidth = Math.max(document.documentElement.clientWidth, window.innerWidth || 0);
		    var lesswidth = 20;
		    if(iwidth>=800)
		    	lesswidth = 40;
		    prewidth = iwidth - lesswidth;
		}
			
		$("#prereqCatalog"+id).dialog({
			bgiframe: true,
			position :[(getWindowWidth()-750)/2,100],
			width:prewidth,
			resizable:false,
			draggable:false,
			closeOnEscape: false,
			modal: true,
			title:(Drupal.t('LBL716')+" <span id='preReqTitle'></span>"),
			buttons: closeButt,
			close: function() {
				$("#prereqCatalog"+id).remove();
				if($('.location-session-detail-clone').size()>0){
                	$('.location-session-detail-clone').remove();
                }
			},
			overlay: {
			   opacity: 0.5,
			   background: '#000000'
			 }	
		});		
		//$('#prereqCatalog'+id).parent().css('border','4px solid gray');
		}catch(e){
			// to do
		}
	},
	
	// To show a Message in the format of box 
	preMsgBox : function(msgTitle,msgTxt){
		try{
	    $('#preMsgBox-wizard').remove();	    
	    var html = '';
	    
	    html += '<div id="preMsgBox-wizard" style="display:none; padding: 0px;">';
	    html += '<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html += '<tr><td height="30"></td></tr>';
	    html += '<tr>';
	    html += '<td align="center"><i>'+msgTxt+'</i></td>';
	    html += '</tr>';
	    html += '</table>';
	    html += '</div>';
	    $("body").append(html);
	    
	    $("#preMsgBox-wizard").dialog({
	        position	:[(getWindowWidth()-500)/2,100],
	        bgiframe	: true,
	        width		: 500,
	        resizable	: false,
	        modal		: true,
	        title		: SMARTPORTAL.t(msgTitle),
	        closeOnEscape : false,
	        draggable	: true,
	        overlay		: {
	           				opacity: 0.9,
	           				background: "black"
	         			  }
	    });	
	  
	    $("#msgTitle-wizard").show();	
		$('.ui-dialog-titlebar-close').click(function(){
	        $("#msgTitle-wizard").remove();
	    });
		}catch(e){
			// to do
		}
	},
	
	getPrerequisites : function(id,divId) {
		try{
		var userId = this.getLearnerId();

		if(!userId){
			this.preMsgBox(Drupal.t("LBL721"),SMARTPORTAL.t("MSG027"));
		} else {
			this.render_popup(id);		
			this.createLoader("prereqCatalog"+id);		
			$('#prereqCatalog'+id).css("min-height","120px");
			var obj 	= this;
			var urlStr 	= this.constructUrl("learning/catalog-prerequisite/"+id+"/"+divId);
			var objStr 	= '$("#lnr-prerequisite").data("prerequisite")';
			//Embed widget related work (Create flexible grid)
			if(Drupal.settings.widget.widgetCallback==true){
				$("#lnr-prerequisite").jqGrid({
					url		 : obj.constructUrl("learning/catalog-prerequisite/"+id+"/"+divId),
					datatype : "json",
					mtype	 : 'GET',
					colNames :['Name','Code','Status'],
					colModel :[ {name:'Name',index:'name', title:false,'widgetObj':objStr,formatter:obj.paintPrereqResults},
					           {name:'Code',index:'code', title:false,'widgetObj':objStr,formatter:obj.paintPrereqResults},
					           {name:'Status',index:'status', title:false,'widgetObj':objStr,formatter:obj.paintPrereqResults}],
					rowNum:10,			
					viewrecords:  true,
					emptyrecords: "",
					sortorder: "asc",
					toppager:true,
					height: 'auto',
					autowidth: true,
	                shrinkToFit: true,
					loadtext: "",
					recordtext: "",			
					loadui:false,
					loadComplete:obj.callbackLoaderClass
				});
			}else{
				$("#lnr-prerequisite").jqGrid({
					url		 : obj.constructUrl("learning/catalog-prerequisite/"+id+"/"+divId),
					datatype : "json",
					mtype	 : 'GET',
					colNames :['Name','Code','Status'],
					colModel :[ {name:'Name',index:'name', title:false, width:600,'widgetObj':objStr,formatter:obj.paintPrereqResults},
					           {name:'Code',index:'code', title:false, width:200,'widgetObj':objStr,formatter:obj.paintPrereqResults},
					           {name:'Status',index:'status', title:false, width:200,'widgetObj':objStr,formatter:obj.paintPrereqResults}],
					rowNum:10,			
					viewrecords:  true,
					emptyrecords: "",
					sortorder: "asc",
					toppager:true,
					height: 'auto',
					width: 756,
					loadtext: "",
					recordtext: "",			
					loadui:false,
					loadComplete:obj.callbackLoaderClass
				});
			}
			
			
			$("#lnr-prerequisite_toppager").css("display","none");
			// Removing vertical scroll bar in Google Chrome by VJ
			$('.ui-jqgrid-bdiv').css('overflow','hidden');
			
		}
		}catch(e){
			// to do
		}
	},
	
	callbackLoaderClass : function(response, postdata, formid) {
		try{
		var crsId = response.rows[0].cell.crsId;
		$('body').data('prerequisite').destroyLoader("prereqCatalog"+crsId);
		var  len = response.rows.length -1;
		$("#"+response.rows[len].id+":last > td ").css("border-bottom","0px solid");
		$("#gview_lnr-prerequisite").css("border-bottom","0px solid");
		/*-- #40207 - Jscrollpane container has more course/class --*/
		$("body").data("learningcore").refreshJScrollPane('#lnr-prerequisite-container #gview_lnr-prerequisite');
		
		$('.limit-title').trunk8(trunk8.Prerequisite_title);
		$('.limit-desc').trunk8(trunk8.Prerequisite_desc);
		//Vtip-Display toolt tip in mouse over
		vtip();
		}catch(e){
			// to do
			// window.console.log(e, e.stack);
		}
	},
	
	getTpPrerequisites : function(id,divId,preLevel) {
		try{
		var userId 	= this.getLearnerId();
		var initReq = (preLevel == 'course') ? 'initClassPrereq' : 'initPrereq';
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback!=true){
		  	if(this.currTheme == "expertusoneV2"){
				// var dWidth = 744;
				// var dWidth = $('.pre-select-class').width();
				var dWidth = 780;
			}else{
				var dWidth = 756;
		    }
		}
		if(!userId){
			this.preMsgBox(Drupal.t("LBL721"),SMARTPORTAL.t("MSG027"));
		} else {
			
			this.render_popup(id);		
			this.createLoader("prereqCatalog"+id);		
			$('#prereqCatalog'+id).css("min-height","120px");
			var obj = this;
			var urlStr = this.constructUrl("learning/catalog-tpprerequisite/"+id+"/"+initReq+"/"+divId+"/"+preLevel);
			var objStr = '$("#lnr-prerequisite").data("prerequisite")';
			//Embed widget related work (Create flexible grid)
			if(Drupal.settings.widget.widgetCallback==true){
				$("#lnr-prerequisite").jqGrid({
					url:obj.constructUrl("learning/catalog-tpprerequisite/"+id+"/"+initReq+"/"+divId+"/"+preLevel),
					datatype: "json",
					mtype: 'GET',
					colNames:[''],
					colModel:[ {name:'tName',index:'preReqTpDetails', title:false,'widgetObj':objStr,formatter:obj.paintPrereqResults}],
					rowNum:-1,
					viewrecords:  true,
					emptyrecords: "",
					sortorder: "asc",
					toppager:true,
					height: 'auto',
					autowidth: true,
	                shrinkToFit: true,
					loadtext: "",
					recordtext: "",			
					loadui:false,
					loadComplete:obj.callbackLoaderClass,
					gridComplete:obj.callbackGridPreSelectClassComplete
				});
			}else{
				$("#lnr-prerequisite").jqGrid({
					url:obj.constructUrl("learning/catalog-tpprerequisite/"+id+"/"+initReq+"/"+divId+"/"+preLevel),
					datatype: "json",
					mtype: 'GET',
					colNames:[''],
					colModel:[ {name:'tName',index:'preReqTpDetails', title:false, width:610,'widgetObj':objStr,formatter:obj.paintPrereqResults}],
					rowNum: -1,
					viewrecords:  true,
					emptyrecords: "",
					sortorder: "asc",
					toppager:false,
					height: 'auto',
					width: dWidth,
					loadtext: "",
					recordtext: "",			
					loadui:false,
					loadComplete:obj.callbackLoaderClass,
					gridComplete:obj.callbackGridPreSelectClassComplete
				});
			}
			$("#lnr-prerequisite_toppager").css("display","none");
			$(".ui-jqgrid-hbox").css("display","none");
			$(".ui-jqgrid-hdiv").css("display","none");
			// Removing vertical scroll bar in Google Chrome by VJ
			$('.ui-jqgrid-bdiv').css('overflow','hidden');
			
			if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
				$('.ui-dialog .ui-dialog-content').addClass('pre-select-class');	
				changeDialogPopUI();
			}
		}
		vtip();
		}catch(e){
			// to do
		}
	},
	
	callbackGridPreSelectClassComplete: function(response, postdata, formid){
		try {
			scrollBar_Refresh('prerequisite');
			$('.limit-title').trunk8(trunk8.Prerequisite_title);
			$('.limit-desc').trunk8(trunk8.Prerequisite_desc);
			resetFadeOutByClass('#lnr-prerequisite','content-detail-code','line-item-container','select');	
		} catch(e) {
			// to do
		}
	},
	
	paintPrereqResults : function(cellvalue, options, rowObject) {
		try{
		var index  = options.colModel.index;
		if(index  == 'name') {
			$("#preReqTitle").html(' '+rowObject['parentTitle']);
			return rowObject['details1'];
		} else if(index  == 'code') {
			return rowObject['details2'];
		} else if(index  == 'status') {
			return rowObject['details3'];
		} else if(index  == 'preReqTpDetails') {
			return rowObject['preReqTpDetails'];
		} else {
			return '';
		}
		vtip();
		}catch(e){
			// to do
		}
	},
	
	getPrereqClass : function(currTr,divId) {
		try{
		var crsId = currTr;
		currTr = 'prereq'+currTr; 
		if(!document.getElementById(currTr+"SubGrid")) {
			$("#"+currTr).after("<tr id='"+currTr+"SubGrid' class='subClassRow'><td colspan='4' id='"+crsId+"preReqclassSubGrid' class='classSubGrid'><table id='"+currTr+"ClassSubGrid' class='subRow'></table></td></tr>");
			$("#"+currTr+"SubGrid").show();			
			$("#preReqTitle"+crsId).removeClass("title_close");
			$("#"+crsId+"prereq").addClass("subClassGridRow");			
			$("#preReqTitle"+crsId).addClass("title_open");
			$("#prereq"+crsId+" > td ").addClass("cls-grid-row-open");			
			$("#prereq"+crsId).removeClass("ui-widget-content");
			$("#"+currTr+".jqgrow").next().children().css("border-bottom","solid 1px #ccc");
			var classPrereq = this.classPrereqFun(crsId,currTr,divId);
		} else {
			var clickStyle = $("#"+currTr+"SubGrid").css("display"); 
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"SubGrid").show();//css("display","block");
    			$("#"+currTr+"SubGrid").slideDown(1000);
				$("#preReqTitle"+crsId).removeClass("title_close");
				$("#preReqTitle"+crsId).addClass("title_open");
				$("#prereq"+crsId+" > td ").removeClass("cls-grid-row-open");
				$("#prereq"+crsId+" > td ").addClass("cls-grid-row-close");
				$("#"+crsId).removeClass("ui-widget-content");
				var classPrereq = this.classPrereqFun(crsId,currTr,divId);
				this.destroyLoader(crsId+"preReqclassSubGrid");
    		} else {			
    			$("#"+currTr+"SubGrid").hide();//css("display","none");
    			$("#"+currTr+"SubGrid").slideUp(1000);
				$("#preReqTitle"+crsId).removeClass("title_open");
				$("#preReqTitle"+crsId).addClass("title_close");
				$("#"+crsId).removeClass("ui-widget-content");
				$("#prereq"+crsId+" > td ").removeClass("cls-grid-row-open");
				$("#prereq"+crsId+" > td ").addClass("cls-grid-row-close");
				$("#"+crsId).addClass("ui-widget-content");
				this.destroyLoader(crsId+"preReqclassSubGrid");
    		}
		}
		vtip();
		}catch(e){
			// to do
		}
	},
	
	classPrereqFun : function(crsId,clsDisp,divId) {
		try{
		var html = '';
		this.createLoader(crsId+"preReqclassSubGrid");
		var obj = this;
		var objStr = '$("#lnr-prerequisite").data("prerequisite")';
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback==true){
			$("#"+clsDisp+"ClassSubGrid").jqGrid({
				url:obj.constructUrl("learning/class-catalog-prerequisite/" + crsId+"/"+divId),
				datatype: "json",
				mtype: 'GET',
				colNames:['Name','Date','Start','End','Type',''],
				colModel:[ {name:'Name',index:'name', title:false,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'Date',index:'date', title:false,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'Start',index:'start', title:false,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'End',index:'end', title:false,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'Type',index:'type', title:false,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'action',index:'action', title:false,'widgetObj':objStr,formatter:obj.paintPrereqClsResult}],
				rowNum:10,
				viewrecords:  true,
				emptyrecords: "",
				sortorder: "asc",
				toppager:true,
				height: 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				loadui:false,
				loadComplete:obj.callbackLoaderPreReq
			});
		}else{
			$("#"+clsDisp+"ClassSubGrid").jqGrid({
				url:obj.constructUrl("learning/class-catalog-prerequisite/" + crsId+"/"+divId),
				datatype: "json",
				mtype: 'GET',
				colNames:['Name','Date','Start','End','Type',''],
				colModel:[ {name:'Name',index:'name', title:false, width:300,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'Date',index:'date', title:false, width:120,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'Start',index:'start', title:false, width:90,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'End',index:'end', title:false, width:90,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'Type',index:'type', title:false, width:100,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'action',index:'action', title:false, width:160,'widgetObj':objStr,formatter:obj.paintPrereqClsResult}],
				rowNum:10,
				viewrecords:  true,
				emptyrecords: "",
				sortorder: "asc",
				toppager:true,
				height: 'auto',
				width: 745,
				loadtext: "",
				recordtext: "",
				loadui:false,
				loadComplete:obj.callbackLoaderPreReq
			});
		}
		
		$("#prereq"+crsId+"ClassSubGrid_toppager").css("display","none");		
		$(".prereq-enrolled-txt").parent().parent().css('border','none');
		vtip();
		}catch(e){
			// to do
		}
	},
	
	callbackLoaderPreReq : function(response, postdata, formid) {
		try{
		var crsId = response.rows[0].cell.crsId;
		$('body').data('prerequisite').destroyLoader(crsId+"preReqclassSubGrid");
		var clsDisp = 'prereq'+crsId;
		$("#"+clsDisp+"ClassSubGrid tr.jqgrow").eq(-1).addClass("last").children("td").css("border-bottom","0px none");
		$("table.search-register-btn td").css("border-bottom","0px none");
		$('.limit-title').trunk8(trunk8.Prerequisite_title);
		$('.limit-desc').trunk8(trunk8.Prerequisite_desc);
		//Vtip-Display toolt tip in mouse over
		 vtip();	
		}catch(e){
			// to do
		}
	},

	paintPrereqClsResult : function(cellvalue, options, rowObject) {
		try{
		var index  = options.colModel.index;
		if(index  == 'name') {
			return rowObject['details4'];
		} else if(index  == 'date') {
			return rowObject['details5'];
		} else if(index  == 'start') {
			return rowObject['details6'];
		} else if(index  == 'end') {
			return rowObject['details7'];
		}else if(index  == 'type') {
			return rowObject['details8'];
		} else if(index  == 'action') {
			return rowObject['clsaction'];
		}
		vtip();
		}catch(e){
			// to do
		}
	},
	
	getPrereqClassDetail : function(currTr) {
		try{
		var crsId = currTr;
		currTr = 'preReqClassDet'+currTr;
		
		if(!document.getElementById(crsId+"ClassSubGrid")) {
			$("#"+currTr).after("<tr id='"+crsId+"ClassSubGrid'><td colspan='6'><span id='"+crsId+"clsDesc'>Loading . . . </span></td></tr>");
			$("#preReqClsTitle"+crsId).removeClass("title_close");
			$("#preReqClsTitle"+crsId).addClass("title_open");
			if($("#"+currTr).hasClass("last")==false) {
				$("#"+currTr+" td").css("border-bottom","0px none");
				$("#"+currTr+" td").parents("tr.jqgrow").next().children().css("border-bottom","solid 1px #ccc");
			}
			var classPrereq = this.showClassDetails(crsId);
		} else {
			var clickStyle = $("#"+crsId+"ClassSubGrid").css("display");
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
				$("#"+crsId+"ClassSubGrid").slideDown(1000);   
				$("#preReqClsTitle"+crsId).removeClass("title_close").addClass("title_open");
				$("#"+crsId).removeClass("ui-widget-content");
				$("#"+currTr+".jqgrow").children().css("border-bottom","0px none");
				var classPrereq = this.showClassDetails(crsId);
    		} else {			
    			$("#"+crsId+"ClassSubGrid").slideUp(1000);  
    			$("#preReqClsTitle"+crsId).removeClass("title_open").addClass("title_close");
				$("#"+crsId).removeClass("ui-widget-content").addClass("ui-widget-content");
				if($("#"+currTr).hasClass("last") == false)
				$("#"+currTr+".jqgrow").children().css("border-bottom","solid 1px #ccc");
    		}
		}
		vtip();
		}catch(e){
			// to do
		}
	},
	
	getTpPrereqCatalog : function(id,divId,catalogType,preLevel,parentId) {
		try{
		var crsId 		= id;
		//var gridRowId 	= catalogType+'prereq'+id;
		var gridRowId 	= catalogType+id+preLevel;
		var newGridId	= gridRowId+'SubGrid';
		var loaderDivId = gridRowId+'level';
		var jqGridTabId = gridRowId+'jgGrid';
		var jgTitleId   = "preTitle"+gridRowId;
		$.fn.extend({
			toggleText: function(a, b){
				return this.text(this.text() == b ? a : b);
			}
		});
		if(!document.getElementById(newGridId)) {
		//	alert("enter");
			$("#"+jgTitleId).before("<div id='"+newGridId+"' class='subClassRow' 	><div id='"+loaderDivId+"' class='classSubGrid'><div id='gview_lnr-tpprerequisite' class='gview_lnr-tpprerequisite'><table id='"+jqGridTabId+"' class='subRow'></table></div></div></div>");
			$("#"+newGridId).show();			
			
			// this.styleSwap(jgTitleId,'title_open','title_close');
			
			$("#"+jgTitleId).toggleText(Drupal.t('LBL713'), Drupal.t('LBL3042'));
			$("#"+gridRowId+" > td ").addClass("cls-grid-row-open");			
			$("#"+gridRowId+" > td ").css("border-bottom","solid 0px #ccc");
			$("#"+gridRowId+" tr:last > td ").css("border-bottom","solid 0px #ccc");
			$("#lnr-prerequisite tr:last > td ").css("border-bottom","solid 0px #ccc");
				
			var classPrereq = this.getTpPrereqList(id,divId,catalogType,preLevel);
			
			$("#gview_"+jqGridTabId).css("border-bottom","solid 0px #ccc");
			if(this.currTheme == 'expertusoneV2') {
				if(navigator.userAgent.indexOf("Chrome")>0 || navigator.userAgent.indexOf("Safari")>0 ){
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","25px");
					if(navigator.userAgent.indexOf("Safari")>0){
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","12px");
				      }
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","42px");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","25px");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","25px");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","42px");
					}
					else{
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","24px"); 
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","42px");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","0px");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","0");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","42px");
					}
					
					
				$("#gview_"+jqGridTabId+" .ui-jqgrid-hdiv").css("border-bottom","solid 1px #EDEDED");
			}else
			{
				if(navigator.userAgent.indexOf("Chrome")>0){
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","7px");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","30px");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","30px");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","15px");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","30px");
				}else{
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","-6px"); 
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","30px");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","30px");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","0");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","30px");
				}
				
				$("#gview_"+jqGridTabId+" .ui-jqgrid-hdiv").css("border-bottom","solid 1px #ccc");
			}
			$('body').data('prerequisite').createLoader("gview_"+jqGridTabId);
			
		} else {
			//alert("close");
			var clickStyle = $("#"+newGridId).css("display"); 
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			//alert("close001");
    			$("#"+newGridId).slideDown(1000, function() {
    				/*-- #40207 - Jscrollpane container has more course/class --*/
					$("body").data("learningcore").refreshJScrollPane('#lnr-prerequisite-container #gview_lnr-prerequisite');
    			});
    			// this.styleSwap(jgTitleId,'title_open','title_close');
				$("#"+jgTitleId).toggleText(Drupal.t('LBL713'), Drupal.t('LBL3042'));
    			if(this.currTheme == 'expertusoneV2') {
    				if(navigator.userAgent.indexOf("Chrome")>0 || navigator.userAgent.indexOf("Safari")>0 ){
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","25px");
    				if(navigator.userAgent.indexOf("Safari")>0){
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","12px");
				      }
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","42px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","25px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","25px");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","42px");
    				}else{
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","24px"); 
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","42px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","0px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","0");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","42px");
    				}
    					
    			$("#"+gridRowId+" > td ").css("border-bottom","solid 0px #EDEDED");
    			}
    			else
    			{
    				if(navigator.userAgent.indexOf("Chrome")>0){
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","7px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","30px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","30px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","15px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","30px");
    				}else{
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","-6px"); 
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","30px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","30px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","0");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","30px");
    				}
    			
    				$("#"+gridRowId+" > td ").css("border-bottom","solid 0px #ccc");
    			}
    		} else {			
    			//alert("open");
    			$("#"+newGridId).slideUp(1000, function() {
    				/*-- #40207 - Jscrollpane container has more course/class --*/
					$("body").data("learningcore").refreshJScrollPane('#lnr-prerequisite-container #gview_lnr-prerequisite');
    			});
    			// this.styleSwap(jgTitleId,'title_close','title_open');
				$("#"+jgTitleId).toggleText(Drupal.t('LBL713'), Drupal.t('LBL3042'));
    			if(this.currTheme == 'expertusoneV2') {
    				if(navigator.userAgent.indexOf("Chrome")>0 || navigator.userAgent.indexOf("Safari")>0 ){
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","25px");
        			if(navigator.userAgent.indexOf("Safari")>0){
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","12px");
				      }
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","42px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","25px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","25px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","42px");
        			}else{
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","24px"); 
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","42px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","0px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","0");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","42px");
        			}
        			
    				$("#"+gridRowId+" > td ").css("border-bottom","solid 1px #EDEDED");
    				$("#lnr-prerequisite").find("tr:last-child td ").css("border-bottom","solid 0px #EDEDED");
    			}
    			else
    			{
    				if(navigator.userAgent.indexOf("Chrome")>0){
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","7px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","30px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","30px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","15px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","30px");
        			}else{
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","-6px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","30px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","30px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","0");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","30px");
        			}
        		
    				$("#"+gridRowId+" > td ").css("border-bottom","solid 1px #ccc");
    				$("#lnr-prerequisite").find("tr:last-child td ").css("border-bottom","solid 0px #ccc");
    			}
    		}
		}
		vtip();
		resetFadeOutByClass('#lnr-prerequisite .tp-prereq-select-class-level-container','content-detail-code','line-item-container','select');
		}catch(e){
			// to do
			// window.console.log(e, e.stack);
		}
	},
	
	styleSwap : function(classId,addClass,removeClass) {
		try{
		$("#"+classId).removeClass(removeClass);
		$("#"+classId).addClass(addClass);
		}catch(e){
			// to do
		}
	},
	
	getTpPrereqList : function(id,divId,catalogType,preLevel) {
		try{
		var divId = ((divId == preLevel) ? 'lnr-catalog-search' : divId);
		var dispGridId, constructUrl, colNames , colModel,loadCompleteCall;
		//var dispGridId 		 = catalogType+'prereq'+id+'jgGrid'; //initPrereq
		var dispGridId 		 = catalogType+id+preLevel+'jgGrid'; //initPrereq
		
		var constructUrl 	 = this.constructUrl("learning/catalog-tpprerequisite/" + id+"/"+catalogType+"/"+divId+"/"+preLevel);
		var colNames		 = [''];
		var obj 			 = this;
		var objStr 			 = '$("#lnr-prerequisite").data("prerequisite")';
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback==true){
			var colModel		 = [ {name:'tName',index:'preReqTpDetails', title:false,'widgetObj':objStr,formatter:obj.paintPrereqResults}];
		}else{
			var colModel		 = [ {name:'tName',index:'preReqTpDetails', title:false, width:600,'widgetObj':objStr,formatter:obj.paintPrereqResults}];
		}
		
		var loadCompleteCall = obj.prereqCallback;
		this.jqGridFun(dispGridId, constructUrl, colNames , colModel,loadCompleteCall);
		vtip();
				}catch(e){
			// to do
		}
	},
	
	getTpPrereqCatalogRegister : function(tpCrsId,objectId,str) {
		try{
		var tpCrsId = tpCrsId.split('-');
		
		var tpCrsIdV, selectedClass,inc = 0,v = 0;
		var checkedClass = new Array();
		var tpCrsIdLen	 = tpCrsId.length;
		var tpAction 	= $('#tpaction-'+objectId).val();
		var tpActionV 	= tpAction.split("-");
		tpActionType 	= tpActionV[0];
		tpNodeId		= tpActionV[1];
		var tpSelectedClass = '';
		
		var params = 'selectedItem={';
		for(var n = 0; n < tpCrsIdLen ; n++) {			
			tpCrsIdV 		= tpCrsId[n];
			inc=inc+1;
			selectedClass 	= $('input[name="cls-'+tpCrsIdV+'"]:radio:checked').val();
			
			if(selectedClass != undefined) {
				checkedClass[n]	= selectedClass;
				tpSelectedClass	= selectedClass+tpSelectedClass;
			}
			checkedClass	= checkedClass
			
			if(selectedClass == undefined || selectedClass == 'undefined') {				
				selectedClass='NULL';
				break;
			}
			//if($("#cls-"+tpCrsIdV).attr("disabled") == false) {
				params += '"'+v+'":'+'{';				
				params += '"tpid":"'+objectId+'",';
				params += '"courseid":"'+tpCrsIdV+'",';
				params += '"classid":"'+selectedClass+'"';
				params += '}';
				
				if(inc < tpCrsIdLen) {
					params += ',';							
					tpSelectedClass = ','+tpSelectedClass;
				 }
				v = v+1;
			//}
			
		}
		
		params += '}';
		
		if(tpCrsIdLen != checkedClass.length) {
			var errMsgPre = new Array();
			errMsgPre[0] = str;
			var message_call = expertus_error_message(errMsgPre,'error');
			$('#show_expertus_message').html(message_call);
			$('#show_expertus_message').show();
		} else {/*
			clsaction 		= $('#clsaction-'+courseId).val();
			var clsactionV 	= clsaction.split("-");
			clsActionType 	= clsactionV[0];
			clsNodeId		= clsactionV[1];
			alert("clsaction : "+clsActionType+" | clsNodeId: "+clsNodeId);
			*/
			
			if(tpActionType == "cart") {
				this.addToCartFun("tp",tpNodeId,objectId,tpSelectedClass);
			} else {
				var isAdminSide = 'N'; 
				var userListIds = ''; 
				var fromPage = 'catalogpopup'; 
				var MasterMandatory = 'N'; 
				var url = this.constructUrl("ajax/trainingplan/class-list-register/"+isAdminSide+ "/" + userListIds+ "/" + fromPage+ "/" + MasterMandatory);		
				var obj = this;
				$.ajax({
					type: "POST",
					url: url,
					data:  params,
					datatype: 'text',
					success: function(result){
						result = $.trim(result);
						var resultErr = new Array();
						if(result == 'Registered') {
							resultErr[0] = Drupal.t("Registered Successfully");
							var message_call = expertus_error_message(resultErr,'error');
							$('#show_expertus_message').html(message_call);
							$('#show_expertus_message').show();
							
					   } else {
						   resultErr[0] = result; 
						   var message_call = expertus_error_message(resultErr,'error');
							$('#show_expertus_message').html(message_call);
							$('#show_expertus_message').show();
					   }
						
					}
			    });
			}
		} 
		}catch(e){
			// to do
		}
	},
	
	//callRegisterClass : function(widgetId,userId, courseId, classId) {
	getPrereqCatalogRegister : function(tpCrsIdv,courseId,str) {
		try{
		var tpCrsId = tpCrsIdv.split('-');
		var tpCrsIdLen = tpCrsId.length;
		var inc = 0;
		var selectedClass,clsaction;
		var clsActionType, clsNodeId;
		
		
		if(tpCrsIdLen == 1) {
			selectedClass 	= $('input[name="cls-'+tpCrsId[0]+'"]:radio:checked').val();
			if(selectedClass != undefined) {
				clsaction 		= $('#clsaction-'+selectedClass).val();
				var clsactionV 	= clsaction.split("-");
				clsActionType 	= clsactionV[0];
				clsNodeId		= clsactionV[1];
				//alert("clsaction : "+clsActionType+" | clsNodeId: "+clsNodeId);
				//addToCartFun(actionType,nodeId,courseId,classId)
				if(clsActionType == "cart" && availableFunctionalities.exp_sp_commerce != undefined && availableFunctionalities.exp_sp_commerce != "") {
				//if(availableFunctionalities.exp_sp_commerce != undefined && availableFunctionalities.exp_sp_commerce != ""){	
					
					this.addToCartFun("class",clsNodeId,tpCrsId[0],selectedClass);
				} else {
					this.prereqCatalogRegister(tpCrsId[0],selectedClass);
				}
			}
		} else {
			for(var n = 0; n < tpCrsId.length; n++) {			
				tpCrsIdV 		= tpCrsId[n];
				courseId		= tpCrsIdV;
				selectedClass 	= $('input[name="cls-'+tpCrsIdV+'"]:radio:checked').val();
				if(selectedClass != undefined) {
					//checkedClass[n]	= selectedClass;
					if($("#cls-"+courseId).attr("disabled") == false) {
						clsaction 		= $('#clsaction-'+courseId).val();
						var clsactionV 	= clsaction.split("-");
						clsActionType 	= clsactionV[0];
						clsNodeId		= clsactionV[1];
						//alert("multiple clsaction : "+clsActionType+" | clsNodeId: "+clsNodeId);
						
						if(clsActionType == "cart") {
							this.addToCartFun("class",clsNodeId,courseId,selectedClass);
						} else {
							this.prereqCatalogRegister(courseId,selectedClass);
						}
						inc=inc+1;
					}
				}
				
			}
		}
		
		if(((tpCrsIdLen == 1) && (selectedClass == undefined)) || ((tpCrsIdLen > 1) && (inc == 0))) {
			var MsgErr = new Array();
			MsgErr[0] = str;
			var message_call = expertus_error_message(MsgErr,'error');
			$('#show_expertus_message').html(message_call);
			$('#show_expertus_message').show();
		} 
			// #43510 - refresh the catalog page for button status update
			var currentPage = $('.ui-pg-input').val();
			if(typeof $("#lnr-catalog-search").data("lnrcatalogsearch") == 'undefined' || $("#lnr-catalog-search").data("lnrcatalogsearch").refreshLastAccessedCatalogRow() == false) {
				$("#lnr-catalog-search #paintContentResults").trigger("reloadGrid",[{page:currentPage}]);
			}
		}catch(e){
			// to do
		}
	},
	prereqCatalogRegister : function(courseId,classId) {
		try{
		var obj = this;
		 
		var userId = this.getLearnerId();			
		var isAdminSide = 'N';
		var waitlist = 1;
			//this.createLoader(loaderObj);
			url = obj.constructUrl("ajax/learningcore/register/" + userId + '/' + courseId + '/' + classId+'/'+waitlist+'/'+isAdminSide);
			$.ajax({
				type: "POST",
				url: url,
				//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
				result = $.trim(result);
				   /*if(userId!=0)
					   obj.callAvailableSeats(userId, courseId, classId);*/
					var preMsg = new Array();
				   if(result == 'Registered') {
					   preMsg[0] = Drupal.t("Registered Successfully");
					   var message_call = expertus_error_message(preMsg,'error');
					   $('#show_expertus_message').html(message_call);
					   $('#show_expertus_message').show();					   
					   $('input[name="cls-'+courseId+'"]:radio').attr('disabled',true);
					   
					   if(document.getElementById('clsStatus-'+courseId)) {
						   $("#clsStatus-"+courseId).html(SMARTPORTAL.t("Registered"));
						   $("#clsStatus-"+courseId).attr("title",SMARTPORTAL.t("Registered"));
					   }
					   
				   }else if(result == 'Waitlisted'){ // Waitlist Message Ticket no :  0021486
					   preMsg[0] = Drupal.t("Waitlisted Successfully");
					   var message_call = expertus_error_message(preMsg,'error');
					   $('#show_expertus_message').html(message_call);
					   $('#show_expertus_message').show();	
					   $('input[name="cls-'+courseId+'"]:radio').attr('disabled',true);
					   
					   if(document.getElementById('clsStatus-'+courseId)) {
						   $("#clsStatus-"+courseId).html(SMARTPORTAL.t("Waitlisted"));
						   $("#clsStatus-"+courseId).attr("title",SMARTPORTAL.t("Waitlisted"));
					   }
				   } 
				   else{
					   preMsg[0] = result;
					   var message_call = expertus_error_message(preMsg,'error');
					   $('#show_expertus_message').html(message_call);
					   $('#show_expertus_message').show();
					   //obj.callMessageWindow('registertitle',result);
				   }
				  // obj.destroyLoader(loaderObj);
				}
		    });
		}catch(e){
			// to do
		}
	},
	
	//addToCartFun : function(widgetId,act,ObjectId,addFrom,class_ids) {
	addToCartFun : function(actionType,nodeId,courseId,classId) {
		try{
		//var loaderObj = widgetId ;
		var waitlist = 0;
		var programId = (actionType == "tp") ? courseId : 0;
		 /*
		 if(addFrom == 'Cart'){
			var classData 	= eval($('#object-addToCartList_'+ObjectId).attr("data"));
		 }else{
			 var classData 	= eval($('#addToCartList_'+ObjectId).attr("data"));
		 }
			var LMSNodeId 	= classData.NodeId;
			var classId	 	= addFrom == 'Cart' ? classData.TpId : classData.ClassId;
			var courseId	= classData.CourseId;

			var obj = this;
			this.createLoader(loaderObj);
			*/
			//if(addFrom == 'Cart'){
			if(actionType == "tp") {
				//var class_ids = '';
				/*if(class_ids ==''){
					class_ids = 0;
				}*/
				url =  this.constructUrl("ajax/cart/tpproduct/add/" + nodeId + "/" + programId + "/" + 'null' + "/" + classId);
			}
			else{
				url =  this.constructUrl("ajax/cart/product/add/" + nodeId + "/" + classId + "/" + courseId + "/" + waitlist);
			}

			$.ajax({
				type: "POST",
				url: url,
				// data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
					//alert("result : "+result.toSource());
					
					//obj.destroyLoader(loaderObj);
					var MsgPre = new Array();
					if(result.cart_msg == 'CartAdded'){
						MsgPre [0] = Drupal.t("Added to your Cart");
						var message_call = expertus_error_message(MsgPre,'error');
						$('#show_expertus_message').html(message_call);
						$('#show_expertus_message').show();
					} else {
						MsgPre [0] = result.cart_msg;
						var message_call = expertus_error_message(MsgPre,'error');
						$('#show_expertus_message').html(message_call);
						$('#show_expertus_message').show();					
					}
					if (parseInt(result.total_product) > 0) {
						$("#ShoppingCartItemsId").addClass("filled");
						$("#ShoppingCartItemsId").html(result.total_product);
					} else {
						$("#ShoppingCartItemsId").removeClass("filled");
						$("#ShoppingCartItemsId").html('');
					}
				}
				
		    });
		}catch(e){
			// to do
		}
	},
	
	jqGridFun : function(dispGridId, constructUrl, colNames , colModel,loadCompleteCall) {
		try{
		this.createLoader(dispGridId);
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback==true){
			$("#"+dispGridId).jqGrid({
				url		 	 : constructUrl,
				datatype 	 : "json",
				mtype	 	 : 'GET',
				colNames 	 : colNames,
				colModel  	 : colModel,
				rowNum	 	 : 10,
				viewrecords  : true,
				emptyrecords : "",
				sortorder	 : "asc",
				toppager	 : true,
				height		 : 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext	 : "",
				recordtext	 : "",
				loadui		 : false,
				loadComplete : loadCompleteCall
			});
		}else{
			$("#"+dispGridId).jqGrid({
				url		 	 : constructUrl,
				datatype 	 : "json",
				mtype	 	 : 'GET',
				colNames 	 : colNames,
				colModel  	 : colModel,
				rowNum	 	 : 10,
				viewrecords  : true,
				emptyrecords : "",
				sortorder	 : "asc",
				toppager	 : true,
				height		 : 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext	 : "",
				recordtext	 : "",
				loadui		 : false,
				loadComplete : loadCompleteCall
			});
		}
		
		$("#"+dispGridId+"_toppager").css("display","none");
		$(".ui-jqgrid-hbox").css("display","none");	
		//$(".ui-jqgrid-hdiv").css("display","none");
		}catch(e){
			// to do
		}
	},
	
	prereqCallback :  function(response, postdata, formid) {
		try{
		var crsId = response.rows[0].cell.crsId;		
		if(document.getElementById('cls-'+crsId)) {
			if($('input[name="cls-'+crsId+'"]:radio').is(":checked") == true)	{
				$('input[name="cls-'+crsId+'"]:radio').attr('disabled',true);
			}
		}
		var len = response.rows.length - 1; 
		var len1 = response.rows.length - 2;
		$("tr #"+response.rows[len].id+":last > td ").css("border-bottom","0px solid");
		// When course has no class, loader icon loading issue fix.
		if (response.rows[len].id == 'coursel3' || response.total==0) {
			var parentId = $('#lnr-prerequisite .loadercontent').parent().attr('id');
		} else {
		var parentId = $("tr #"+response.rows[0].id+"").parent().parent().parent().parent().parent().attr("id");
		}
		
		$('body').data('prerequisite').destroyLoader(parentId);

		/*if(response.rows[len].cell.registerBtn == false) {
			$("tr #"+response.rows[len].id+":last ").css("border-bottom","1px solid #cccccc");
			//$("#"+parentId).css("border-bottom","1px solid #cccccc");
			
		} else if(len1 > 0){
			if($("tr #"+response.rows[len1].id+":last").next().find("table").attr("id") == undefined) {
				$("tr #"+response.rows[len1].id+":last").next().css("display","none");
				$("tr #"+response.rows[len1].id+":last > td ").css("border-bottom","0px solid #cccccc");
			}
		}*/
		//$("#gview_lnr-prerequisite").css("border-bottom","0px solid");		
		//$('body').data('prerequisite').destroyLoader("prereqCatalog"+crsId);
		$('.limit-title').trunk8(trunk8.Prerequisite_title);
		$('.limit-desc').trunk8(trunk8.Prerequisite_desc);
		$('.lmt-des-prereq-title').trunk8(trunk8.Prerequisite_title);
		/*-- #40207 - Jscrollpane container has more course/class --*/
		$("body").data("learningcore").refreshJScrollPane('#lnr-prerequisite-container #gview_lnr-prerequisite');
		//$('.fade-out-title-container.assignlearn-tp-cls-session-title-fadeout-container').css('max-width','500px');
		vtip();
		resetFadeOutByClass('#lnr-prerequisite .tp-prereq-select-class-level-container','content-detail-code','line-item-container','select');
		resetFadeOutByClass('#lnr-prerequisite .tp-prereq-select-course-level-container','content-detail-code','line-item-container','select');
	
		}catch(e){
			// to do
			// console.log(e, e.stack);
		}
	},
	
	getPreqClassId : function(classId,crsId) {
		try{
		var seatsleft  = $("#hidden-seatleft-cls-"+classId).html();
		    $("#seatleftcount"+crsId).html(seatsleft);
		}catch(e){
			// to do
		}
	},
	showClassDetails : function(clsId) {
		try{
		var oHtml = '';
		var currTr = 'preReqClassDetils'+clsId;
		
		if(document.getElementById(currTr)) {
			var data = eval($("#"+currTr).attr("data"));
			var dataInfo = data;
			var desc = data.description; 
			oHtml += "<table cellpadding='2' cellspacing='2' width='100%'>";
			if(data.cls_code) {
				oHtml += "<tr><td class='class-code-heading'>"+SMARTPORTAL.t("Class Code")+": "+data.cls_code+"</td></tr>";
			}
			oHtml += "<tr><td class='cls-description'>"+desc+"</td></tr><tr><td class='cls-description'>";
			if(dataInfo.sessionDetails.length>0) {
				oHtml += '<div class="enroll-session-details">'+SMARTPORTAL.t("Session Details")+':</div>';
				var inc = 1;
				$(dataInfo.sessionDetails).each(function(){ //sessionDate sessionDay
					if($(this).attr("sessionTitle")) {
						var sesionsH = ($(this).attr("sessionTitle") != '') ? $(this).attr("sessionTitle") : "&nbsp;"; 
						var sesionsHead = titleRestricted(sesionsH,15);
						
						oHtml += '<div class="sessionDet"><div class="sessName vtip" title="'+(($(this).attr("sessionTitle") != '') ? $(this).attr("sessionTitle") : "&nbsp;")+'">'+sesionsHead+'</div>';
						oHtml += '<div class="sessDay">'+$(this).attr("sessionDay")+'</div>';
						oHtml += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span></div></div>";
						inc++;
					}
				});
			}
			if(data.language) {
				oHtml += "<div class='cls-lang'>"+SMARTPORTAL.t("Language")+": "+data.language+"</div>";
			}
			var LocationName 	= dataInfo.locationDetails.locationName; 
			if(LocationName != '') {
				oHtml +='<table border="0" cellpadding="0" class="enroll-loc-details" cellspacing="0"><tr><td class="enroll-location-head" valign="top"><div class="enroll-loc-head">'+SMARTPORTAL.t("Location")+':</div></td><td></td></tr><tr><td>';
				
				oHtml += "<div class='enroll-location-text'>"+LocationName+"</div>";
				if(dataInfo.locationDetails.locationAddr1 !='' && dataInfo.locationDetails.locationAddr1 != null) {
					oHtml += "<div class='enroll-location-text'>"+dataInfo.locationDetails.locationAddr1+"</div>";
				}
				if(dataInfo.locationDetails.locationAddr2 !='' && dataInfo.locationDetails.locationAddr2 != null) {
					oHtml += "<div class='enroll-location-text'>"+dataInfo.locationDetails.locationAddr2+"</div>";
				}
				if(dataInfo.locationDetails.locationCity !='' && dataInfo.locationDetails.locationCity != null) {
					oHtml += "<div class='enroll-location-text'>"+dataInfo.locationDetails.locationCity;
					if (dataInfo.locationDetails.locationState == '' && dataInfo.locationDetails.locationState != null) {
						oHtml += "<br />";
					}
				}
				if(dataInfo.locationDetails.locationState !='' && dataInfo.locationDetails.locationState != null) {
					if (dataInfo.locationDetails.locationCity != '' && dataInfo.locationDetails.locationCity != null) {
						oHtml += " , ";
					}
					oHtml += dataInfo.locationDetails.locationState+"</div>";
				}
				if(dataInfo.locationDetails.locationZipcode !='' && dataInfo.locationDetails.locationZipcode != null){
					oHtml += "<div class='enroll-location-text'>zip"+dataInfo.locationDetails.locationZipcode+"</div>";
				}

				oHtml += "</td></tr></table>";
			}
		
			oHtml += "</td></tr></table>"; 
			$("#"+clsId+"clsDesc").html(oHtml);
		}
		vtip();
		}catch(e){
			// to do
		}
	},
	
	
	
	});

$.extend($.ui.prerequisite.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);

$(function() {
	try{
	$("body").prerequisite();
	}catch(e){
		// to do
	}
});