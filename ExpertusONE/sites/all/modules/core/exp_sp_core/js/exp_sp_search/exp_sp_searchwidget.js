registernamespace("EXPERTUS.SMARTPORTAL.SearchManager");
EXPERTUS.SMARTPORTAL.SearchManager = function() {
	try{
	this.generateSimpleSearchRequest = function(obj,type,cobj,searchCallObj) {
		try{
		//alert(searchCallObj);
		var request = new Object();
		request.query = new SOAPObject("SearchRequest")
				.attr("xsi:type", "null");
		if(type != undefined){
			request.query.appendChild(new SOAPObject("Type").attr("xsi:type", "null").val(type));
		}
		if(obj != undefined){
			var subqry = new Object();
			subqry.query = new SOAPObject("SearchKey").attr("xsi:type", "null");
			subqry.query.appendChild(new SOAPObject("name")
					.attr("xsi:type", "null").val(obj.key));
			subqry.query.appendChild(new SOAPObject("value").attr("xsi:type",
					"null").val(obj.value));
			subqry.query.appendChild(new SOAPObject("searchCallType")
					.attr("xsi:type", "null").val(searchCallObj));
			
			request.query.appendChild(subqry.query);
		}

		/* Changed for Advanced Seachr Widget Plugin*/
		if(cobj.currentWidgetObj!=undefined && cobj.currentWidgetObj != '' && cobj.currentWidgetObj != 'undefined') {
			if(typeof (eval(cobj.currentWidgetObj).generateAdvanceRequest)=='function'){
				var adops=eval(cobj.currentWidgetObj).generateAdvanceRequest();
				request.query.appendChild(adops);
			}
		}else{
			if(typeof (eval(cobj.parentWidgetObj).generateAdvanceRequest)=='function'){
				var adops=eval(cobj.parentWidgetObj).generateAdvanceRequest();
				request.query.appendChild(adops);
			}
		}
		
		/* ----------------- */

		opts='';
		$('.faqOptionCheckbox').each(function(){
			if($(this).attr("checked")){
				opts+=$(this).val();
				opts+=',';
			}
		});
		opts=opts.length>0 ? opts.substring(0, opts.length-1):opts;
		subqry = new Object();
		subqry.query = new SOAPObject("FaqSearchKey").attr("xsi:type", "null");
		subqry.query.appendChild(new SOAPObject("opts")
		.attr("xsi:type", "null").val(opts));
		request.query.appendChild(subqry.query);


		var sr = new SOAPRequest("SearchRequest", request);
		sr = "<?xml version='1.0' encoding='UTF-8'?>" + sr;
		sr = sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};

	this.generateAdvancedSearchRequest = function(obj) {
		try{
		var request = new Object();
		request.query = new SOAPObject("AdvancedSearchOptsRequest")
				.attr("xsi:type", "null");
		request.query.appendChild(new SOAPObject("type")
		.attr("xsi:type", "null").val(obj));
		var sr = new SOAPRequest("AdvancedSearchOptsRequest", request);
		sr = "<?xml version='1.0' encoding='UTF-8'?>" + sr;
		sr = sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};

	this.generateFAQSearchRequest = function(obj) {
		try{
		var request = new Object();
		request.query = new SOAPObject("FAQSearchOptsRequest")
				.attr("xsi:type", "null");
		request.query.appendChild(new SOAPObject("type")
		.attr("xsi:type", "null").val(obj));
		var sr = new SOAPRequest("FAQSearchOptsRequest", request);
		sr = "<?xml version='1.0' encoding='UTF-8'?>" + sr;
		sr = sr.toString();
		return sr;
		}catch(e){
			// to do
		}
	};
	}catch(e){
		// to do
	}
};
(function($) {
	$.extend(EXPERTUS.SMARTPORTAL.SearchManager.prototype,
			EXPERTUS.SMARTPORTAL.AbstractManager);
})(jQuery);
registernamespace("EXPERTUS.SMARTPORTAL.SearchWidget");
EXPERTUS.SMARTPORTAL.SearchWidget = function() {
	try{
	this.mode = 'searchresults';
	this.widgetGlobalName = "EXPERTUS.SMARTPORTAL.SearchWidget";

	this.searchTitle = (Drupal.t("LBL304")).toUpperCase();
	this.searchParams = [];
	this.searchParamsDivId = '';
	this.searchConfig = {
		"advanced" :config.data[4].SearchConfig.advanced,
		"faq" :config.data[4].SearchConfig.faq,
		"CategoryBrowse" :config.data[4].SearchConfig.CategoryBrowse
	};
	this.addOptions;
	this.customOptions;
	this.faqXml=null;

	this.searchResults = [];
	this.searchResultsDivId = '';
	this.tblConfig={
			'sort':true,
			'sortOptions':[],
			'filter':true,
			'filterOptions':[],
			'print':true,
			'exportTbl':true,
			'pageLength':5
		};

	this.searchWidgetId;
	this.parentWidgetObj;
	this.searchServiceWidgetId = '';

	this.searchType='';
	this.lastSearchQry='';
	this.qrystring='';
	this.printCols;
	this.exportCols;
	this.callerType;
	this.searchCallType='';

	this.setSearchWidgetId=function(id){
		try{
		this.searchWidgetId = this.getUniqueWidgetId()+id;
		}catch(e){
			// to do
		}
	};

	this.getSearchWidgetId=function(){
		try{
		return this.searchWidgetId;
		}catch(e){
			// to do
		}
	};

	this.setSearchType=function(){
		try{
		var type='normal';
		var advOn='';
		var faqOn='';
		$('.advOptionSelect').each(function(){
			var val=$(this).val();
			if(val != undefined && val != ''){
				advOn=',adv';
			}
		});

		$('.faqOptionCheckbox').each(function(){
			if($(this).attr("checked")){
				faqOn=',faq';
			}
		});
		type+=type+advOn+faqOn;
		this.searchType=type;
		}catch(e){
			// to do
		}
	};


	this.render = function(serviceattributes) {
		try{
		this.renderBase(serviceattributes, this);
		var id = Math.floor(Math.random() * 100);
		this.setSearchWidgetId(id);
		///alert("function___"+this.searchTitle);
		$('#' + this.searchParamsDivId).expertusSearch(
				this.getSearchWidgetId(),this.searchTitle, this.addOptions, this.searchParams, this.searchConfig,this.customOptions);
		$("#contentGuide").showHelpPage("screen",this.searchTitle);
		$('#' + this.getSearchWidgetId() + '_searchtext').autocomplete(
				resource.service_url, {
					minChars :3,
					max :50,
					autoFill :true,
					mustMatch :false,
					matchContains :false,
					inputClass :"ac_input",
					loadingClass :"ac_loading"
				});
		var entityType	= '';
		if(this.searchParams[0].entityType=='' || this.searchParams[0].entityType=='undefined' || this.searchParams[0].entityType==undefined){
			entityType = this.searchCallType;
		}else{
			entityType = this.searchParams[0].entityType;
		}
		$.fn.expertusAutocomplete(
				this.getSearchWidgetId() + '_searchtext',
				this.searchParams[0].autocomplete_type,
				this.searchParams[0].key,
				this.searchParams[0].formatIndex,
				entityType
		);


		$('#' + this.getSearchWidgetId() + '_simplesearch').bind('click', {
			'searchWidget' :this
		}, function(e) {
			e.data.searchWidget.simpleSearchAction();
		});

		var advOptHtml = '';
		if (this.searchConfig.advanced) {
			$('#' + this.getSearchWidgetId() + '_advancedsearch').bind('click',
					{
						'searchWidget' :this
					}, function(e) {
						e.data.searchWidget.advancedSearchAction();
					});
		}
		var browseOptHtml = '';
		if (this.searchConfig.CategoryBrowse) {
			$('#' + this.getSearchWidgetId() + '_categorybrowse').bind('click',
					{
						'searchWidget' :this
					}, function(e) {
						e.data.searchWidget.categorySearchAction();
					});
		}

		var faqOptHtml = '';
		if (this.searchConfig.faq) {
			$('#' + this.getSearchWidgetId() + '_faqsearch').bind('click', {
				'searchWidget' :this
			}, function(e) {
				e.data.searchWidget.faqSearchAction();
			});
		}

		if (this.searchConfig.customBtn) {
			$('#' + this.getSearchWidgetId() + '_custonbtn').bind('click',
					{
						'searchWidget' :this
					}, function(e) {
						e.data.searchWidget.customBtnAction();
					});
		}

		if (this.searchConfig.savequery) {
			$('#' + this.getSearchWidgetId() + '_savequery').bind('click', {
				'searchWidget' :this
			}, function(e) {
				e.data.searchWidget.saveQueryAction();
			});

			$('#' + this.getSearchWidgetId() + '_findsavequery').bind('click', {
				'searchWidget' :this
			}, function(e) {
				e.data.searchWidget.searchSaveQuery();
			});
		}

		if (this.searchConfig.advanced) {
			$('#' + this.getSearchWidgetId() + '_searchtable')
					.after(advOptHtml);
		}

		if (this.searchConfig.faq) {
			$('#' + this.getSearchWidgetId() + '_searchtable')
			.after(faqOptHtml);
			$('#' + this.getSearchWidgetId() + '_newsearch')
					.bind(
							'click',
							{
								'searchWidget' :this
							},
							function(e) {
								$(
										'#' + e.data.searchWidget
												.getSearchWidgetId() + '_faqsearchopts')
										.hide();
								$(
										'#' + e.data.searchWidget
												.getSearchWidgetId() + '_advsearchopts')
										.hide();
								$(
										'#' + e.data.searchWidget
												.getSearchWidgetId() + '_newsearchopts')
										.fadeIn();
								$('.searchadv').fadeIn();
								$('.searchfaq').fadeIn();

							});

			$('#' + this.getSearchWidgetId() + '_faqsearchbtn')
			.bind(
					'click',
					{
						'searchWidget' :this
					},
					function(e) {
						e.data.searchWidget.faqPerformSearchAction();
					});
		}

		// Old
	/*	$('#' + this.searchParamsDivId).append('<div class="sp_rowseparator" id="'+this.searchParamsDivId+'AdFaq"></div>');
		//add padding to search div
		$('#' + this.searchParamsDivId).css("padding-bottom",'4px');
		var searchHtml = "<div class='splitter'>&nbsp;</div>";
		searchHtml += "<div id='"+this.getSearchWidgetId()+"_advancedsearchDiv' style='display:none;'></div>";
		$('#' + this.searchParamsDivId+"AdFaq").after(searchHtml);
		searchHtml = "<div id='"+this.getSearchWidgetId()+"_faqsearchDiv' style='display:none;left:256px;'>";
		searchHtml += "</div>";
		$('#' + this.searchParamsDivId+"AdFaq").after(searchHtml);
*/
		// New
	/*	var searchHtml = "<div class='splitter'>&nbsp;</div>";
		searchHtml += "<div id='"+this.getSearchWidgetId()+"_advancedsearchDiv' style='display:none;'></div>";
		$('#' + this.getSearchWidgetId()+"_advsearchdisplay").after(searchHtml);
		searchHtml = "<div id='"+this.getSearchWidgetId()+"_faqsearchDiv' style='display:none;left:256px;'>";
		searchHtml += "</div>";
		$('#' + this.getSearchWidgetId()+"_advsearchdisplay").after(searchHtml);
		*/
		$('#' + this.getSearchWidgetId()+"_advsearchdisplay").append('<div id="'+this.searchParamsDivId+'AdFaq"></div>');
		var searchHtml = "<div class='splitter'>&nbsp;</div>";
		searchHtml += "<div id='"+this.getSearchWidgetId()+"_advancedsearchDiv' style='display:none;'></div>";
		$('#' + this.searchParamsDivId+"AdFaq").after(searchHtml);
		searchHtml = "<div id='"+this.getSearchWidgetId()+"_faqsearchDiv' style='display:none;left:256px;'>";
		searchHtml += "</div>";
		$('#' + this.searchParamsDivId+"AdFaq").after(searchHtml);
		}catch(e){
			// to do
		}
	};
	this.categorySearchAction = function() {
		try{
		alert('Under Construction');
		}catch(e){
			// to do
		}
	};
	this.simpleSearchAction = function() {
		try{
		this.mode = 'searchresults';
		var DCMgr = new EXPERTUS.SMARTPORTAL.SearchManager();
		DCMgr.initialize(this);
		DCMgr.setActionKey(eval('config.data[1].serviceaction[0].' + this.searchServiceWidgetId));
		var url = this.getEndPointURL();
		DCMgr.setEndPointURL(url);
		DCMgr.url=DCMgr.url + ((this.qrystring != undefined)?'&searchqry='+this.qrystring:'');
		var param;
		var value = $('#' + this.getSearchWidgetId() + '_searchtext').val();
		//if(value != undefined && value.length> 0){
		if(value != undefined){
			var key = '';
			if($('.' + this.getSearchWidgetId() + '_searchparam').length>1){
				for ( var i = 0; i < $('.' + this.getSearchWidgetId() + '_searchparam').length; i++) {
					if ($('.' + this.getSearchWidgetId() + '_searchparam')[i].checked) {
						key = $('.' + this.getSearchWidgetId() + '_searchparam')[i].value;
					}
				}
			}else{
				key=$('.' + this.getSearchWidgetId() + '_searchparam').val();
			}
			param = {
				'widgetId' : this.getSearchWidgetId(),
				'key' :key,
				'value' :value
			};
		}

		this.lastSearchQry = DCMgr.generateSimpleSearchRequest(param,this.callerType,this,this.searchCallType);

		//set the lastSearchQry in the calling widget too
		if(eval(this.parentWidgetObj).lastSearchQry != undefined){
			eval(this.parentWidgetObj).lastSearchQry = this.lastSearchQry;
		}

		DCMgr.requestXml = this.lastSearchQry;
		DCMgr.setCallBack("renderSearchResults");
		
		/* Changed for Advanced Seachr Widget Plugin*/
		if(this.currentWidgetObj!=undefined && this.currentWidgetObj != '' && this.currentWidgetObj != 'undefined'){
			var pob = eval(this.currentWidgetObj).getUniqueWidgetId();
		}else{
			var pob = eval(this.parentWidgetObj).getUniqueWidgetId();			
		}
		var lodob = this.searchParamsDivId.substring(pob.length);
		DCMgr.setLoaderObj(lodob);
		/* ---------------------*/
		//set the search type
		this.searchType='normal';
		this.setSearchType();
		DCMgr.execute();
		}catch(e){
			// to do
		}
	}; 

	this.refreshSimpleSearch = function(lastQry,pageNo){
		try{
		this.lastSearchQry = lastQry;
		var DCMgr = new EXPERTUS.SMARTPORTAL.SearchManager();
		DCMgr.initialize(this);
		DCMgr.setActionKey(eval('config.data[1].serviceaction[0].' + this.searchServiceWidgetId));
		 var url = this.getEndPointURL();
         DCMgr.setEndPointURL(url);
		DCMgr.requestXml = this.lastSearchQry;
		DCMgr.setCallBack("renderSearchResults",{pageNo:pageNo});
		//var pob = eval(this.parentWidgetObj).getUniqueWidgetId();
		 /* Changed for Advanced Seachr Widget Plugin*/

            if(this.currentWidgetObj!=undefined && this.currentWidgetObj != '' && this.currentWidgetObj != 'undefined'){
                  var pob = eval(this.currentWidgetObj).getUniqueWidgetId();
            }else{
                  var pob = eval(this.parentWidgetObj).getUniqueWidgetId();               
            }
		var lodob = this.searchParamsDivId.substring(pob.length);
		DCMgr.setLoaderObj(lodob);
		//set the search type
		this.searchType='normal';
		this.setSearchType();
		DCMgr.execute();
		}catch(e){
			// to do
		}
	};

	this.renderSearchResults = function (refreshObj){
		try{
		var widObj = this.getWidgetInstanceName();
		var tblXMLStr = this.responseText;
		var tblXMLObj=this.responseXml;
		var colNames = [
		     {"title" : " ", "sort" : false, "filter": false},
		     {"title" : " ", "sort" : false, "filter": false}
		];
		var colModel = [
			{
				'width':'445px',
				'align' :'left',
				'searchWidget':this.parentWidgetObj,
				formatter :this.renderResultItemDetails
			},
			{
				'width':'180px',
				'align' :'left',
				'searchWidget':this.parentWidgetObj,
				formatter :this.renderResultItemActions
			}
		];

		var aTblDefn = {
			datatype :"xmlstring",
			datastr :tblXMLStr,
			dataobj:tblXMLObj,
			parentWidgetObj: this.parentWidgetObj,
			widgetObjName:this.getObjectName(),
			searchReqXml:escape(this.lastSearchQry),
			xmlReader : {
				root :"Items",
				row :"Item",
				repeatitems :false
			},
			colNames :colNames,
			colModel :colModel,
			height :'auto',
			width :this.tblConfig.sort ||this.tblConfig.filter ? '560' : '690',
			printCols:this.printCols,
			exportCols:this.exportCols
		};
		var tblId = this.searchResultsDivId + 'Tbl';
		//divide the results div into two divs
		if(this.tblConfig.sort||this.tblConfig.filter){
			var tblOpsDiv = '<div id="'+tblId+'OpsDiv" class="spSearchResultsOps" ></div>';
			var tblDiv = '<div id="'+this.searchResultsDivId+'DataTblDiv" style="margin-left:110px" ></div>';
			$('#' + this.searchResultsDivId).html(tblOpsDiv+tblDiv);
		} else {
			var tblDiv1 = '<div id="'+this.searchResultsDivId+'DataTblDiv"></div>';
			$('#' + this.searchResultsDivId).html(tblDiv1);
		}

		var resHtml = "<table cellpadding=\"0\" cellspacing=\"0\" style='overflow:hidden' border=\"0\" id=\""
				+ tblId + "\"></table>";

		$('#' + this.searchResultsDivId + 'DataTblDiv').html(resHtml);
		$('#' + tblId).expertusDataTable(aTblDefn, this.tblConfig);
		$('#' + this.searchResultsDivId + 'DataTblDiv').css('margin-left', '0px');

		if(this.tblConfig.sort||this.tblConfig.filter){
			$('#'+tblId+'OpsDiv').hide();
			var html = '';
			if(this.tblConfig.sort) {
				html = '<fieldset style="border:#cccccc 1px solid;">';
				html += '<legend style="color:#1D4E5F;" align="center">'+SMARTPORTAL.t("Sort By")+'</legend>';
				html += this.renderSortBlock();
				html += '</fieldset>';
			}
			if(this.tblConfig.filter) {
				html += '<fieldset style="border:#cccccc 1px solid;">';
				html += '<legend style="color:#1D4E5F;" align="center">'+SMARTPORTAL.t("Filter By")+'</legend>';
				html += this.renderFilterBlock();
				html += '</fieldset>';
			}
			$('#'+this.searchResultsDivId +'TblOpsDiv').html(html);
			var searchObj = this;
			$('.spTblSortOpts :checkbox').click(function(){
				var sortStr='';
				$('.spTblSortOpts :checkbox').each(function(){
					if($(this).attr("checked")){
						sortStr+=sortStr == '' ? $(this).val() : ','+$(this).val();
					}
				});
				searchObj.refreshAction(sortStr);
			});
		}


		$('#'+tblId+' > tbody > tr:first > td:first').css('width','100%');
		//$('#'+tblId+' > tbody > tr:first > td:last').css('width','21%');

		$('.ItemStats').each(function(){
			var nextObj = $(this).next('.ItemStatsDiv:first');
			var obj = $(this);
			$(this).click(function(){
				$(nextObj).slideToggle();
			});
		});
		if(refreshObj != undefined && refreshObj != ''){
			 var refreshObjVal = eval(refreshObj);
			 $('#' + tblId).showPage(refreshObjVal.pageNo);
		 }
		}catch(e){
			// to do
		}
	};

	this.renderSortBlock = function (){
		try{
		//var sortBy = '<div class="spTblSortOptsHeader" style="float:top;">'+SMARTPORTAL.t("Sort By:")+'</div>';
		var sortOpts = '<div class="spTblSortOpts">';
		for(var i=0;i<this.tblConfig.sortOptions.length;i++){
			sortOpts += '<div style="padding:2px;vertical-align:middle;"><input type="checkbox" name="spSortOpts" value="'+this.tblConfig.sortOptions[i].value+'"></input>';
			sortOpts += this.tblConfig.sortOptions[i].name + '</div>';
		}

		sortOpts += '</div>';
		//return '<tr><td>' + sortBy + sortOpts + '</td></tr>';
		return sortOpts;
		}catch(e){
			// to do
		}
	};

	this.renderFilterBlock = function(){
		try{
		//var filterBy = '<div class="spTblFilterOptsHeader">'+SMARTPORTAL.t("Filter By:")+'</div>';
		var filterOpts = '<div class="spTblFilterOpts">';
		for(var i=0;i<this.tblConfig.filterOptions.length;i++){
			filterOpts += '<div style="padding:2px;vertical-align:middle;"><input type="checkbox" name="spFilterOpts" value="'+this.tblConfig.filterOptions[i].value+'"></input>';
			filterOpts += this.tblConfig.filterOptions[i].name + '</div>';
		}
		filterOpts +='</div>';
		//return '<tr><td>' + filterBy + filterOpts + '</td></tr>';
		return filterOpts;
		}catch(e){
			// to do
		}
	};

	this.renderResultItemDetails = function(el,cellval,options,rowObj){
		try{
		var obj = options.colModel.searchWidget;
		//convert the other details to Json
		var val = "";
		var rowId = '';
		$(rowObj).children().each(function(){
			var nodeName=$(this).get(0).nodeName;
			if(nodeName == 'Id') rowid = $(this).text();
			if(nodeName != 'Actions' && nodeName != 'Action' && nodeName != 'Parameters'){
				val=val == "" ? "{" : val+",";
				val+="\""+nodeName+"\"";
				val+=":";
				val+="\""+escape($(this).text())+"\"";
			}
		});
		val+=",\"widgetObject\":\""+obj+"\"";
		val+="}";

		var delegateFn=obj+'.'+'renderResultItemDetails(\''+val+'\')';
		var html = eval(delegateFn);
		$(el).html(html);
		//$(el).css('background-color','white');
		}catch(e){
			// to do
		}
	};

	this.renderResultItemActions = function (el,cellval,options,rowObj){
		try{
		//var thisObj	= this;
		var obj = options.colModel.searchWidget;
		var val = "";
		var rowId = '';
		$(rowObj).children().each(function(){
			var nodeName=$(this).get(0).nodeName;
			if(nodeName == 'Id') rowid = $(this).text();
			if(nodeName != 'Actions' && nodeName != 'Action' && nodeName != 'Parameters'){
				val=val == "" ? "{" : val+",";
				val+="\""+nodeName+"\"";
				val+=":";
				val+="\""+escape($(this).text())+"\"";
			}
		});
		val+=",\"widgetObject\":\""+obj+"\"";
		val+="}";

		var actionArr = '{"Actions" : [';
		var index = 0;
		$("Actions>Action",rowObj).each(
				function() {
					var action='';
					var type = $(this).attr('type');
					if (type == 'Ajax') {
						var actionNm = $(this).attr('name');
						var param = $('Parameters',this).text();
						param = eval(obj).Trim(param);
						param=escape(obj+'.'+param);
						action = '{ ';
						action += '\"name\"' + ':\"' + actionNm + '\"';
						action += ',';
						action += '\"param\"' + ':\"' + param + '\"';
						action += ' }';
						actionArr += actionArr == '{"Actions" : [' ? action : ',' + action;
					}
				});
		actionArr+=']}';
		var delegateFn=obj+'.'+'renderResultItemActions(\''+actionArr+'\', \''+val+'\')';
		var html = eval(delegateFn);
		$(el).html(html);
		//$(el).css('background-color','white');
		}catch(e){
			// to do
		}
	};

	this.customBtnAction = function(){
		try{
			var adCallback=this.searchConfig.customBtnDt.script;
			if(adCallback!=undefined && adCallback !=''){
				eval(this.parentWidgetObj+'.'+adCallback+'()');
			}
		}catch(e){
			// to do
		}
	};

	this.advancedSearchAction = function() {
		try{
		var ostr = "";		
		if(document.getElementById(this.getSearchWidgetId()+"_advsearchopts_content")==null){
			$('#'+this.getSearchWidgetId()+'_advancedsearch').css('font-weight','bold');
			ostr += "<div style='float: left; width: 98%; padding: 1%;'>";
			ostr += "<div class='splitter'>&nbsp;</div>";
			ostr += "<div id='"+this.getSearchWidgetId()+"_advsearchopts_content'></div>";
			ostr += "<div id='"+this.getSearchWidgetId()+"_savequeryDiv' class='saveQuery'></div>";
			ostr += "</div>";
			ostr += '<div class="sp_rowseparator"></div>';

			$('#'+this.getSearchWidgetId()+'_faqsearchDiv').hide();
			$('#'+this.getSearchWidgetId()+'_faqsearchopts_content').remove();
			$('#'+this.getSearchWidgetId()+'_advancedsearchDiv').html(ostr);
			/* COMMENTED BY RANJANI - SAVE AND RETREIVE QUERY NEED NOT BE DISPLAYED*/
			/*var params={
					parentObj:this.getWidgetInstanceName(),
					divid:this.getSearchWidgetId()+'_savequeryDiv',
					type:this.searchConfig.type,
					isfromSearch:true
			};
			callsavequerydetail(params);*/

			$('#'+this.getSearchWidgetId()+'_advancedsearchDiv').show();

			$('#' + this.getSearchWidgetId() + '_advancedsearch_close').bind('click', {
				'searchWidget' :this
			}, function(e) {
				$('#'+e.data.searchWidget.getSearchWidgetId()+'_advancedsearchDiv').hide();
			});

			var adCallback=this.searchConfig.script;
			if(adCallback!=undefined && adCallback !=''){
				var divid=this.getSearchWidgetId()+"_advsearchopts_content";
				
				/* Changed for Advanced Seachr Widget Plugin*/
				if(this.currentWidgetObj!=undefined && this.currentWidgetObj != '' && this.currentWidgetObj != 'undefined') {
					ostr += eval(this.currentWidgetObj+'.'+adCallback+'("'+divid+'")');
				}else{
					ostr += eval(this.parentWidgetObj+'.'+adCallback+'("'+divid+'")');
				}
				/* ----------------------- */
				
			}
		}else{
			$('#'+this.getSearchWidgetId()+'_advancedsearch').css('font-weight','normal');
			$('#'+this.getSearchWidgetId()+'_advancedsearchDiv').hide();
			$('#'+this.getSearchWidgetId()+'_advsearchopts_content').remove();
		}
		}catch(e){
			// to do
		}
	};

	this.faqSearchAction = function() {
		try{
		alert("Under Construction");
		// Commented because of sending request since functionality not yet build
		/*
		var ostr = "";
		if(document.getElementById(this.getSearchWidgetId()+"_faqsearchopts_content")==null){
			ostr += "<div>";
			ostr += "<div id='"+this.getSearchWidgetId()+"_faqsearchopts_content'>";
			ostr += "</div>";
			ostr += "</div>";
			ostr += '<div class="sp_rowseparator"></div>';

			$('#'+this.getSearchWidgetId()+'_advancedsearchDiv').hide();
			$('#'+this.getSearchWidgetId()+'_advsearchopts_content').remove();
			$('#'+this.getSearchWidgetId()+'_faqsearchDiv').html(ostr);
			$('#'+this.getSearchWidgetId()+'_faqsearchDiv').show();

			$('#' + this.getSearchWidgetId() + '_faqsearch_close').bind('click', {
				'searchWidget' :this
			}, function(e) {
				$('#'+e.data.searchWidget.getSearchWidgetId()+'_faqsearchDiv').hide();
			});
			if(this.faqXml==null){
				var adtype = this.searchConfig.type;
				var DCMgr = new EXPERTUS.SMARTPORTAL.SearchManager();
				DCMgr.initialize(this);
				DCMgr.setActionKey(eval('config.data[1].serviceaction[0].'+this.searchServiceWidgetId));
				DCMgr.setEndPointURL(this.getEndPointURL());
				DCMgr.setCallBack("faqRenderCallback");
				DCMgr.setLoaderObj('Search');
				DCMgr.requestXml = DCMgr.generateFAQSearchRequest(adtype);
				DCMgr.execute();
			}else{
				this.faqRenderCallback();
			}
		}else{
			$('#'+this.getSearchWidgetId()+'_faqsearchDiv').hide();
			$('#'+this.getSearchWidgetId()+'_faqsearchopts_content').remove();
		}
		*/
		}catch(e){
			// to do
		}
	};

	this.refreshAction=function(sortStr){
		try{
		var DCMgr = new EXPERTUS.SMARTPORTAL.SearchManager();
		DCMgr.initialize(this);
		DCMgr.setActionKey(eval('config.data[1].serviceaction[0].'+this.searchServiceWidgetId));
		DCMgr.setEndPointURL(this.getEndPointURL());
		DCMgr.url=DCMgr.url + (sortStr != undefined ?'&sortStr='+sortStr:'');
		DCMgr.setCallBack("renderRefreshResults");
		DCMgr.requestXml=unescape(this.lastSearchQry);
		this.mode='searchresults';
		this.searchType='normal';
		this.setSearchType();
		DCMgr.execute();
		}catch(e){
			// to do
		}
	};

	this.renderRefreshResults=function(){
		try{
		var widObj = this.getWidgetInstanceName();
		var tblXMLStr = this.responseText;
		var tblXMLObj=this.responseXml;
		var colNames = [
		     {"title" : " ", "sort" : false, "filter": false},
		     {"title" : " ", "sort" : false, "filter": false}
		];
		var colModel = [
			{
				'width':'545px',
				'align' :'left',
				'searchWidget':this.parentWidgetObj,
				formatter :this.renderResultItemDetails
			},
			{
				'width':'100px',
				'align' :'left',
				'searchWidget':this.parentWidgetObj,
				formatter :this.renderResultItemActions
			}
		];

		var aTblDefn = {
			datatype :"xmlstring",
			datastr :tblXMLStr,
			dataobj:tblXMLObj,
			parentWidgetObj: this.parentWidgetObj,
			widgetObjName:this.getObjectName(),
			searchReqXml:escape(this.lastSearchQry),
			xmlReader : {
				root :"Items",
				row :"Item",
				repeatitems :false
			},
			colNames :colNames,
			colModel :colModel,
			height :'auto',
			width :this.tblConfig.sort ||this.tblConfig.filter ? '655' : '775',
			printCols:this.printCols,
			exportCols:this.exportCols
		};
		var tblId = this.searchResultsDivId + 'Tbl';

		var resHtml = "<table cellpadding=\"0\" cellspacing=\"0\" style='overflow:hidden' border=\"0\" id=\""
				+ tblId + "\"></table>";
		$('#' + this.searchResultsDivId + 'DataTblDiv').html(resHtml);
		$('#' + tblId).expertusDataTable(aTblDefn, this.tblConfig);
		$('#' + this.searchResultsDivId + 'DataTblDiv').css('margin-left', '110px');

		$('#'+tblId+' > tbody > tr:first > td:first').css('width','90%');
		$('#'+tblId+' > tbody > tr:first > td:last').css('width','10%');

		$('.ItemStats').each(function(){
			var nextObj = $(this).next('.ItemStatsDiv:first');
			var obj = $(this);
			$(this).click(function(){
				$(nextObj).slideToggle();
			});
		});
		}catch(e){
			// to do
		}
	};

	/*********** AJAX Callbacks - start ***************************/
	this.faqRenderCallback=function(){
		try{
		if(this.faqXml==null)
			this.faqXml=this.responseXml;
		if($(this.responseXml).find("Items>Item").size()==0)
		{
			alert("Under Construction");
			$('#'+this.getSearchWidgetId()+'_faqsearchDiv').hide();

		}else{
		var faqHtml='';

		faqHtml+='<table cellpadding="0" cellspacing="0" style="width:100%;padding-left:5px;" >';
		var count=0;
		var optionIndex=1;
		$("Items>Item",this.faqXml).each(function(){
			faqHtml+='<tr>';
			faqHtml+='<td>';
			faqHtml+='<div style="float:left;"><input class="faqOptionCheckbox" type="radio" value="'+optionIndex+'" name="faqOption" id="faqOption"></input></div>';
			optionIndex=optionIndex+1;
			faqHtml+='<div style="padding-top:3px;"><label for="faqOption" style="cursor:pointer;">'+$('Name', this).text()+'</label></div>';
			faqHtml+='</td>';
			faqHtml+='</tr>';
		});
		faqHtml+='</table>';

		$('#'+this.getSearchWidgetId() + '_faqsearchopts_content').html(faqHtml);
         }
		}catch(e){
			// to do
		}
	};

	/*********** AJAX Callbacks - end ***************************/


	this.initializeWidgetInstance=function(obj){
		try{
		var assignObj=this.getObjectName()+"["+this.getIndex()+"]=obj";
		eval(assignObj);
		this.setWidgetObject(eval(assignObj));
		}catch(e){
			// to do
		}
	};

	this.getWidgetInstanceName=function(){
		try{
		return this.widgetGlobalName+"["+this.getIndex()+"]";
		}catch(e){
			// to do
		}
	};

	this.getIndex=function(){
		try{
		var widgetIdx=(SMARTPORTAL.SearchWidgetInstance).length;
		return widgetIdx;
		}catch(e){
			// to do
		}
	};
	}catch(e){
		// to do
	}
};
(function($) {
	$.extend(EXPERTUS.SMARTPORTAL.SearchWidget.prototype,
			EXPERTUS.SMARTPORTAL.AbstractDetailsWidget);
})(jQuery);
function callSearchWidget(obj) {
	try{
	var idx = SMARTPORTAL.SearchWidgetInstance.length;
	var searchObj = new EXPERTUS.SMARTPORTAL.SearchWidget();
	SMARTPORTAL.SearchWidgetInstance[idx]= searchObj;
	searchObj.setWidgetObject(searchObj);
	searchObj.setUniqueWidgetId(obj.parentWidgetUniqueId);
	searchObj.parentWidgetObj = obj.parentWidgetObj;
	searchObj.currentWidgetObj = obj.currentWidgetObj;
	searchObj.searchServiceWidgetId = obj.searchServiceWidgetId;
	searchObj.searchTitle=obj.searchTitle==undefined?searchObj.searchTitle:obj.searchTitle;
	searchObj.addOptions=obj.addOptions;
	searchObj.customOptions=obj.customOptions;
	$.extend(searchObj.searchConfig, obj.searchConfig);
	$.extend(searchObj.tblConfig, obj.tblConfig);

	searchObj.searchParams = obj.searchParams;
	searchObj.searchParamsDivId = obj.searchParamsDivId;
	searchObj.searchResults = obj.searchResults;
	searchObj.searchResultsDivId = obj.searchResultsDivId;
	searchObj.callerType=obj.callerType;
	searchObj.qrystring=obj.qrystring;
	searchObj.printCols=obj.printCols;
	searchObj.exportCols=obj.exportCols;
	searchObj.searchCallType = obj.searchCallType;
	searchObj.render();
	return searchObj;
	}catch(e){
		// to do
	}
};