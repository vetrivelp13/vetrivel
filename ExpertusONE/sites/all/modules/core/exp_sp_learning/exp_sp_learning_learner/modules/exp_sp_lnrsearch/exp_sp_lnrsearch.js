(function($) {

$.widget("ui.lnrcatalogsearch", {
	_init: function() {
		try{
		if(document.getElementById('block-exp-sp-coursedetail-course-details') || document.getElementById('block-exp-sp-classdetail-class-details') || document.getElementById('block-exp-sp-learning-plan-detail-learning-details'))
			return;
		var self = this;
		//41371: When an anonymous user clicks on register he should be registered after sign in/register
		if($.cookie('catalog_searchStr')!==undefined && $.cookie('catalog_searchStr')!= null && $.cookie('catalog_searchStr') !='' && $.cookie('catalog_searchStr') !='&title=')
			$.cookie('catalog_temp_searchStr', $.cookie('catalog_searchStr'));
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		this.widgetCallback = Drupal.settings.widget.widgetCallback;
		var searchStr = this.searchActionCheck();
		// console.log('searchStr', searchStr, typeof searchStr);
		var referrer = document.referrer;
		if(searchStr!='' && searchStr != undefined && searchStr!=null){
			filters = ['top-search'];
			self.getGetJSONCookie('catalogAppliedFilters', filters);
			searchStr = '&title='+encodeURIComponent(searchStr)+'&catalogAppliedFilters='+JSON.stringify(filters);
			// self.searchAction('top-search');
			//Embed widget related work
			if(Drupal.settings.widget.widgetCallback!=true){
				$.cookie('catalog_searchStr', searchStr);
				$.cookie('catalog_temp_searchStr', $.cookie('catalog_searchStr'));
			}
		}else if(searchStr=='' && (referrer.indexOf('q=learning/class-details')>1 || referrer.indexOf('q=learning/learning-plan')>1 || referrer.indexOf('q=learning/course-details')>1 || (referrer.indexOf('q=learning/catalog-search')>1))) {
			searchStr = $.cookie('catalog_searchStr');
			if(searchStr!='' && searchStr!=null){ // Checked for cookie disabled status 
				$.cookie("searchStr_read", 1);
				var sortByIndex = searchStr.indexOf('&sortby=');
				this.sortbyValue = searchStr.substring(sortByIndex+8, searchStr.length);
			}
		} else {
			//$.cookie('catalog_searchStr')==''?null:$.cookie('catalog_searchStr', '');
			$.cookie('priceLftCkValue')==''?null:$.cookie('priceLftCkValue', '');
			$.cookie('priceRgtCkValue')==''?null:$.cookie('priceRgtCkValue', '');
			//$.cookie('catalog_temp_searchStr')==''?null:$.cookie('catalog_temp_searchStr', '');
			$.cookie("searchStr_read", 0);
		}
		//42055: Provide an option in the admin share widget screen to pass site url
		if($.cookie('catalog_searchStr_passurl') !== undefined && $.cookie('catalog_searchStr_passurl')!='' && $.cookie('catalog_searchStr_passurl')!=null){
			$.cookie('catalog_searchStr', $.cookie('catalog_searchStr_passurl'));
		}
		//console.log($.cookie('catalog_searchStr'));
		this.renderSearchResults(searchStr);
		filtersAdded = false;	// a global variable to check if the filters have been added from cookie
		}catch(e){
			// to do
			// console.log(e, e.stack);
		}
	},
	searchAction : function(callFrom, sortbytxt, className) {
		try{
			
			
		var searchStr = '';		
		var ob_type = '';
		$('.ot-others').each(function(){
			if($(this).is(':checked')){
				ob_type+=ob_type!=''?'|':'';
				ob_type+=$(this).val();
			}
		});
		
		var mro_type = '';
		$('.mro-others').each(function(){
			if($(this).is(':checked')){
				mro_type+=mro_type!=''?'|':'';
				mro_type+=$(this).val();
			}
		});

		var all_mro_type = '';
		$('.mro-others').each(function(){
				all_mro_type+=all_mro_type!=''?'|':'';
				all_mro_type+=$(this).val();
		});

		var dl_type = '';
		$('.dt-others').each(function(){
			if($(this).is(':checked')){
				dl_type+=dl_type!=''?'|':'';
				dl_type+=$(this).val();	
			}

		});
		
		var lg_type = '';
		$('.lang-others').each(function(){
			if($(this).is(':checked')){
				lg_type+=lg_type!=''?'|':'';
				lg_type+=$(this).val();
			}
		});

		var all_lg_type = '';
		$('.lang-others').each(function(){
			all_lg_type+=all_lg_type!=''?'|':'';
			all_lg_type+=$(this).val();
		});
		
		var jr_type = '';
		$('.jobrole-others').each(function(){
			if($(this).is(':checked')){
				jr_type+=jr_type!=''?'|':'';
				jr_type+=$(this).val();
			}
		});

		var all_jr_type = '';
		$('.jobrole-others').each(function(){
			all_jr_type+=all_jr_type!=''?'|':'';
			all_jr_type+=$(this).val();
		});
		
		var cy_type = '';
		$('.country-others').each(function(){
			if($(this).is(':checked')){
				cy_type+=cy_type!=''?'|':'';
				cy_type+=$(this).val();
			}
		});

		var all_cy_type = '';
		$('.country-others').each(function(){
			all_cy_type+=all_cy_type!=''?'|':'';
			all_cy_type+=$(this).val();
		});
		/*-------Price-------*/
		var Lprice = $( "#price-slide-left" ).val();
		if(Lprice != undefined && Lprice != 'undefined')
			Lprice = encodeURIComponent(Lprice.charAt(0))+Lprice.substring(1);
		var Rprice = $( "#price-slide-right" ).val();
		if(Rprice != undefined && Rprice != 'undefined')
			Rprice = encodeURIComponent(Rprice.charAt(0))+Rprice.substring(1);
		price = $.trim(Lprice+"-"+Rprice);
		price = price.replace(/ /g, "");			
		if(price == 'undefined-undefined' || price == undefined-undefined || price=='-')
			price = '';

		/*-------Start Date && End Date-------*/
		var startdate   = $("#ad_startdate1").val();
		var enddate   = $("#ad_startdate2").val();	
		var regEx = /^\d{2}-\d{2}-\d{4}$/;
		// console.log(startdate.match(regEx));
		if(startdate == Drupal.t("LBL251")+ ':' +Drupal.t("LBL112") ||  startdate.match(regEx) == null)
				startdate = "";
			else			
				$('#date-clr').css('display','block');
	
		if(enddate == Drupal.t("LBL113") ||  enddate.match(regEx) == null)
				enddate = "";
			else		
				$('#date-clr').css('display','block');
	
		/*-------Title-------*/
		var title 	  = $('#search_searchtext').val();
		
			if((title.toLowerCase()) == (Drupal.t('LBL304').toLowerCase()))
				title='';
			
		
		/*-------Location-------*/
		var location 	= $('#srch_criteria_location').val();
			if(location == Drupal.t("LBL1321")) // Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404
				location='';
			else
				$('#location-clr').css('display','block');

		/*-------Tag-------*/
		var tag = '';
		if ($('#srch_criteria_tag').length > 0) {
		  var tag = $('#srch_criteria_tag').val();
        }
        if (tag != '') {
	      $('#tag-clr').css('display', 'block');
		}
		/*-------Rating-------*/	
		var rating_type = '';
		$('.rating-others').each(function(){
			if($(this).is(':checked')){
				rating_type+=rating_type!=''?'|':'';
				rating_type+=$(this).val();	
			}

		});

		/*-------Sort By-------*/
		if (sortbytxt!=null && sortbytxt!=undefined) this.sortbyValue = sortbytxt; 
		var sortby = this.sortbyValue;
		if(title == undefined)
			title = "";
		
		//Following section will deal about showing applied filters with its clear command
		//if callFrom is checkbox, try getting its label name using 'for' attribute and vtip class
		//if callFrom is textbox, print the text as it is
		//if callFrom is rating, try seeing its value which is average rating value, you can convert it star value
		// var filterType = $(callFrom).attr('type');
		var filtersApplied = this.getGetJSONCookie('catalogAppliedFilters');
		if(callFrom == 'clearAll') {
			this.removeFilters(filtersApplied, filtersApplied.length);
			filtersApplied = [];
		} else if($(callFrom).is(':checkbox')) {
			var filterId = $(callFrom).data('filter-id');
			if($(callFrom).is(':checked')) {
				//add filter to applied filters list
				filtersApplied.push(filterId);
				// this.addFilters(filterId);
				this.addFilters(filterId);
			} else {
				//remove filters from applied filters list
				filtersApplied = filtersApplied.filter(function(item) {
					return filterId !== item;
				});
				this.removeFilters(filterId, filtersApplied.length);
			}
		} else if($(callFrom).is('div#price-slider-range') || callFrom == 'clearprice') {
			//call from is price slider
			var filterId = $('#price-slider-range').data('filter-id');
			if(callFrom == 'clearprice') {
				filtersApplied = filtersApplied.filter(function(item) {
					return filterId !== item;
				});
				this.removeFilters(filterId, filtersApplied.length);
			} else if(price != '') {
				filtersApplied = filtersApplied.filter(function(item) {
					return filterId !== item;
				});
				this.removeFilters(filterId, filtersApplied.length);
				filtersApplied.push(filterId);
				this.addFilters(filterId);
			}
		} else if(callFrom == 'date') {
			// startdate, enddate
			filtersApplied = filtersApplied.filter(function(item) {
				return 'date-range' !== item;
			});
			this.removeFilters('date-range', filtersApplied.length);
			if(startdate != '' || enddate != '') {
				filtersApplied.push('date-range');
				this.addFilters('date-range');
			}
		} else if(callFrom == 'Clear-Date') {	//callFrom clearField("Date");
			filtersApplied = filtersApplied.filter(function(item) {
				return 'date-range' !== item;
			});
			this.removeFilters('date-range', filtersApplied.length);
		} else if(callFrom == 'cloud-tag') {
			filtersApplied = filtersApplied.filter(function(item) {
				return 'cloud-tag' !== item;
			});
			this.removeFilters('cloud-tag', filtersApplied.length);
			filtersApplied.push('cloud-tag');
			this.addFilters('cloud-tag');
		} else if(callFrom == 'Clear-Tag') {	//clearField("Tag");"
			filtersApplied = filtersApplied.filter(function(item) {
				return 'cloud-tag' !== item;
			});
			this.removeFilters('cloud-tag', filtersApplied.length);
		} else if(callFrom == 'location') {
			filtersApplied = filtersApplied.filter(function(item) {	//remove existing location filter
				return 'srch_criteria_location' !== item;
			});
			this.removeFilters('srch_criteria_location', filtersApplied.length);
			filtersApplied.push('srch_criteria_location');
			this.addFilters('srch_criteria_location');
		} else if(callFrom == 'Clear-Location') {	//clearField("Location");"
			filtersApplied = filtersApplied.filter(function(item) {
				return 'srch_criteria_location' !== item;
			});
			this.removeFilters('srch_criteria_location', filtersApplied.length);
		} else if(callFrom == 'top-search') {
			filtersApplied = filtersApplied.filter(function(item) {
				return 'top-search' !== item;
			});
			this.removeFilters('top-search', filtersApplied.length);
			if(title != '') {
				filtersApplied.push('top-search');
				this.addFilters('top-search');
			}
			
		}
		this.getGetJSONCookie('catalogAppliedFilters', filtersApplied);
			//console.log('catalogAppliedFilters - ', filtersApplied);
		searchStr	= '&title='+encodeURIComponent(title)+'&dl_type='+dl_type+'&lg_type='+lg_type+'&all_lg_type='+all_lg_type+'&ob_type='+ob_type+'&location='+encodeURIComponent(location)+'&tag='+encodeURIComponent(tag)+'&price='+price+'&startdate='+startdate+'&enddate='+enddate+'&jr_type='+jr_type+'&all_jr_type='+all_jr_type+'&cy_type='+cy_type+'&all_cy_type='+all_cy_type+'&sortby='+sortby+'&mro_type='+mro_type+'&all_mro_type='+all_mro_type+'&rating_type='+rating_type+'&catalogAppliedFilters='+JSON.stringify(filtersApplied);
		$.cookie("catalog_searchStr", searchStr);

		/*$('#paintContentResults').hide();
		$('#gview_paintContentResults').css('min-height','100px');*/
		
    	this.createLoader('lnr-catalog-search');
		$('#paintContentResults').setGridParam({url: this.constructUrl('learning/catalog-search/search/all/'+searchStr)});
		$("#paintContentResults").trigger("reloadGrid",[{page:1}]);
	    $('.ac_results').css('display', 'none');
	    
	    this.checkboxValidation();

	    //Highlight sort type VJ
	    if(className!= null) {
			$('.sort-by-links li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
			});
			$('.'+className).addClass('sortype-high-lighter');
	    }
	    else {
			$('.sort-by-links li').each(function() {
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
			//window.console.log(e, e.stack);
		}
	},

	searchActionLocation : function() {
		try{
		var location 	= $('#srch_criteria_location').val(); //Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404
		if(location != Drupal.t("LBL1321"))
			this.searchAction('location');
		}catch(e){
			// to do
		}
	},
	
	clearField : function (txt, searchAction) {
		try{
		if(typeof searchAction == 'undefined') {
			searchAction = true;
		}
		if(txt=='Location'){
			this.clearSearchParam('location'); //Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404
			$('#srch_criteria_location').val(Drupal.t("LBL1321")).css('color','#999999').css('fontSize','11px');
			$('#location-clr').css('display','none');
		}
		else if (txt == 'Tag') {
				this.clearSearchParam('tag');
				$('#srch_criteria_tag').val('');
				$('.tagscloud-tag').css('text-decoration', '');
				$('#tag-clr').css('display', 'none');
		}
		else {
			this.clearSearchParam('startdate');
			this.clearSearchParam('enddate');
			$('#ad_startdate1').val(Drupal.t("LBL251")+ ':' +Drupal.t("LBL112")).css('color','#999999').css('fontSize','11px');
			$('#ad_startdate2').val(Drupal.t("LBL113")).css('color','#999999').css('fontSize','11px');
			$('#date-clr').css('display','none');
		}
		$("#date-validate-newid").remove();
		if(searchAction) {
			this.searchAction('Clear-'+txt);
		}
		}catch(e){
			// to do
		}
	},
	
	clearSearchParam : function(clearField) {
		try {
			// read the obj value from settings
			var param = unserialize(Drupal.settings.refer_course.share_details);	
			// clear the filed
			param[clearField] = '';
			//construct the obj
			var clearStr = Object.keys(param).map(function(key){ 
				  return encodeURIComponent(key) + '=' + encodeURIComponent(param[key]); 
				}).join('&');
			Drupal.settings.refer_course.share_details = clearStr; 
		} catch(e) {
			// to do
		}
	},
	
	showHide : function (strOne, strTwo, closeTarget) {
		try{
		if(closeTarget) {
			$('#'+strTwo).hide();
		} else {
		$('#'+strTwo).toggle();
		}
		
			var classShowHide = $('#'+strOne).hasClass('cls-show');
			if(classShowHide){
				$('#'+strOne).removeClass('cls-show');
				$('#'+strOne).addClass('cls-hide');
			}else{
				$('#'+strOne).removeClass('cls-hide');
				$('#'+strOne).addClass('cls-show');
			}
		var filters = (($.cookie('filters-state') !== undefined && $.cookie('filters-state') != null) ? JSON.parse($.cookie('filters-state')) : {});
		filters[strOne] = $('#'+strOne).hasClass('cls-show');	//filter value true means it visible/uncolopsed
		$.cookie('filters-state', JSON.stringify(filters));
		// console.log($.cookie('filters-state'));
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
					checkboxSelectedUnselectedCommon(this);
				}
			} else {			
				if($(this).val() != 'All'){
					$(this).parent().next('label').removeClass('highlight-light-gray');
					$(this).parent().next('label').addClass('highlight-light-blue');
					checkboxSelectedUnselectedCommon(this);
				}
			}

		});	
		}catch(e){
			// to do
		}
	},
	
	moreListDisplay : function(recLen,dispId) {
		try{
		for(i=0;i<=recLen;i++){
			$('#'+dispId+'_'+i).css("display","block");
		}
		$('#'+dispId+'_more').css("display","none");
		$('#'+dispId+'_short').css("display","block");
		}catch(e){
			// to do
		}
		vtip();
	},

	shortListDisplay : function(recLen,dispId){
		try{
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
		}catch(e){
			// to do
		}
		vtip();
	},

  hidePageControls : function(hideAll) {
	try{  
	$('#paintCatalog-show_more').hide();
    var lastDataRow = $('#paintContentResults tr.ui-widget-content').filter(":last");
    // console.log(lastDataRow.length);
     if (lastDataRow.length != 0) {
        //console.log('hidePageControls() : hideAll - last data row found');
        lastDataRow.children('td').css('border', '0 none');
      }
	  return;
    if (hideAll) {
      if(this.currTheme == "expertusoneV2")
    	  $('#paintContent .block-footer-left').show();
      $('#pager').hide();
      //remove bottom dotted border from the last row in result if records are present
      if (lastDataRow.length != 0) {
        //console.log('hidePageControls() : hideAll - last data row found');
        lastDataRow.children('td').css('border', '0 none');
      }
    }
    else {
      //console.log('hidePageControls() : hide only next/prev page control');
      $('#pager').show();
      if(this.currTheme == "expertusoneV2")
    	  $('#paintContent .block-footer-left').hide();
      $('#pager #pager_center #first_pager').hide();
      $('#pager #pager_center #last_pager').hide();
      $('#pager #pager_center #next_pager').hide();
      $('#pager #pager_center #prev_pager').hide();              
      $('#pager #pager_center #sp_1_pager').parent().hide();
    }
	}catch(e){
		// to do
	}
  },
  
  showPageControls : function() {
    //console.log('showPageControls() : show all control');
	try{  
		$('#paintCatalog-show_more').show();
		var lastDataRow = $('#paintContentResults tr.ui-widget-content').filter(":last");
    // console.log(lastDataRow.length);
     if (lastDataRow.length != 0) {
        //console.log('hidePageControls() : hideAll - last data row found');
        lastDataRow.children('td').css('border', '0 none');
      }
		return;
    $('#pager').show();
    if(this.currTheme == "expertusoneV2")
    	$('#paintContent .block-footer-left').hide();
    $('#pager #pager_center #first_pager').show();
    $('#pager #pager_center #last_pager').show();
    $('#pager #pager_center #next_pager').show();
    $('#pager #pager_center #prev_pager').show();              
    $('#pager #pager_center #sp_1_pager').parent().show();
	}catch(e){
		// to do
	}
  },

	renderSearchResults : function(searchStr){	   
		try{
		this.createLoader('lnr-catalog-search');
		var obj = this;
		if(this.currTheme == "expertusoneV2"){
			var gridWidth 		= 762;	
			var detailsWidth 	= 521;
			var actionWidth 	= 150;
			var iconsWidth 		= 70;
		}else{
			var detailsWidth 	= 595;
			var actionWidth 	= 130;
			var gridWidth 		= 764;
			var iconsWidth 		= 54;
		}
		
		//searchStr = '';
		//var urlStr = (searchStr != '') ? ('&title='+encodeURIComponent(searchStr)) : '';
		var urlStr = (searchStr != '') ? searchStr : '';
		// change the search string based on the email share
		if (Drupal.settings.refer_course.share_details !== undefined) {
			urlStr = Drupal.settings.refer_course.share_details;
		}
		//var tempPath = "http://widget.expertusone.com/?q=learning/catalog-search/search/all/"+urlStr;	
		var objStr = '$("#lnr-catalog-search").data("lnrcatalogsearch")';
		 var pagenumber = 1;
		 var learnerId = obj.getLearnerId();
		 var rownumber = 10;
		 var rowlist = [10,20,30];
		//after login auto register or add to cart related work start
		 if(learnerId != 0 && learnerId !='' && learnerId != null && learnerId!=undefined){
			 var user_selected_page_number = $.cookie("user_selected_page_number");
			 if(user_selected_page_number != null && user_selected_page_number !=undefined){
				 pagenumber = user_selected_page_number;
				 $.cookie("user_selected_page_number",'',{expires: -300})
			 }
			 //42055: Provide an option in the admin share widget screen to pass site url
			 var user_selected_row_number = $.cookie("user_selected_row_number");
			 if(user_selected_row_number != null && user_selected_row_number !=undefined){
				 if(user_selected_row_number == 20){
					 rownumber = 20;
					 rowlist = [20,40,60];
				 }else if(user_selected_row_number == 30){
					 rownumber = 30;
					 rowlist = [30,60,90];
				 }				 
				 $.cookie("user_selected_row_number",'',{expires: -300});
			 }
		}
		//42055: Provide an option in the admin share widget screen to pass site url
		 if (Drupal.settings.WIDGETMODULEVARIABLE.page_number != undefined) {
			 pagenumber = Drupal.settings.WIDGETMODULEVARIABLE.page_number;
		 }
		 if (Drupal.settings.WIDGETMODULEVARIABLE.row_number != undefined) {
			 if(Drupal.settings.WIDGETMODULEVARIABLE.row_number == 20){
				 rownumber = 20;
				 rowlist = [20,40,60];
			 }else if(Drupal.settings.WIDGETMODULEVARIABLE.row_number == 30){
				 rownumber = 30;
				 rowlist = [30,60,90];
			 }	
		 }
		//after login auto register or add to cart related work end
		 //Embed widget related work (check the display parameter)
		if(Drupal.settings.widget.widgetCallback==true){
			urlStr = Drupal.settings.widget.widget_details['widget_parameters']+'&callfrom=widget';
			var displayParams = Drupal.settings.widget.widget_details['catalog_display_parameters'];
			if(typeof displayParams != 'undefined') {
				var colNamesArray = [];
				var colModelsArray = [];
				//console.log(displayParams['show_icon']);
				if(displayParams['show_icon']==true){
					colNamesArray.push('Icons');
					colModelsArray.push({name:'Icons',index:'Icons',align:'left', title:false,fixed: true, width:iconsWidth,'widgetObj':objStr,formatter:obj.paintLPSearchIcons});
		    	}
				colNamesArray.push('Details');
				colModelsArray.push({name:'Details',index:'Details',align:'left', title:false,'widgetObj':objStr,formatter:obj.paintLPSearchResults});
				//console.log(displayParams['show_button']);
		    	if(displayParams['show_button']==true){
		    		colNamesArray.push('Action');
		    		colModelsArray.push({name:'Action',index:'Action',align:'left', title:false,fixed: true, width:actionWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResultsAction});
		    	}
			} else {
				var colNamesArray = ['Icons','Details','Action'];
				var colModelsArray = [ {name:'Icons',index:'Icons',align:'left', title:false,fixed: true, width:iconsWidth,'widgetObj':objStr,formatter:obj.paintLPSearchIcons},
							           {name:'Details',index:'Details',align:'left', title:false,'widgetObj':objStr,formatter:obj.paintLPSearchResults},
							           {name:'Action',index:'Action',align:'left', title:false,fixed: true, width:actionWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResultsAction}];
			}
			
			$("#paintContentResults").jqGrid({
				url: obj.constructUrl("learning/catalog-search/search/all/"+urlStr),
				datatype: "json",
				mtype: 'GET',
				colNames:['Cell'],
				colModel:[{name:'cell',index:'cell', title:false, width:iconsWidth,'widgetObj':objStr,formatter:obj.testReturn}],
				page: pagenumber,
				rowNum:rownumber,
				rowList:rowlist,
				pager: '#pager',
				viewrecords:  true,
				emptyrecords: "",
				sortorder: "desc",
				toppager:true,
				height: 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadui:false,
				hoverrows:false,
				loadComplete:obj.callbackLoader,
				jsonReader: {id: "jqgrid-rowid"},
			}).navGrid('#pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});
			this.paintAfterReady();
		}
		else{
			if($.cookie('catalog_searchStr_passurl') !== undefined && $.cookie('catalog_searchStr_passurl')!='' && $.cookie('catalog_searchStr_passurl')!=null){
				if(learnerId != 0 && learnerId !='' && learnerId != null && learnerId!=undefined)
					urlStr = urlStr+$.cookie('catalog_searchStr_passurl')+'&language='+Drupal.settings.user.language+'&callfrom=passurl';
				else
					urlStr = urlStr+$.cookie('catalog_searchStr_passurl')+'&callfrom=passurl';
		   }else {
			   if($.cookie('catalog_searchStr') !== undefined && $.cookie('catalog_searchStr')!='' && $.cookie('catalog_searchStr')!=null){
					if(learnerId != 0 && learnerId !='' && learnerId != null && learnerId!=undefined)
						urlStr = urlStr+$.cookie('catalog_searchStr')+'&language='+Drupal.settings.user.language+'&callfrom=passurl';
					else
						urlStr = urlStr+$.cookie('catalog_searchStr')+'&callfrom=passurl';
			   }
		   }
			$("#paintContentResults").jqGrid({
				url: obj.constructUrl("learning/catalog-search/search/all/"+urlStr),
				datatype: "json",
				mtype: 'GET',
				// colNames:['Icons','Details','Action'],
				colNames:['Cell'],
				// colModel:[ {name:'Icons',index:'Icons', title:false, width:iconsWidth,'widgetObj':objStr,formatter:obj.paintLPSearchIcons},
				           // {name:'Details',index:'Details', title:false, width:detailsWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResults},
				           // {name:'Action',index:'Action', title:false, width:actionWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResultsAction}],
				colModel:[{name:'cell',index:'cell', title:false, width:iconsWidth,'widgetObj':objStr,formatter:obj.testReturn}],
				page: pagenumber,
				rowNum:rownumber,
				rowList:rowlist,
				pager: '#pager',
				viewrecords:  true,
				emptyrecords: "",
				sortorder: "desc",
				toppager:true,
				height: 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadui:false,
				hoverrows:false,
				loadComplete:obj.callbackLoader,
				jsonReader: {id: "jqgrid-rowid"},
				/* beforeSelectRow: function (e, rowid, orgClickEvent) {
									try {
									// if we want to return true, we should test e.result additionally
									// console.log(e, rowid, orgClickEvent);
									// console.log($(rowid.target).closest("tr.jqgrow")[0]);
									// console.log($('#paintContentResults').jqGrid('getGridParam', 'lastAccessedRow'));
									return e.result === undefined ? true : e.result;
									} catch(e) {
										// console.log(e, e.stack);
									}
								} */
			}).navGrid('#pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});
				}
		
		/* To highlight the default sort order - added by Rajkumar U*/
		$('.sort-by-links li').each(function() {
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
			var obj = $("#lnr-catalog-search").data("lnrcatalogsearch");
			var prefSet = Drupal.settings.user_preferences.catalog_refine
			var userId = obj.getLearnerId();
			// Fix for #0077109
			if(response.user_preferences != undefined && userId>0){
				Drupal.settings.user_preferences = response.user_preferences
			}
			if(prefSet != Drupal.settings.user_preferences.catalog_refine){
				obj.pinUnpinFilterCriteria('','toggle');
			}
		//alert(response.compstatus.toSource())
		$('#paintContentResults').show();
		var recs = parseInt($("#paintContentResults").getGridParam("records"),10);
	  //console.log('callbackLoader() : recs = ' + recs);
        if (recs == 0) {
        	 $('#no-records').css('display','block');
            var html = Drupal.t('MSG381');
            $("#no-records").html(html);            
        } else {
        	$('#no-records').css('display','none');
        	$("#no-records").html("");
        }

        Drupal.attachBehaviors();
        
        // Show pagination only when search results span multiple pages
        var reccount = parseInt($("#paintContentResults").getGridParam("reccount"), 10);
        var hideAllPageControls = true;
        if (recs > 10) { // 10 is the least view per page option.
          hideAllPageControls = false;
          //console.log('callbackLoader() : hideAllPageControls set to false');
        }
        
        // console.log('callbackLoader() : reccount = ' , reccount, 'recs', recs);
        /* if (recs <= reccount) {
          //console.log('callbackLoader() : recs <= reccount : hide pagination controls');
          obj.hidePageControls(hideAllPageControls);
        }
        else {
          //console.log('callbackLoader() : recs <= reccount : show pagination controls');
          obj.showPageControls();
        } */   
		var recs = response['records'];
		var showMore = $('#paintCatalog-show_more');
		updateShowMore = ((updateShowMore == undefined || updateShowMore == null) ? true : updateShowMore);
		if (updateShowMore) {
			$("#paintContentResults").data('totalrecords', recs);
			if ($('#paintContentResults').getGridParam("reccount") < recs) {
				showMore.show();
			} else {
				showMore.hide();
			}
		}
        $("#paintContentResults").find('tr.ui-widget-content').filter(":last").addClass('ui-widget-content-last-row');
/*        $('.fivestar-click').click(function() {
        	var fivestarClick = this;
        	var fiveStarClass = $(this).attr('class');
        	var pattern = /[0-9]+/;
        	var rating = fiveStarClass.match(pattern);
        	var nodeId = $(this).parents("form").siblings("input:hidden").val();
        	var param = {'rating':rating,'nodeId':nodeId};
        	
    		url = obj.constructUrl("learning/five-star-submit");
    		$.ajax({
    			type: "POST",
    			url: url,
    			data:  param,
    			success: function(result){
	    			if(result.average_rating == "AlreadyRated"){
	    				return false;
		    			
	    			}else{
	    				var average_stars = (result.average.value * 5) / 100;
		    			average_stars = average_stars.toFixed(1);
		    			var voteText = (result.count.value == 1) ? 'vote' : 'votes';
		    			var avgRating = '<span class="average-rating">Average: <span>'+average_stars+'</span></span>';
		    			avgRating     += '<span class="total-votes"> (<span>'+result.count.value+'</span> '+voteText+')</span>';
		    			$(fivestarClick).parent("div").next("div.description").children("div.fivestar-summary").html(avgRating);	    				
	    			}
    			}
    	    });
		});*/
        
	    //$("#dummy_link").trigger("click");
        $("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('lnr-catalog-search');
        $("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('searchRecordsPaint');
        $('.loadercontent').remove();
        //$('.cloudtagsp').html($(response.filter).find(".cloudtagsp").html());
	   if($("#lnr-catalog-search").data("lnrcatalogsearch").defaults.start) {
		     $("#search-filter-content").html(response.filter);
			// console.log(response.filter);
		    $("#find-trng-sort-display").show();
		   /* if ($('#tags_cloud').length > 0) {
		      var tagsHeight = $('#tags_cloud').height();
		      if (tagsHeight > 175) {
		        $('#tags_cloud').height(175);
		        $('#tags_cloud').css('max-height', '175px');
			    //$('#tags_cloud').jScrollPane({});
		      }
		    }*/
		    $("#lnr-catalog-search").data("lnrcatalogsearch").defaults.start = false;
		    
		    /*
		    * if search text from home page through search autocomplete inputbox then Catalog page Type checkboxes should be unchecked. 
		    * same if click CATALOG link then checkboxes should be checked based on results (By default delivery type results only will be displayed).
		    */
		    var req=window.location.search;
			req=req.substring(3,req.length);
			var reqArr=req.split("/");
			/*if(reqArr.length<=2){
				$("#lnr-catalog-search").data("lnrcatalogsearch").checkBoxSelected(response);
			}*/
			/*----*/
			if ($('#lang_list').val() > 6) {
				$('#paintLanguage').css({'height': '160px', 'width':'135px','margin-top':'10px'});
				$('#paintLanguage').jScrollPane({});
			}
			// $('.limit-title').trunk8(trunk8.embedwidget_title);
			// $('.limit-desc').trunk8(trunk8.embedwidget_desc);
			var trunk8options = [];
			trunk8options.push({'selector': '.content-title .limit-title', 'lines': trunk8.embedwidget_title});
			trunk8options.push({'selector': '.content-description .limit-desc', 'lines': trunk8.embedwidget_desc});
			 obj.truncateTitleDescription(trunk8options);
			//var languagelistid = #($('.filterlanguage').attr('id'));
			//var countrylistid = #($('.filtercountry').attr('id'));				
			if ($('#filterlist_language').val() > 6) {
				$('#paintLanguage .searchtext').css('display', 'block');
				$('#language_searchtext').css('display', 'block');				
				$('#paintLanguagescroll').css({'height': '160px', 'width': '130px'});
				$('#paintLanguagescroll').jScrollPane({});
			}
			if($('#filterlist_country').val() > 6){
				$('#paintCountry .searchtext').css('display', 'block');
				$('#country_searchtext').css('display', 'block');
				$('#paintCountryscroll').css({'height': '160px', 'width': '130px'});
				$('#paintCountryscroll').jScrollPane({});
			}
			
	    } else {
		   $("#lnr-catalog-search").data("lnrcatalogsearch").unselectFilter(response);
		   // $('.limit-title').trunk8(trunk8.embedwidget_title);
		   // $('.limit-desc').trunk8(trunk8.embedwidget_desc); 
		    var trunk8options = [];
			trunk8options.push({'selector': '.content-title .limit-title', 'lines': trunk8.embedwidget_title});
			trunk8options.push({'selector': '.content-description .limit-desc', 'lines': trunk8.embedwidget_desc});
			obj.truncateTitleDescription(trunk8options);
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
	   		// call for updated content result display in launch-content-container register and launch feature
		  //console.log('test event for one click launch');
		  if(document.getElementById('launch-content-container')){
			  //console.log('hrere');
			  var dataObj = $("#paintContentResults").jqGrid('getGridParam', 'postData');
			  //console.log('pst data received');
			  //console.log(dataObj);
			  if(dataObj.enrollmentId!='' && dataObj.enrollmentId!=undefined){
				  $("#launch"+dataObj.enrollmentId).click();
				 // console.log(dataObj.enrollmentId);
			  }
		  }
		//Vtip-Display toolt tip in mouse over
		 vtip();
		 resetFadeOutByClass('#paintContentResults','content-detail-code','line-item-container','catalog');
		 if ($('#jobrole_list').val() > 4) {
				$('#paintJobrole').css('height', '90px').css('width', '130px');
			 if(!$('#paintJobrole').hasClass('jspScrollable')) {
				$('#paintJobrole').jScrollPane({});
			}
		 }
		 var learnerId = obj.getLearnerId();
		 //console.log(learnerId);
		//after login auto register or add to cart related work start
		 if(learnerId != 0 && learnerId !='' && learnerId != null && learnerId!=undefined){
			 var user_selected_class_id = $.cookie("user_selected_class_id");
			 if(user_selected_class_id != null && user_selected_class_id !=undefined){
				 $(window).scrollTop($('#'+user_selected_class_id).offset().top);
				 $('#'+user_selected_class_id).focus();
				 var onclickprop = $('#'+user_selected_class_id).attr("onclick");
				 if(onclickprop != null &&  onclickprop !=''  && onclickprop!=undefined){
					 $('#'+user_selected_class_id).click();
				}else if($.trim($('#'+user_selected_class_id).html()) == Drupal.t('Register') && $('#'+user_selected_class_id).hasClass("action-btn-disable")){
					var status_msg = Drupal.t('MSG829');
					obj.callMessageWindow(Drupal.t('LBL721'),status_msg);
				}else{
					var status_msg = Drupal.t('ERR047');
					if(user_selected_class_id.indexOf("object-registerCls") > -1 == true){
						status_msg = Drupal.t('MSG430');
					}
					 obj.callMessageWindow(Drupal.t('LBL721'),status_msg);
				}
				 $.cookie("user_selected_class_id",'',{expires: -300})
				 $.cookie("user_selected_url", '',{ expires: -300 });
			 }
		 }
		 //42055: Provide an option in the admin share widget screen to pass site url
		 if (Drupal.settings.WIDGETMODULEVARIABLE.click_id != undefined && Drupal.settings.WIDGETMODULEVARIABLE.click_id != null) {
			 $('#'+Drupal.settings.WIDGETMODULEVARIABLE.click_id).click();
			 Drupal.settings.WIDGETMODULEVARIABLE.click_id = undefined;
 			 history.pushState(null, Drupal.t('CATALOG'), "?q=learning/catalog-search");
		 }
		//after login auto register or add to cart related work end
		   if(Drupal.settings.widget.widgetCallback==true){
			   if($.browser.msie && parseInt($.browser.version, 10)=='10'){
	 		    $("#lnr-catalog-search .ui-jqgrid .ui-jqgrid-bdiv").css("height","auto");   
			   }
			   $("#pager").css("width","auto");
		   }
	 //  $(".ui-jqgrid-bdiv").css("overflow","hidden");
	   obj.checkboxValidation();
	   // console.log('Drupal.settings.refer_course.share_details', Drupal.settings.refer_course.share_details);
	   // console.log('widget_searchstr', $.cookie('widget_searchstr'));
	   // console.log('catalog_searchStr_passurl', $.cookie('catalog_searchStr_passurl'));
	   // console.log('catalog_temp_searchStr', $.cookie('catalog_temp_searchStr'));
	   //42055: Provide an option in the admin share widget screen to pass site url
	   if ((Drupal.settings.refer_course.share_details !== undefined && $.cookie('widget_searchstr') == true) || 
			   ($.cookie('catalog_searchStr_passurl') !== undefined && $.cookie('catalog_searchStr_passurl')!=null && 
					   $.cookie('catalog_searchStr_passurl')!='') || ($.cookie('catalog_temp_searchStr') !== undefined && 
							   $.cookie('catalog_temp_searchStr')!=null && $.cookie('catalog_temp_searchStr')!='')
		)
	   {
		   if (Drupal.settings.refer_course.share_details !== undefined && $.cookie('widget_searchstr') == true){
				   var param = unserialize(Drupal.settings.refer_course.share_details);
				   Drupal.settings.refer_course.share_details = undefined;
		   }
		   else if($.cookie('catalog_searchStr_passurl') !== undefined && $.cookie('catalog_searchStr_passurl')!=null && $.cookie('catalog_searchStr_passurl')!=''){
			   var param = unserialize($.cookie('catalog_searchStr_passurl'));
			   if(learnerId != 0 && learnerId !='' && learnerId != null && learnerId!=undefined)
				   $.cookie('catalog_searchStr_passurl', '', { path: '/', expires: -5 });
		   }else if(($.cookie('catalog_temp_searchStr') !== undefined && $.cookie('catalog_temp_searchStr')!=null && $.cookie('catalog_temp_searchStr')!='')){
			   var cookie_arr = $.cookie('catalog_temp_searchStr').split('&');
			   var param = [];
			   var search_str_arr = '';
			   var cnt = cookie_arr.length;
			   var i = 0;
			   for(i=0;i<cnt;i++){
				   if(cookie_arr[i]!=''){
					   search_str_arr = cookie_arr[i].split('=');
					   if(search_str_arr[0] == 'price'){
						   search_str_arr[1] = decodeURIComponent(search_str_arr[1]);
					   }
	  					param[search_str_arr[0]] = search_str_arr[1];
				   }
			   }
			   $.cookie('catalog_temp_searchStr', '', { path: '/', expires: -5 });
		   }
			// console.log('param', param);
			// Language
		   if (param['lg_type'] != '' &&  param['lg_type'] != undefined) {
				$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['lg_type']);
				var lang_arr = param['lg_type'].split('|');
				var seemore_check = false;
				if(lang_arr.length > 0 ) {
				$.each(lang_arr, function( index, value ) {
					if ($("#lrn_srch_lang_"+value).parents('.srch-checkbox-container-cls').css('display') == 'none') {
						seemore_check = true;
						return false;
					}
				});
				if(seemore_check == true) {
					$lng_cnt = $("#paintLanguage").find(".srch-label-cls").length;
					$("#lnr-catalog-search").data("lnrcatalogsearch").moreListDisplay($lng_cnt,"lang_hideshow");
				}
				}
			}
			// course type
			if ((param['dl_type'] != '' &&  param['dl_type'] != undefined )|| (param['ob_type'] != '' &&  param['ob_type'] != undefined )) {
				$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['dl_type']);
				$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['ob_type']);
			}
			// training type
			if (param['mro_type'] != '' &&  param['mro_type'] != undefined) {
				$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['mro_type']);
			}
			// jobrole
			if (param['jr_type'] != '' &&  param['jr_type'] != undefined) {
				$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['jr_type']);
			}
			// country
			if (param['cy_type'] != '' &&  param['cy_type'] != undefined) {
				$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['cy_type']);
			}
			// location
			if (param['location'] != '' &&  param['location'] != undefined) {
				//$('#srch_criteria_location').val(param['location'].trim());
				//Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404
				var tmpval=decodeURIComponent(unescape(param['location']));
				tmpval=decodeURIComponent(tmpval); 
				$('#srch_criteria_location').val(tmpval.trim());  
				
				$('#srch_criteria_location').css({'color':'#333333', 'font-style':'normal'});
				$('#location-clr').show();
			}
			// tag
			if (param['tag'] != '' &&  param['tag'] != undefined) {
				$('#srch_criteria_tag').val(decodeURIComponent(param['tag'].trim()));
				$(".tagscloud-tag").filter(function() {
					   return $(this).text() === param['tag'].trim();
				}).css("text-decoration", "underline");
				$('#tag-clr').show();
			}
			// date filter 
			if (param['startdate'] != '' &&  param['startdate'] != undefined) {
			    $('#ad_startdate1').val(param['startdate']);
			    $('#ad_startdate1').css({'color':'#333333', 'font-style':'normal'});
			    $('#date-clr').show();
	   		}
	   		if (param['enddate'] != '' &&  param['enddate'] != undefined) {
			    $('#ad_startdate2').val(param['enddate']);
			    $('#ad_startdate2').css({'color':'#333333', 'font-style':'normal'});
			    $('#date-clr').show();
		   	}
	   		// price range filter
	   		if (param['price'] != '' &&  param['price'] != undefined) {
	   			var symbol = Drupal.settings.user_prefference.currency_sym;
	   			if(symbol == "")
	   				symbol = "";
	   			param['price'] = replaceAll(symbol, "", param['price']);
	   			var priceStr = param['price'].split('-');
	   			var minPrice = parseInt(priceStr[0].replace ( /[^\d]/g, '' ));
	   			var maxPrice = parseInt(priceStr[1].replace ( /[^\d]/g, '' ));
	   			var range 	 = maxPrice - minPrice;
	   			
	 
	   			if($.cookie("preferredcurrencychange")==1){
	   			  	var min_price = 0;
	   				$('#price-slide-left').val(symbol+min_price);
	   			    $('#price-slide-right').val(symbol+price_max_value);
	   			     obj.clearPriceSlider(false);
	   			     setTimeout(function(){
				      obj.searchAction('clearprice');
			         },100);
	   			   	$.cookie("preferredcurrencychange", "0");
	   			}else{

	   				$('#price-slider-range .ui-slider-range').css({'left': (minPrice/parseInt(price_max_value))*100+'%', 'width': (range/parseInt(price_max_value))*100+'%'});
	   				$('#price-slider-range a').eq(0).css('left', (minPrice/parseInt(price_max_value))*100+'%');
	   				$('#price-slider-range a').eq(1).css('left', (maxPrice/parseInt(price_max_value))*100+'%');
	   				$('#price-slide-left').val(symbol+minPrice);
	   				$('#price-slide-right').val(symbol+maxPrice);
	   			}	
	   		}
	   		// title filter 
	   		if (param['title'] != '' &&  param['title'] != undefined) {
	   			$('#search_searchtext').val(unescape(param['title']));
	   			$('#search_searchtext').css('color', '#333333')
	   		}
	   		// rating 
	   		if (param['rating_type'] != '' &&  param['rating_type'] != undefined) {
	   			$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['rating_type']);
	   		}
	   		// sort by value
	   		if (param['sortby'] != '' &&  param['sortby'] != undefined) {
	   			$("#lnr-catalog-search").data("lnrcatalogsearch").setSortByValue(param['sortby']);
	   		}
	   		$.cookie('widget_searchstr', '', { path: '/', expires: -5 });
	   		
	   }
	   if(Drupal.settings.widget.widgetCallback==true){
		   $('.top-record-div-left').each(function() {
			   var imageWidth = $(this).find('.catalog-course-compliance-role-bg').width();
			   var imageRole  = $(this).find('.catalog-course-role-access-bg').width();
			   if(imageWidth ==  null) {
				   if(imageRole !=null) {
					   imageWidth = imageRole;
				   }
			   }
			   var divWidth = $(this).width();
			   var fadeout = $(this).find('.widget-catalog-page-fadeout-container').width();
			   if(imageWidth != null) {
				   var fadeout_imagewidth = imageWidth + fadeout;
				   if(fadeout_imagewidth > divWidth || fadeout == 415 ) {
					   var compliace_change = divWidth-(imageWidth+30);
					   $(this).find('.widget-catalog-page-fadeout-container').css('max-width',compliace_change);
				   }
			   } 
			   else {
				   if(fadeout != 415) {
					   if(fadeout > divWidth ) {
						   var value = divWidth-20;
						   $(this).find('.widget-catalog-page-fadeout-container').css('max-width',value);
					   } 
				   }else {
					   value = divWidth-20;
					   $(this).find('.widget-catalog-page-fadeout-container').css('max-width',value);
				   }
			   }
		   });
	   }
	// $('.limit-title').trunk8(trunk8.catalog_title);
	// $('.limit-desc').trunk8(trunk8.catalog_desc);
	// $('#paintContent .content-title').trunk8(trunk8.catalog_title);
	// $('#paintContent .content-description').trunk8(trunk8.catalog_desc);
		$('.catalog-criteria-refine-icon').addClass('click-binded').mouseenter(function(){
		//console.log('mouseenter');
			obj.showHideFilterCriteria(1);
			datePickerOpen = false;
			//afterRenderRefineIcon();
			
			//Added by vetrivel.P for #0079070 
			if ($('#filterlist_language').val() > 6) {
				$('#paintLanguagescroll').jScrollPane({}).data('jsp').destroy();
				$('#paintLanguagescroll').jScrollPane();
			}
			if($('#filterlist_country').val() > 6){
				$('#paintCountryscroll').jScrollPane({}).data('jsp').destroy();
				$('#paintCountryscroll').jScrollPane();
			}
			if ($('#jobrole_list').val() > 4) {
				$('#paintJobrole').jScrollPane({}).data('jsp').destroy();
				$('#paintJobrole').jScrollPane({});
			}
			//Ended by vetrivel.P for #0079070 
			afterRenderRefineIcon('#paintContent');
		});
		$('#search-filter-content').mouseenter(function(){
			if(!$("#ad_startdate1, #ad_startdate2").datepicker("widget").is(":visible")) {
				datePickerOpen = false;
			}
		});
		$('.catalog-criteria-refine-icon, #search-filter-content').mouseout(function(event){
				try{
					afterRenderRefineIcon('#paintCriteriaResults');/*Viswanathan added for #78031 */
//					console.API;
//
//					if (typeof console._commandLineAPI !== 'undefined') {
//					    console.API = console._commandLineAPI; //chrome
//					} else if (typeof console._inspectorCommandLineAPI !== 'undefined') {
//					    console.API = console._inspectorCommandLineAPI; //Safari
//					} else if (typeof console.clear !== 'undefined') {
//					    console.API = console;
//					}
//
//					console.API.clear();
//					 console.log('target-',$(event.target));
//					 console.log('relatedTarget-',$(event.relatedTarget));
//					 console.log('relatedTarget-',$(event.target));
					var comingfrom = event.toElement || event.relatedTarget;

					var isVtip = ($(comingfrom).attr('id') == 'vtip' || $(comingfrom).attr('id') == 'vtipArrow');
					var isAutoCompl = ($(comingfrom).parents('.ac_results').length > 0 || $(comingfrom).is('.ac_results')) ? true : false;
					// console.log('comingfrom-',comingfrom, $(comingfrom).attr('id'), isVtip, isAutoCompl);
					// if(!isVtip && $(comingfrom).parents('#search-filter-content').length == 0) {
				if(!isVtip && !isAutoCompl && $(comingfrom).parents('#search-filter-content').length == 0 
					&& !$(comingfrom).is('.block-title-middle') && $(comingfrom).parents('.catalog-criteria-refine-icon').length == 0
					&& !$(comingfrom).is('.catalog-criteria-refine-icon') && (!datePickerOpen)) {
						// $('#search-filter-content-wrapper').hide();
						obj.showHideFilterCriteria(0);
						$('.ac_results').hide();
					// console.log('closed');
					}
				} catch(e){
					 //console.log(e, e.stack);
				}
		});
		obj.collapseFilters('filters-state');
		if(filtersAdded == false) {
			var filters = obj.getGetJSONCookie('catalogAppliedFilters');
			
			// console.log('before filters added - ', filters);
			var langFilters = filters.filter(function(item) {
				return item.indexOf('lrn_srch_lang_cre_sys_lng_') != -1;
			});
/* 			var seachString = obj.searchActionCheck();
			if(seachString != '' && seachString != undefined && seachString != null) {
				filters = ['top-search'];
				obj.getGetJSONCookie('catalogAppliedFilters', filters);
			} else  */
			if(filters.length == 0 || langFilters.length == 0) {
				//if no filters (other than language) or no language filters applied, user preferred langugae should be applied
				//check for default set filters such as language filters
				var filterId = $('.lang-others:checked').data('filter-id');
				if(filterId !== undefined) {
					filters.push(filterId);
					obj.getGetJSONCookie('catalogAppliedFilters', filters);
				}
			}
			obj.addFilters(filters);
			filtersAdded = true;
		}
		$("#paintContentResults").showmore({
			showAlways: true,
			'grid': "#paintContentResults",
			'gridWrapper': '#searchRecordsPaint',
			'showMore': '#paintCatalog-show_more'
		});
		
	}catch(e){
			// to do
			// window.console.log(e, e.stack);
		}
	},	
	// To show a Message in the format of box 
	preAssMsgBox : function(enrID,vodType){
		try{
			var callMsgFlag = true;
			var contentLaunchFlag =true;
			if(document.getElementById('launch-content-container')){
				var dataObj = '';
				if(document.getElementById('paintContentResults'))
					var dataObj = $("#paintContentResults").jqGrid('getGridParam', 'postData');
				if(document.getElementById('tbl-paintCatalogContentResults'))
					var dataObj = $("#tbl-paintCatalogContentResults").jqGrid('getGridParam', 'postData');
				if(document.getElementById('paint-classes-list'))
					var dataObj = $("#paint-classes-list").jqGrid('getGridParam', 'postData');
				if(dataObj.enrollmentId!='' && dataObj.enrollmentId!=undefined)
					var callMsgFlag = false;
			}
			if(callMsgFlag){
			    $('#commonMsg-wizard').remove();
			    var html = '';
			    html+='<div id="commonMsg-wizard" style="display:none; padding: 0px;">';
			    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
			    html+='<tr><td height="30"></td></tr>';
			    html+='<tr>';
		    	html+= '<td align="center"><span class="select-greyed-out-text">'+SMARTPORTAL.t('MSG708')+'</span></td>';
			    html+='</tr>';
			    html+='</table>';
			    html+='</div>';
			    $("body").append(html);
			    var closeButt={};
		    	closeButt[Drupal.t('LBL123')]=function(){ 
		    		$(this).dialog('destroy');
		    		$(this).dialog('close');
		    		$('#commonMsg-wizard').remove();
		    		contentLaunchFlag=false;
		    		if(vodType=='sinVod'){
		    			//console.log(enrID);
		    			$('#dummylaunch'+enrID).click();
		    		}else{
		    			eval($('#launch'+enrID).attr('alt'));
		    		}
		    	};
			    $("#commonMsg-wizard").dialog({
			        position:[(getWindowWidth()-500)/2,100],
			        bgiframe: true,
			        width:520,
			        resizable:false,
			        modal: true,
			        title: SMARTPORTAL.t('Assessment'),
			        buttons:closeButt,
			        closeOnEscape: false,
			        draggable:true,
			        overlay:
			         {
			           opacity: 0.9,
			           background: "black"
			         }
			    });
			   	$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
			    this.currTheme = Drupal.settings.ajaxPageState.theme;
				  if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass(' white-btn-bg-middle');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
					}
				if(this.currTheme == "expertusoneV2"){
			 	   $('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
			 	   $('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
			       $('#select-class-equalence-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
				   changeChildDialogPopUI('select-class-equalence-dialog');	
				  // $('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
				   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
				   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
				   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
				   }
				}
			 	else {
			 		$('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
				 	$('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
					changeChildDialogPopUI('select-class-equalence-dialog');	
					//$('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
				   }
			    $("#commonMsg-wizard").show();
				$('.ui-dialog-titlebar-close,.removebutton').click(function(){
					$("#commonMsg-wizard").remove();
					if(contentLaunchFlag)
						eval($('#launch'+enrID).attr('alt'));
					contentLaunchFlag=false;
			       // $('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
			    });
			}
		 }catch(e){
				// to do
		 }
	},
	selectFormCheckbox : function(inputStr) {
		try {
			var tempStr = inputStr.split('|');
			for (var tempVal in tempStr) {
				var tempLabel = tempStr[tempVal].trim();
				$("input:checkbox[value='"+tempLabel+"']").attr('checked', 'checked');
				$("input:checkbox[value='"+tempLabel+"']").parent().removeClass('checkbox-unselected').addClass('checkbox-selected');
			}
		} catch (e) {
			// to do	
		}
	},//Common function for calling dialog window. Param : title, message
	callMessageWindow : function(title,message){
		 try{
			if(title == 'registertitle'){
				title = Drupal.t('LBL721');
			}
			
		    $('#commonMsg-wizard').remove();
		    var html = '';
		    html+='<div id="commonMsg-wizard" style="display:none; padding: 0px;">';
		    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
		    html+='<tr><td height="30"></td></tr>';
		    html+='<tr>';
	    	html+= '<td align="center"><span class="select-greyed-out-text">'+SMARTPORTAL.t(unescape(message))+'</span></td>';
		    html+='</tr>';
		    html+='</table>';
		    html+='</div>';
		    $("body").append(html);
		    var closeButt={};
	    	closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#commonMsg-wizard').remove();};
		    $("#commonMsg-wizard").dialog({
		        position:[(getWindowWidth()-500)/2,100],
		        bgiframe: true,
		        width:520,
		        resizable:false,
		        modal: true,
		        title: SMARTPORTAL.t(title),
		        buttons:closeButt,
		        closeOnEscape: false,
		        draggable:true,
		        overlay:
		         {
		           opacity: 0.9,
		           background: "black"
		         }
		    });
		   	$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		    this.currTheme = Drupal.settings.ajaxPageState.theme;
			  if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass(' white-btn-bg-middle');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
				}
		    /*new dialog popUI Script*/
			 // $('#select-class-dialog').hide();  
		 	if(this.currTheme == "expertusoneV2"){
		     //changeDialogPopUI();
		 	   $('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
		 	   $('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
		       $('#select-class-equalence-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
			   changeChildDialogPopUI('select-class-equalence-dialog');	
			   $('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
			   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
			   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
			   }
			}
		 	else {
		 		$('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
			 	$('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
				changeChildDialogPopUI('select-class-equalence-dialog');	
				$('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
			   }
		    $("#commonMsg-wizard").show();	
			$('.ui-dialog-titlebar-close,.removebutton').click(function(){
				$("#commonMsg-wizard").remove();
		        //$('#select-class-dialog').show();
		        $('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
		    });
		 }catch(e){
				// to do
			}
		},
	
	setSortByValue : function(sortVal) {
		try {
			$('.sort-by-links li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
			});
			switch (sortVal) {
			case 'ZA':
				$('.type2').addClass('sortype-high-lighter');
				break;
			case 'Time':
				$('.type3').addClass('sortype-high-lighter');
				break;
			case 'ClassStartDate':
				$('.type4').addClass('sortype-high-lighter');
				break;
			case 'Mandatory':
				$('.type5').addClass('sortype-high-lighter');
				break;
			default:
				$('.type1').addClass('sortype-high-lighter');
				break;
			}
		} catch(e) {
			// to do
		}
	},

	fiveStarCheckCompStatus : function(node_id,entity_type,crs_level_class_id){
		try{
		if(typeof(crs_level_class_id)=='undefined') crs_level_class_id = '';
		Drupal.attachBehaviors();
		$("body").data("learningcore").disableFiveStarOnVoting();
		var obj = $("#lnr-catalog-search").data("lnrcatalogsearch");
		
	    //$('.fivestar-click').click(function() {
		$('#cls-node-'+node_id+' '+'.fivestar-click').click(function() {
			
	    	var fivestarClick = this;
	    	var fiveStarClass = $(this).attr('class');
	    	var pattern = /[0-9]+/;
	    	var rating = fiveStarClass.match(pattern);
	    	//var nodeId = $(this).parents("form").siblings("input:hidden").val();
	    	var nodeId = node_id;

	    	var param = {'rating':rating,'nodeId':crs_level_class_id,'entityType':entity_type};
	    	
			url = obj.constructUrl("learning/five-star-submit");
			$.ajax({
				type: "POST",
				url: url,
				data:  param,
				success: function(result){
				
	    			if(result.average_rating == "AlreadyRated"){
	    				return false;
		    			
	    			}else{
	    				$("body").data("learningcore").fiveStarCheck(nodeId,'cls-node-');	    			    
	    				var average_stars = (result.average.value * 5) / 100;
		    			average_stars = average_stars.toFixed(1);
		    			var voteText = (result.count.value == 1) ? result.votemsg : result.votesmsg;
		    			var avgRating = '<span class="total-votes"> (<span>'+result.count.value+'</span> '+voteText+')</span>';
		    			$(fivestarClick).parent("div").next("div.description").children("div.fivestar-summary").html(avgRating);	    				
	    			}
				}
		    });
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
	
	paintLPSearchIcons : function (cellvalue, options, rowObject) {	
		try{
		return rowObject['image'];
		}catch(e){
			// to do
		}
	},
	testReturn : function (cellvalue, options, rowObject) {	
		try{
		// console.log(cellvalue);
		// console.log(options);
		// console.log(rowObject);
		return (typeof rowObject['cell'] != 'undefined' ? rowObject['cell'] : rowObject);
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

	 priceSlider: function(priceMin,priceMax){
	  try{
		obj = this;
		var symbol = Drupal.settings.user_prefference.currency_sym;
		if(symbol == "")
				symbol = ""; // 0056924 - default $ removal
		if(priceMax == 0){
			$('#search_price').css('display','none');
		} else {
			$( "#price-slider-range" ).slider({
				range: true,
				min: priceMin,
				max: priceMax,
				values: [ priceMin, priceMax ],
				slide: function( event, ui ) {
			
					$( "#price-slide-left" ).val( symbol + ui.values[ 0 ]);
					 $( "#price-slide-right" ).val( symbol + ui.values[ 1 ]);
					$.cookie("priceLftCkValue", ui.values[ 0 ]);
					$.cookie("priceRgtCkValue", ui.values[ 1 ]);
				},
				change: function(e,ui){
					if (e.originalEvent) {
						$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this);
					}
			    }
			});
			$( "#price-slide-left" ).val( symbol + $( "#price-slider-range" ).slider( "values", 0 ));
			$( "#price-slide-right" ).val( symbol + $( "#price-slider-range" ).slider( "values", 1 ));
			
			var priceLft = ''; var priceRgt = '';
			priceLft 	= $.cookie("priceLftCkValue");
			priceRgt 	= $.cookie("priceRgtCkValue");
			priceLft 	= (priceLft=='' || priceLft==null)?'':priceLft.replace("", "");
			priceRgt 	= (priceRgt=='' || priceRgt==null)?'':priceRgt.replace("", "");
			priceLft 	= priceLft==''?priceMin:priceLft;
			priceRgt 	= priceRgt==''?priceMax:priceRgt;
				
			$( "#price-slider-range" ).slider({ values: [priceLft, priceRgt] });
			$( "#price-slide-left" ).val(symbol +priceLft);
			$( "#price-slide-right" ).val(symbol +priceRgt);
	        $("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('lnr-catalog-search');
	        $("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('searchRecordsPaint');
	        /*if(this.currTheme == "expertusoneV2"){
	          $('#search_price #paintPrice #price-slider-range a:last').css('margin-left','-8px');
	        }*/

		}
	  }catch(e){
			// to do
		}
	},  

	clearPriceSlider: function(searchAction) {
		try{
			if(typeof searchAction == 'undefined') {
				var searchAction = true;
			}
			obj = this;
			var priceMin = $('div#price-slider-range').slider("option", "min");
			var priceMax = $('div#price-slider-range').slider("option", "max");
			//reset cookie values which has from and to price values
			$.cookie("priceLftCkValue", "");
			$.cookie("priceRgtCkValue", "");
			//destroy and reinitialize price slider
			$('div#price-slider-range').slider("destroy");
			obj.priceSlider(priceMin, priceMax);
			if(searchAction) {
				obj.searchAction('clearprice');
			}
		} catch(e){
			// to do
			// window.console.log(e, e.stack);
		}
	},
	
	locationEnterKey : function(){
		try{
		 obj = this;
		 $("#srch_criteria_location").keyup(function(event){
			 if(event.keyCode == 13){
				 obj.searchAction('location');
			 }
		 });
		}catch(e){
			// to do
		}
	},

	paintLocationAutocomplete : function(){
	  try{
		$('#srch_criteria_location').autocomplete(
			"/?q=learning/location-autocomplete",{
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
	paintAutocomplete: function(inputId, ValuesEnabled) {
	    try {
	       $("#"+inputId).autocomplete(
	    		   ValuesEnabled, {
	            	minLength: 2,

	            });
	       $("#"+inputId).focus(function(event) {
	                try {
	                    if ($(this).val() == Drupal.t("LBL304")) {
	                        $(this).css({
	                                'color': '#474747',
	                                'font-size': '12px'
	                            })
	                            .val('');
	                    }
	                } catch (e) {
	                  //  console.log(e, e.stack);
	                }
	            })
	            .blur(function(event) {
	                try {
	                    if ($(this).val() == '') {
	                        $(this).css({
	                                'color': '#999999',
	                                'font-size': '12px'
	                            })
	                            .val(Drupal.t('LBL304'));
	                    }
	                    if ($(this).val() != Drupal.t('LBL304')) {
	                        $(this).css({
	                            'color': '#333333',
	                            'font-size': '12px'
	                        });
	                    }
	                } catch (e) {
	                  //  console.log(e, e.stack);
	                }
	            });
	    } catch (e) {
	        // to do
	      // console.log(e, e.stack);
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
				if(reqTitleSearch.length == 1){
                    reqTitleSearch=searchStr.split("%7C");
				}
				 passSearchTitle = reqTitleSearch[0];
				 passSearchValue = reqTitleSearch[1];
			}
		}
		
		passSearchValue = passSearchValue.replace('@@','/');

		var searchTerm = $('#search_searchtext').val(unescape(passSearchValue)).val();
		if((searchTerm.toLowerCase()) != (Drupal.t("LBL304").toLowerCase())) {
		    $('#search_searchtext').css({
			'color': '#333333',
			'font-size': '13px'
		    });
		    //$(".eol-search-clearance").show();
		}
		
		if((passSearchValue.toLowerCase())==(Drupal.t('LBL304').toLowerCase()))
			return '';
		else
			return unescape(passSearchValue);
	 }catch(e){
			// to do
		}
	},
	
	hightlightedText : function(event, ID,textType) {
	 try{	
			 //		 textType = unescape(textType);
		var crrTheme = Drupal.settings.ajaxPageState.theme;
		var fontStyle = (crrTheme == 'expertusoneV2')?'normal':'italic';
		if(event.type == "blur"){
   			if($("#"+ID).val() != textType) {
        		$("#"+ID).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$("#"+ID).val(textType).css('color','#999999').css('fontSize','10px').css('fontStyle',fontStyle);
    		}
		}
		else if(event.type == "focus"){
		   			if($("#"+ID).val() != textType) {
		   				$("#"+ID).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
		    		}
		    		if($("#"+ID).val() == textType) {
		        		$("#"+ID).val('').css('color','#333333').css('fontSize','11px').css('fontStyle',fontStyle);
		    		}
				}
				else if (event.type == "change"){
		   			if($("#"+ID).val() != textType) {
		        		$("#"+ID).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
		    		}
		    		if($("#"+ID).val() == '') {
		        		$("#"+ID).val(textType).css('color','#999999').css('fontSize','11px').css('fontStyle',fontStyle);
		    		}
				}
			/*$("#"+ID).blur(function(){
				console.log('in blur '+$("#"+ID).val());
				console.log('in blur '+textType);
	   			if($("#"+ID).val() != textType) {
        		$(this).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(textType).css('color','#999999').css('fontSize','10px').css('fontStyle',fontStyle);
    		}
		});
		$("#"+ID).focus(function(){
				console.log('in focus '+$("#"+ID).val());
				console.log('in focus '+textType);
   			if($("#"+ID).val() != textType) {
        		$(this).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == textType) {
        		$(this).val('').css('color','#333333').css('fontSize','11px').css('fontStyle',fontStyle);
    		}
		});
		$("#"+ID).change(function(){
   			if($("#"+ID).val() != textType) {
        		$(this).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(textType).css('color','#999999').css('fontSize','11px').css('fontStyle',fontStyle);
    		}
			});*/
	 }catch(e){
			// to do
		}
	},	
	
	dateValidationCheck : function() {
	 try{	
		var v1=$('#ad_startdate1').val();
		var v2=$('#ad_startdate2').val();
				
		if($('#ad_startdate1').val() == 'Start: mm-dd-yyyy')
			v1 = '';

		if($('#ad_startdate2').val() == 'End: mm-dd-yyyy')
			v2 = '';

		var v1Split = v1.split('-');
		var v2Split = v2.split('-');
		var date1 = v1Split[2] + '-' + v1Split[0] + '-' + v1Split[1];
		var date2 = v2Split[2] + '-' + v2Split[0] + '-' + v2Split[1];	
		
		/*
		
		var d1=new Date(date1);
		var d2=new Date(date2);
		var end_date_less_diff= d2.valueOf() - d1.valueOf();		
		var now = new Date();
		var dateString = now.getFullYear() + "-" + now.getMonth() + "-" + now.getDate() ;
		var currdate=new Date(dateString);
		var start_date_less_currdate =  d1.valueOf() - currdate.valueOf();
		var end_date_less_currdate =  d2.valueOf() - currdate.valueOf();
		
		*/
		
		var d1=new Date(date1);
		var d2=new Date(date2);
		
		var currentTime = new Date();
		var month = currentTime.getMonth() + 1;
		var day = currentTime.getDate();
		var year = currentTime.getFullYear();
		
		var date3 = year + '-' + month + '-' + day;
		var today = new Date(date3);
				

		var end_date_less_diff= d2 - d1;		
		var start_date_less_currdate =  d1 - today;
		var end_date_less_currdate =  d2 - today;

		$("#date-validate-newid").remove();

		if((v1!='') && (v2!='')) {			

			if (start_date_less_currdate < 0 ) {
				
				 newDiv=document.createElement('div');
				 newDiv.id= "date-validate-newid";
				 $(newDiv).css({'color':'red','display':'none','float':'left','font-size':'11px'});
				 $(newDiv).html(SMARTPORTAL.t("Start date cannot be before current date."));		
				 $('.catalog-date-format').before(newDiv);
				 $("#date-validate-newid").show();
				 
			} else if (end_date_less_diff < 0){
				
				 newDiv=document.createElement('div');
				 newDiv.id= "date-validate-newid";
				 $(newDiv).css({'color':'red','display':'none','float':'left','font-size':'11px'});
				 $(newDiv).html(SMARTPORTAL.t("End date cannot be less than the Start date."));
				 $('.catalog-date-format').before(newDiv);
				 $("#date-validate-newid").show();
				 
			} else {				
				 this.searchAction('date');
			}
		} else if ((v1!='') && (v2=='')) {

			if (start_date_less_currdate < 0 ) {
				
				newDiv=document.createElement('div');
				newDiv.id= "date-validate-newid";
				$(newDiv).css({'color':'red','display':'none','float':'left','font-size':'11px'});
				$(newDiv).html(SMARTPORTAL.t("Start date cannot be before current date."));		
				$('.catalog-date-format').before(newDiv);
				$("#date-validate-newid").show();
				
			} else {				
				this.searchAction('date');
			}
			
		} else if ((v2!='') && (v1=='')){

			if (end_date_less_currdate < 0 ) {
				
				newDiv=document.createElement('div');
				newDiv.id= "date-validate-newid";
				$(newDiv).css({'color':'red','display':'none','float':'left','font-size':'11px'});
				$(newDiv).html(SMARTPORTAL.t("End date cannot be before current date."));		
				$('.catalog-date-format').before(newDiv);
				$("#date-validate-newid").show();
				
			} else {				
				this.searchAction('date');
			}
			
		} else {
			this.searchAction('date');
		}
	 }catch(e){
			// to do
	 }
	},
	
	customRangeDate : function (input) 
	{ 
		try{
	        var dateMin = null;
	        var dateMax = null;

	        if (input.id == "ad_startdate1" && $("#ad_startdate2").datepicker("getDate") != null)
	        {
	        		dateMin = new Date();
	                dateMax = $("#ad_startdate2").datepicker("getDate");
	                dateMax.setDate(dateMin.getDate() + 20000);
	                                      
	        }
	        else if (input.id == "ad_startdate2")
	        {
	                dateMax = new Date();
	                if ($("#ad_startdate1").datepicker("getDate") != null)
	                {
	                        dateMin = $("#ad_startdate1").datepicker("getDate");
	                        dateMax = $("#ad_startdate1").datepicker("getDate");
	                        dateMax.setDate(dateMax.getDate() + 20000); 
	                }
	        }
	        
	     return {
	    	 minDate: dateMin, 
	    	 maxDate: dateMax
	   	 }; 
		}catch(e){
			// to do
		}
		
	},
	displayTagTip : function(elementid, messagecontent){
		try{
			$('.qtip-contentWrapper').css('border','0px none');
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
	
	paintAfterReady : function() {		
		try{
		var dates = $('#ad_startdate1').datepicker({
			  duration: '',
			  showTime: false,
			  constrainInput: false,
			  stepMinutes: 5,
			  stepHours: 1,
			  time24h: true,
			  dateFormat: "mm-dd-yy",
			  buttonImage: themepath+'/expertusone-internals/images/calendar_icon.JPG',
			  buttonImageOnly: true,
			  firstDay: 0,
			  showOn: 'both',
			  buttonText:Drupal.t('LBL675'),
			  showButtonPanel: true,
			  changeMonth: true,
			  changeYear: true, 
			  beforeShow: $("#lnr-catalog-search").data("lnrcatalogsearch").customRangeDate
		});
		var dates = $('#ad_startdate2').datepicker({
			  duration: '',
			  showTime: false,
			  constrainInput: false,
			  stepMinutes: 5,
			  stepHours: 1,
			  time24h: true,
			  dateFormat: "mm-dd-yy",
			  buttonImage: themepath+'/expertusone-internals/images/calendar_icon.JPG',
			  buttonImageOnly: true,
			  firstDay: 0,
			  showOn: 'both',
			  buttonText:Drupal.t('LBL676'),
			  showButtonPanel: true,
			  changeMonth: true,
			  changeYear: true, 
			  beforeShow: $("#lnr-catalog-search").data("lnrcatalogsearch").customRangeDate
		});
		if(document.getElementById('lnr-catalog-search')) {
			$('#first_pager').click( function(e) {
				
				if(!$('#first_pager').hasClass('ui-state-disabled')) {
					$('#paintContentResults').hide();
					$('#gview_paintContentResults').css('min-height','100px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
				}
			});
			$('#prev_pager').click( function(e) {
				
				if(!$('#prev_pager').hasClass('ui-state-disabled')) {
					$('#paintContentResults').hide();
					$('#gview_paintContentResults').css('min-height','100px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
				}
			});
			$('#next_pager').click( function(e) {
				
				if(!$('#next_pager').hasClass('ui-state-disabled')) {
					$('#paintContentResults').hide();
					$('#gview_paintContentResults').css('min-height','100px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
				}
			});
			$('#last_pager').click( function(e) {
		
				if(!$('#last_pager').hasClass('ui-state-disabled')) {
					$('#paintContentResults').hide();
					$('#gview_paintContentResults').css('min-height','100px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
				}
			});
			$('.ui-pg-selbox').bind('change',function() {
				$('#paintContentResults').hide();
				$('#gview_paintContentResults').css('min-height','100px');
				$("#lnr-catalog-search").data("lnrcatalogsearch").hidePageControls(false);
				$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
			});			
			$(".ui-pg-input").keyup(function(event){				
				if(event.keyCode == 13){
					$('#paintContentResults').hide();
					$('#gview_paintContentResults').css('min-height','100px');
				  $("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
				}
			});
			if(this.currTheme == "expertusoneV2"){				
					$('.page-show-prev').bind('click',function() {
						if(parseInt($("#pg_pager .page_count_view").attr('id')) < 0){
							$("#pg_pager .page_count_view").attr('id','0');
						}else{
							$('#paintContentResults').hide();
							$('#gview_paintContentResults').css('min-height','100px');
							$("#lnr-catalog-search").data("lnrcatalogsearch").hidePageControls(false);
							$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
						}
					});
				
					$('.page-show-next').bind('click',function() {	
						if(parseInt($("#pg_pager .page_count_view").attr('id')) >= parseInt($("#pg_pager .page-total-view").attr('id'))){
							$("#pg_pager .page_count_view").attr('id',($("#pg_pager .page_count_view").attr('id')-1));
						}else{
							$('#paintContentResults').hide();
							$('#gview_paintContentResults').css('min-height','100px');
							$("#lnr-catalog-search").data("lnrcatalogsearch").hidePageControls(false);
							$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
						}
					});
				
			}
		}
		}catch(e){
			// to do
		}
	},
	showSelectClass : function(userId,courseId){
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var chgClassWidth = 688 ;
	 	if(this.currTheme == "expertusoneV2"){
			chgClassWidth = 750 ;	
		}
		$("#lnr-catalog-search").data("lnrcatalogsearch").defaults.catStart = true;
		//this.changeClsEnrollId = enrollId;
		this.renderSelectClassPopup(userId);
		$('#paintCatalogContentResults'+userId).css('min-height','auto').css('overflow','visible');
		this.createLoader('paintCatalogContentResults'+userId);
		$('#loaderdivpaintCatalogContentResults'+userId).css('height','250px');
		
		var nAgt = navigator.userAgent;
		var verOffset ;
		var DetailsW = 465;
		var actionW = 85;
	
		// In Chrome, the true version is after "Chrome" 
		if ((verOffset=nAgt.indexOf("Chrome"))!=-1) {
			 DetailsW = 420;
			 actionW = 100;
		}
		//	alert(10000);
		var obj = this;
		var objStr = '$("#lnr-catalog-search").data("lnrcatalogsearch")';
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback==true){
			$("#tbl-paintCatalogContentResults").jqGrid({
				url:this.constructUrl("learning/courselevel-search/catalog/"+userId+ "/" +courseId),
				datatype: "json",
				mtype: 'GET',
				colNames:['Icons','Details','Action'],
				colModel:[ {name:'Icons',index:'Icons', title:false,'widgetObj':objStr,formatter: obj.paintSelectClsSearchIcons},
				           {name:'Details',index:'Details', title:false,'widgetObj':objStr,formatter: obj.paintSelectClsSearchResults},
				           {name:'Action',index:'Action', title:false,'widgetObj':objStr,formatter: obj.paintSelectClsSearchResultsAction}],
				rowNum:5,
				rowList:[5,10,15],
				pager: '#pager1',
				viewrecords:  true,
				emptyrecords: "",
				sortorder: "desc",
				toppager:false,
				height: 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadui:false,
				loadComplete:obj.callbackCatalogSelectClassLoader,
				gridComplete:obj.callbackGridSelectClassComplete
			}).navGrid('#pager1',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}else{
			$("#tbl-paintCatalogContentResults").jqGrid({
				url:this.constructUrl("learning/courselevel-search/catalog/"+userId+ "/" +courseId),
				datatype: "json",
				mtype: 'GET',
				colNames:['Icons','Details','Action'],
				colModel:[ {name:'Icons',index:'Icons', title:false, width:55,'widgetObj':objStr,formatter: obj.paintSelectClsSearchIcons},
				           {name:'Details',index:'Details', title:false, width:DetailsW,'widgetObj':objStr,formatter: obj.paintSelectClsSearchResults},
				           {name:'Action',index:'Action', title:false, width:actionW,'widgetObj':objStr,formatter: obj.paintSelectClsSearchResultsAction}],
				rowNum:5,
				rowList:[5,10,15],
				pager: '#pager1',
				viewrecords:  true,
				emptyrecords: "",
				sortorder: "desc",
				toppager:false,
				height: 'auto',
				width: chgClassWidth,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadui:false,
				loadComplete:obj.callbackCatalogSelectClassLoader,
				gridComplete:obj.callbackGridSelectClassComplete
			}).navGrid('#pager1',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}
			$("#shareoption #bubble-face-table" ).hide();
			$("#shareoption .qtip-popup-visible" ).hide();		
			$("#multishareoption #bubble-face-table" ).hide();
			$("#multishareoption .bottom-qtip-tip-up-visible" ).hide();				
		this.paintCatalogSelectClassAfterReady(userId);
		}catch(e){
			// to do
		}
	},
	
	paintSelectClsSearchIcons : function (cellvalue, options, rowObject) {	
		try{
		return rowObject['image'];
		}catch(e){
			// to do
		}
	},

	paintSelectClsSearchResults : function(cellvalue, options, rowObject) {	
		try{
		return rowObject['details'];
		}catch(e){
			// to do
		}
	},

	paintSelectClsSearchResultsAction : function (cellvalue, options, rowObject) {
		try{
		return rowObject['action'];
		}catch(e){
			// to do
		}
	},
	
	
	/*
	 * To show the pop-up with the rendered classes
	 */
	renderSelectClassPopup : function(id) {
		try{
		var obj	= this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
			var chgclassqtipWidth = 750 ;	
		}else{
			var chgclassqtipWidth = 712;
	    }
	 	//Embed widget related work (Create flexible grid)
	 	if(Drupal.settings.widget.widgetCallback==true){
		    var iwidth = Math.max(document.documentElement.clientWidth, window.innerWidth || 0);
		    chgclassqtipWidth = iwidth - 40;
		}
		var html = "<div id='paintCatalogContentResults"+id+"'></div>";
        $("#paintCatalogContentResults"+id).remove();
        $('body').append(html);
        var nHtml = "<div id='show_expertus_message'></div><div id='paintCatalogHeader'></div><div id='catalog-no-records' style='display: none'></div><table id='tbl-paintCatalogContentResults'></table><div id='paintCatalogFooter'><div id='pager1' style='display:none;'></div></div>";
        $('#paintCatalogContentResults'+id).html(nHtml);
        var closeButt={};
        $("#paintCatalogContentResults"+id).dialog({
        	autoResize: true,
        	position:[(getWindowWidth()-chgclassqtipWidth)/2,100],
            bgiframe: true,
            width:chgclassqtipWidth,
            resizable:false,
            draggable:false,
            closeOnEscape: false,
            modal: true,
            title:SMARTPORTAL.t('<span class="myteam-header-text">' + Drupal.t("LBL716") +'</span><div id="search-cat-keyword"></div>'),
            buttons: closeButt,
            close: function(){
                $("#paintCatalogContentResults"+id).remove();
                $("#select-class-dialog").remove();
                if($('.location-session-detail-clone').size()>0){
                	$('.location-session-detail-clone').remove();
                }
                //obj.viewLearning(id,rowObj);
            },
            overlay: {
 	           opacity: 0.5,
	           background: "#000000"
             },
            open: function(){
            	//Embed widget related work (Create flexible grid)
            	if((typeof(Drupal.settings.widget.widgetCallback) == "undefined" && Drupal.settings.widget.widgetCallback == null) || Drupal.settings.widget.widgetCallback == false) {
            		$(".ui-widget-overlay").eq(-1).css("width",getWindowWidth()-1+"px");
            	}
             }
        });
        //$('.ui-dialog').css('border','solid 10px #5F5F5F');
        $('.ui-dialog').wrap("<div id='select-class-dialog'></div>");
        $("#paintCatalogContentResults"+id).css("position","");
        this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
	 		$('#select-class-dialog').find('.ui-dialog-content').addClass('assign-learning-team');	
	 		$('#select-class-dialog').find('.ui-dialog').css("overflow","visible");
	 		changeDialogPopUI();
	 		$('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
	   }
		}catch(e){
			// to do
		}
    },
	
	/*
	 * Call back after rendering the classes in the pop-up
	 */
    callbackCatalogSelectClassLoader: function(response, postdata, formid){
     try{	
	  $("#tbl-paintCatalogContentResults").show();
	  $('#select-class-dialog').find('#tbl-paintCatalogContentResults > tbody > tr > td:first-child').addClass('assign-learn-first-row');
		$('#select-class-dialog').find('#tbl-paintCatalogContentResults > tbody > tr > td:nth-child(2)').addClass('assign-learn-second-row');
		$('#select-class-dialog').find('#tbl-paintCatalogContentResults > tbody > tr > td:last-child').addClass('assign-learn-last-row');
		
	  var recs = parseInt($("#tbl-paintCatalogContentResults").getGridParam("records"),10);
      if (recs == 0) {
        $('#catalog-no-records'). css('display','block');
        var html = Drupal.t("MSG381")+'.';
        $("#catalog-no-records").html(html);            
      } else {
        $('#catalog-no-records'). css('display','none');
        $("#catalog-no-records").html("");
      }    
      var obj = $("#lnr-catalog-search").data("lnrcatalogsearch");

      var reccount = parseInt($("#tbl-paintCatalogContentResults").getGridParam("reccount"), 10);
	  // Show pagination only when search results span multiple pages
	  var hideAllPageControls = true;
	  if (recs > reccount) { // 5 is the least view per page option.
	    hideAllPageControls = false;
	    $('#select-class-dialog .popupBotLeftCorner').hide();
	  }

      
      if (recs <= reccount) {
        obj.hideCatalogSelectClassPageControls(hideAllPageControls);
        $('#select-class-dialog .popupBotLeftCorner').show();
      }
      else {
        obj.showCatalogSelectClassPageControls();
        $('#select-class-dialog .popupBotLeftCorner').hide();
      }

      if($("#lnr-catalog-search").data("lnrcatalogsearch").defaults.catStart) {
		$("#paintCatalogHeader").html(response.header);
		$("#myteam-find-trng-sort-display").show();
		$("#lnr-catalog-search").data("lnrcatalogsearch").defaults.catStart = false;
      }
      var curHeight = parseInt($(".ui-dialog").eq(-1).css("height")) + parseInt($(".ui-dialog").eq(-1).css("top"));
      var curOlay = parseInt($(".ui-widget-overlay").eq(-1).css("height"));
	  if(curOlay < curHeight) {
	    $(".ui-widget-overlay").eq(-1).css("height", (curHeight + 50) + "px");
	  }
	//Embed widget related work (Create flexible grid)
	 if(Drupal.settings.widget.widgetCallback==true){
		 if(Drupal.settings.ajaxPageState.theme != "expertusoneV2"){
			 var leftval = $('#select-class-dialog .ui-dialog').offset().left;
			 $('#select-class-dialog .ui-dialog').css('left',Math.floor(leftval/2)+'px');
		 }
	 }
	  $("#tbl-paintCatalogContentResults tr.ui-widget-content:last td").css('border','0px none');  
	  $("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('paintCatalogContentResults'+response.userId);
	  
	// call for updated content result display in launch-content-container register and launch feature
	  //console.log('test event for one click launch');
	  if(document.getElementById('launch-content-container')){
		  //console.log('hrere');
		  var dataObj = $("#tbl-paintCatalogContentResults").jqGrid('getGridParam', 'postData');
		  //console.log('pst data received');
		  //console.log(data);
		  if(dataObj.enrollmentId!='' && dataObj.enrollmentId!=undefined){
			  $("#launch"+dataObj.enrollmentId).click();
		  }
	  }
	  vtip();	
	  resetFadeOutByClass('#tbl-paintCatalogContentResults','find-training-list-course','line-item-container','catalog');
	  $('.limit-title').trunk8(trunk8.catalogclsmul_title);
	  $('.limit-desc').trunk8(trunk8.catalogclsmul_desc);
	  scrollBar_Refresh('catalog');
     }catch(e){
			// to do
		}
	},
	
	callbackGridSelectClassComplete: function(response, postdata, formid){
		try {
			$('.limit-title').trunk8(trunk8.catalogclsmul_title);
			$('.limit-desc').trunk8(trunk8.catalogclsmul_desc);	
			scrollBar_Refresh('catalog');
		} catch(e) {
			// to do
		}
	},
	
	hideCatalogSelectClassPageControls : function(hideAll) {
	 try{	
	  var lastDataRow = $('#tbl-paintCatalogContentResults tr.ui-widget-content').filter(":last");
	  if (hideAll) {
		$('#pager-lp').hide();
		$('#pager1').hide();
	    if (lastDataRow.length != 0) {
	      lastDataRow.children('td').css('border', '0 none');
	    }
	  }
	  else {
	    $('#pager1').show();
	    $('#pager1 #pager1_center #first_pager1').hide();
	    $('#pager1 #pager1_center #last_pager1').hide();
	    $('#pager1 #pager1_center #next_pager1').hide();
	    $('#pager1 #pager1_center #prev_pager1').hide();              
	    $('#pager1 #pager1_center #sp_1_pager1').parent().hide();
	  }
	 }catch(e){
			// to do
		}
	},
	  
	showCatalogSelectClassPageControls : function() {
	 try{	
	  $('#pager1').show();
	  $('#pager1 #pager1_center #first_pager1').show();
	  $('#pager1 #pager1_center #last_pager1').show();
	  $('#pager1 #pager1_center #next_pager1').show();
	  $('#pager1 #pager1_center #prev_pager1').show();              
	  $('#pager1 #pager1_center #sp_1_pager1').parent().show();
	 }catch(e){
			// to do
		}
	},
	/*
	 * action to be performed while clicking on pagination at Change Classes pop-up
	 */
	paintCatalogSelectClassAfterReady : function(userId) {
		try{
		
		if(document.getElementById('select-class-dialog') != null) {
			$('#first_pager1').click( function(e) {
				if(!$('#first_pager1').hasClass('ui-state-disabled')) {
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+userId);
				}
			});
			$('#prev_pager1').click( function(e) {
				if(!$('#prev_pager1').hasClass('ui-state-disabled')) {
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+userId);
				}
			});
			$('#next_pager1').click( function(e) {
				if(!$('#next_pager1').hasClass('ui-state-disabled')) {
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+userId);
				}
			});
			$('#last_pager1').click( function(e) {
				if(!$('#last_pager1').hasClass('ui-state-disabled')) {
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+userId);
				}
			});
			$('#pager1 .ui-pg-selbox').bind('change',function() {
				$('#tbl-paintCatalogContentResults').hide();
				$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
				$("#lnr-catalog-search").data("lnrcatalogsearch").hideCatalogSelectClassPageControls(false);
				$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+userId);
			});			
			$("#pager1 .ui-pg-input").keyup(function(event){				
				if(event.keyCode == 13){
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+userId);
				}
			});
		}
		}catch(e){
			// to do
		}
	},
	
	/*
	 * To list all matched classes on class/code autocomplete on class search at pop-up
	 * 'Change Classes' section
	 */
	paintCatSltClskeywordAutocomplete : function(userId,courseId){
		try{
		var obj	= this;
		var delType	= $('#myteam-cat-delivery-type-hidden').val();
		url = obj.constructUrl("learning/select-class/catalog-autocomplete/" + delType + '/' + userId + '/' + courseId + '/');
		$('#srch_criteria_catkeyword').autocomplete(
			url,{
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
	
	deliverySelectClsHideShow : function () {
		try{
		$('#myteam-cat-delivery-type-list').slideToggle();
		$('#myteam-cat-delivery-type-list li:last').css('border-bottom','0px none');	
		}catch(e){
			// to do
		}
	},
	
	deliveryTypeSelectClsText : function(dCode,dText,userId,courseId) {
		try{
		$('#myteam-cat-delivery-type-list').hide();
		$('#myteam-cat-delivery-type').text(dText);
		$('#myteam-cat-delivery-type-hidden').val(dCode);
		//$("#lnr-catalog-search").data("lnrcatalogsearch").catalogSearchSelectClassAction("","",userId,courseId);
		$("#lnr-catalog-search").data("lnrcatalogsearch").paintCatSltClskeywordAutocomplete(userId,courseId);
		}catch(e){
			// to do
		}
	},
	
	
	 /* To highlight the default text when there is no text, in search filters*/
	 
	highlightedSelectClassText : function(ID,textType) {
		try{
		$("#"+ID).blur(function(){
   			if($("#"+ID).val() != textType) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(textType).css('color','#999999').css('fontSize','12px').css('fontStyle','normal');
    		}
		});
		$("#"+ID).focus(function(){
   			if($("#"+ID).val() != textType) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == textType) {
        		$(this).val('').css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
    		}
		});
		$("#"+ID).change(function(){
   			if($("#"+ID).val() != textType) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(textType).css('color','#999999').css('fontSize','12px').css('fontStyle','normal');
    		}
		});
		}catch(e){
			// to do
		}
	},	
	
	 /* To call class search widget when enter class keywords on pop-up, 'Change Classes' section*/
	 
	catkeywordSelectClassEnterKey : function(userId,courseId){
		try{
		 obj = this;
		 $("#srch_criteria_catkeyword").keyup(function(event){
			 if(event.keyCode == 13){
				 obj.catalogSearchSelectClassAction('','',userId,courseId);
			 }
		 });
		}catch(e){
			// to do
		}
	},
	/*
	 * Action to performed during class search in the pop-up
	 */
	catalogSearchSelectClassAction : function(sortbytxt,className,userId,courseId) {
		try{
		var searchStr = '';
		/*-------Title-------*/
		var title 	  = $('#srch_criteria_catkeyword').val();
		if(title == Drupal.t("LBL545"))
			title='';
		else		
			$('#cat-title-clr').css('display','block');
		
		/*-------Delivery Type-------*/
		var dl_type 	  = $('#myteam-cat-delivery-type-hidden').val();
		if(dl_type == Drupal.t("LBL428"))
			dl_type='';
		else		
			$('#cat-title-clr').css('display','block');	
		
		/*-------Sort By-------*/
		var sortby = sortbytxt;
		if(title == undefined)
			title = "";
		searchStr	= '&title='+encodeURIComponent(title)+'&dl_type='+encodeURIComponent(dl_type)+'&sortby='+sortby;
		this.searchStrValue = searchStr;
		$("#lnr-catalog-search").data("lnrcatalogsearch").hideCatalogSelectClassPageControls(true);
		$('#tbl-paintCatalogContentResults').hide();		
        this.createLoader('paintCatalogContentResults'+userId);
        $('#loaderdivpaintCatalogContentResults'+userId).css('height','250px');
    	
		$('#tbl-paintCatalogContentResults').setGridParam({url: this.constructUrl("learning/courselevel-search/catalog/"+userId+"/" +courseId+ '/'+searchStr)});
	    $("#tbl-paintCatalogContentResults").trigger("reloadGrid",[{page:1}]);
	   //Highlight sort type added by yogaraja 
	    $('.'+className).parents('li').siblings().find('a').removeClass('sortype-high-lighter');
		if(className!=''){
			$('.'+className).addClass('sortype-high-lighter');
		}else{
			$('#paintCatalogHeader .catalog-type1').addClass('sortype-high-lighter');
		}
		}catch(e){
			// to do
		}
	},
	
	underlineTagAndTriggerSearch: function(tagName, currElem) {
		  try {
			$('.tagscloud-tag').css('text-decoration', '');
			$('.hovertag').css('text-decoration', '');
			$(currElem).css('text-decoration', 'underline');
			$('#srch_criteria_tag').val(tagName);
			this.searchAction('cloud-tag');
		  }
		  catch (e) {
		    // To Do
		  }
	},
	
	underlineTagCloudAndTriggerSearch: function(tagName, currElem, tagWeight) {
		  try {
			$('.tagscloud-tag').css('text-decoration', '');
			$('.hovertag').css('text-decoration', '');
			$(currElem).css('text-decoration', 'underline');
			$('#srch_criteria_tag').val(tagName);
			this.searchAction('cloud-tag');
			var tagpos = $.inArray($(currElem).html(),existtagArr,0);
			if(tagpos == -1){
				$tagText = $(currElem).html();
				existtagArr.unshift($tagText);
				existtagArr.pop();
				$('#tagcloudcontainerid').find('span:last-child').remove();
				$(currElem).html(descController('FADE OUT',$tagText,weightArr[tagWeight-1]));
				var selectedTag = $(currElem).parent().html();
				$('#tagcloudcontainerid').prepend('<span class="tagscloud-term">'+selectedTag+'</span>');
				$(currElem).html($tagText);
			}else if(tagpos >= 0){
				$tagText = $(currElem).html();
				$('#tagcloudcontainerid').find('span:nth-child('+(tagpos+1)+')').remove();
				if (tagpos > -1) {
					existtagArr.splice(tagpos, 1);
 				}
				existtagArr.unshift($tagText);
				$(currElem).html(descController('FADE OUT',$tagText,weightArr[tagWeight-1]));
				var selectedTag = $(currElem).parent().html();
				$('#tagcloudcontainerid').prepend('<span class="tagscloud-term">'+selectedTag+'</span>');
				$(currElem).html($tagText);
			}
		  }
		  catch (e) {
		    // To Do
		  }
	},
	truncateTitleDescription: function(trunk8options) {
		try{
		// return;
			if(trunk8options === undefined) {
				var trunk8options = [];
				trunk8options.push({'selector': '.content-title .limit-title', 'lines': trunk8.embedwidget_title});
				trunk8options.push({'selector': '.content-description .limit-desc', 'lines': trunk8.embedwidget_desc});
				$(trunk8options).each(function() {
				$(this.selector).trunk8('revert');
				$(this.selector).trunk8();
			});
			} else {
			$(trunk8options).each(function() {
				// $(this.selector).trunk8(this.lines);
				$(this.selector).trunk8(this.lines);
			});
	}
		} catch(e) {
			// window.console.log(e, e.stack);
		}
	},
	pinUnpinFilterCriteria: function(options,type) {
		try {
			var _this = this;
			// _this.toggleClass('unpinned');
			$('#paintContent').toggleClass('catalog-extended');
			////////////
			_this.showHideFilterCriteria(!$('#paintContent').hasClass('catalog-extended'));
			
			/* $('#search-filter-content-wrapper').appendTo('#paintContent .catalog-criteria-refine-icon')
			// .addClass('.pinned')
			.width($('#paintCriteriaResults').width())
			// .css({'position': 'absolute', 'z-index':10, 'display':'none'})

			$('#paintContent .catalog-criteria-refine-icon').mouseenter(function(event) {
				// $('#search-filter-content-wrapper').show();
				$('#paintContent .catalog-criteria-refine-icon').addClass('hover');
			});

			$('#paintContent .catalog-criteria-refine-icon').mouseleave(function(event){
				try{
					// console.log('target-',$(event.target));
					// console.log('relatedTarget-',$(event.relatedTarget));
					// console.log('relatedTarget-',$(event.target));
					var comingfrom = event.toElement || event.relatedTarget;

					var isVtip = ($(comingfrom).attr('id') == 'vtip' || $(comingfrom).attr('id') == 'vtipArrow');
					console.log('comingfrom-',comingfrom, $(comingfrom).attr('id'), isVtip);
					if(!isVtip && $(comingfrom).parents('.catalog-criteria-refine-icon').length == 0) {
						// $('#search-filter-content-wrapper').hide();
						$('#paintContent .catalog-criteria-refine-icon').removeClass('hover');
					}
				} catch(e){
					console.log(e, e.stack);
				}
			});
			 */
			
			////////////
			$('#page-container .searchcriteria-div').toggleClass('searchcriteria-div-unpinned');
			// $('.catalog-criteria-refine-icon:not(.click-binded)').addClass('click-binded').bind('click', _this.showHideFilterCriteria);
			// $('.catalog-criteria-refine-icon:not(.click-binded)').addClass('click-binded').bind('mouseenter mouseout', _this.showHideFilterCriteria);
			$("#paintContentResults").jqGrid('setGridWidth', ($('#paintContent').width() - 2));
			_this.truncateTitleDescription();
			var userId = _this.getLearnerId();
			var refineHidden = $('#paintContent').hasClass('catalog-extended');
			if(type != 'toggle') {	//user logged in
				$.ajax({
					type: "POST",
					url: _this.constructUrl("learning/save-preference/1"),
					data: {catalog_refine: (refineHidden ? 1 : 0)},
					success: function(result) {
						// console.log(result);
						if(userId==0)
							Drupal.settings.user_preferences.catalog_refine = refineHidden ? 1 : 0
						else
							Drupal.settings.user_preferences = result;
					}
				});
			}
			$('#paintContentResults .content-detail-code').find('.fadeout-applied').removeClass('fadeout-applied');
			setTimeout(function(){
				resetFadeOutByClass('#paintContentResults','content-detail-code','line-item-container','catalog');
			},100);
		} catch(e) {
			// window.console.log(e, e.stack);
		}
	},
	showHideFilterCriteria: function(show, toggleCompleteHandler) {
		try {
			if(show == true) {
				$('#page-container .searchcriteria-div').addClass('active');
			} else if(show == false) {
				$('#page-container .searchcriteria-div').removeClass('active');
			} else {
				$('#page-container .searchcriteria-div').toggleClass('active');
			}
			$('#paintCriteriaResults').find('.fade-out-title-container:not(.title-processed)').each(function() {
				if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
					$(this).find('.fade-out-image').remove();
				}
			}).addClass('title-processed');
		} catch(e) {
			// window.console.log(e, e.stack);
		}
	},
	collapseFilters: function(cookieName, count) {
		try {
			var obj = this;
			if(cookieName !== undefined && $.cookie(cookieName) !== undefined && $.cookie(cookieName) !== null) {
				//if cookie is given it will collapse/uncollape the given filters with given state
				var filters = JSON.parse($.cookie('filters-state'));
				// console.log('filters set ', filters);
				for(var handlerId in filters){
					// console.log(handlerId, filters[handlerId]);
					// console.log($('#'+handlerId).parents('.find-list-items').siblings('.catalog-criteria-filter-set'));
					if(filters[handlerId]) {	//uncollapse filter section
						$('#'+handlerId).removeClass('cls-hide')
							.addClass('cls-show')
							.parents('.find-list-items').siblings('.catalog-criteria-filter-set').show();
					} else {	//collapse filter section
						$('#'+handlerId).removeClass('cls-show')
							.addClass('cls-hide')
							.parents('.find-list-items').siblings('.catalog-criteria-filter-set').hide();
					}
				}
			} else {
				(count === undefined) && (count = -5);	//default select of last 5 filters if count is not given
				$('#paintCriteriaResults').find(".catalog-criteria-filter-set").slice(count).each(function(){
					var handlerId = $(this).siblings().find('.cls-show').attr('id');
					var targetId = $(this).attr('id');
					obj.showHide(handlerId, targetId, true);
				});
			}
		} catch(e) {
			// window.console.log(e, e.stack);
		}
	},
	parseParams: function(paramString) {
		try {
			var result = {};
			// "&title=&dl_type=&lg_type=cre_sys_lng_eng&all_lg_type=cre_sys_lng_eng|cre_sys_lng_gzh&ob_type=&location=&tag=&price=%24,$0-%24,$10000&startdate=&enddate=&jr_type=&all_jr_type=&cy_type=&all_cy_type=&sortby=undefined&mro_type=&all_mro_type=cre_sys_inv_com|cre_sys_inv_man&rating_type="
			paramString.split("&").forEach(function(part) {
				var item = part.split("=");
				result[item[0]] = decodeURIComponent(item[1]);
			});
			return result;
		} catch(e) {
			//window.console.log(e, e.stack);
		}
	},
	addFilters: function(filters) {
		try {
			var filtersList = [];
			if($.isArray(filters)) {
				filtersList = filters;
			} else {
				filtersList.push(filters);
			}
			
			var filterLength = 0;
			$(filtersList).each(function(index, filterId) {
				var selector = '[data-filter-id="'+filterId+'"]';
				var filter = $(selector);
				var filterId = filter.data('filter-id');
				var filterName = filter.data('filter-name');
				var displayName = filter.data('filter-label');
				var menustr = '<span class="cls-filtermenu-wrapper"><span class="cls-filtermenu">';
				var showFilter = false;
				if(filter.is(':checkbox')) {
					if(filter.is(':checked')) {
						var delAction = '<label data-filter-tag-name="'+filterName+'" class="enable-delete-icon" id="filter-delete-handler-'+filterId+'" for="'+filterId+'"></label>';
						if(filterName == 'rating') {
							displayName = '<span class="lrn_srch_rating">'+filter.parent().siblings('span.lrn_srch_rating').html()+'</span>';
						}
						menustr += displayName;
						showFilter = true;
						filterLength++;
					}
					
				} else if(filterName == 'price-slider') {
					var Lprice = $("#price-slide-left").val();
					if (Lprice != undefined && Lprice != 'undefined') {
						Lprice = encodeURIComponent(Lprice.charAt(0)) + Lprice.substring(1);
					}
					var Rprice = $("#price-slide-right").val();
					if (Rprice != undefined && Rprice != 'undefined') {
						Rprice = encodeURIComponent(Rprice.charAt(0)) + Rprice.substring(1);
					}
					var price = $.trim(Lprice + "-" + Rprice);
					price = price.replace(/ /g, "");
					if (price == 'undefined-undefined' || price == undefined - undefined || price == '-') {
						price = '';
					}
					var displayName = decodeURIComponent(price.replace(/-/g, " - "));
					var delAction = '<label data-filter-tag-name="' + filterName + '" class="enable-delete-icon" id="filter-delete-handler-' + filterId + '"  onclick="$(\'#lnr-catalog-search\').data(\'lnrcatalogsearch\').clearPriceSlider();"></label>';
					menustr += displayName;
					if (($('#price-slider-range').slider("option", "min") != $('#price-slider-range').slider("option", "values")[0])
						 || ($('#price-slider-range').slider("option", "max") != $('#price-slider-range').slider("option", "values")[1])) {
						//this will work when slider action happens
						showFilter = true;
						filterLength++;
					} else {
						//this will work when slider doesn't have proper values, if we don't initialize the slider properly (say catalog share scenario)
						var symbol = Drupal.settings.user_prefference.currency_sym;
						var minPrice = replaceAll(symbol, "", $('#price-slide-left').val());
						var maxPrice = replaceAll(symbol, "", $('#price-slide-right').val());
						minPrice = parseInt(minPrice.replace(/[^\d]/g, ''));
						maxPrice = parseInt(maxPrice.replace(/[^\d]/g, ''));
						if (($('#price-slider-range').slider("option", "min") != minPrice)
							 || ($('#price-slider-range').slider("option", "max") != maxPrice)) {
							//this will work when slider action happens
							showFilter = true;
							filterLength++;
						}
					}
				} else if(filterName == 'date-range') {
					var startdate = $("#ad_startdate1").val();
					var enddate = $("#ad_startdate2").val();	
					if(startdate == Drupal.t("LBL251")+ ':' +Drupal.t("LBL112")) {
						startdate = "";
					}
					if(enddate == Drupal.t("LBL113")) {
						enddate = "";
					}
					if(startdate != '' && enddate != '') {
						var displayName = startdate+'<span class=\'lower-case\'> '+Drupal.t('LBL621')+' </span>'+enddate;
						showFilter = true;
						filterLength++;
						
					} else if(startdate != '' || enddate != '') {
						var displayName = (startdate != '' ? startdate : enddate);
						showFilter = true;
						filterLength++;
					}
					var delAction = '<label data-filter-tag-name="'+filterName+'" class="enable-delete-icon" id="filter-delete-handler-'+filterId+'"  onclick="$(\'#lnr-catalog-search\').data(\'lnrcatalogsearch\').clearField(\'Date\');"></label>';
					menustr += displayName;
				} else if(filterName == 'cloud-tag') {
					var delAction = '<label data-filter-tag-name="'+filterName+'" class="enable-delete-icon" id="filter-delete-handler-'+filterId+'"  onclick="$(\'#lnr-catalog-search\').data(\'lnrcatalogsearch\').clearField(\'Tag\');"></label>';
					menustr += decodeURIComponent($('#srch_criteria_tag').val());
					if($('#srch_criteria_tag').length > 0 && $('#srch_criteria_tag').val() != '') {
						showFilter = true;
						filterLength++;
					}
				} else if(filterName == 'location') {
					var delAction = '<label data-filter-tag-name="'+filterName+'" class="enable-delete-icon" id="filter-delete-handler-'+filterId+'"  onclick="$(\'#lnr-catalog-search\').data(\'lnrcatalogsearch\').clearField(\'Location\');"></label>';
					menustr += decodeURIComponent($('#srch_criteria_location').val());
					if($.trim($('#srch_criteria_location').val()) != Drupal.t("LBL1321")) {
						showFilter = true;
						filterLength++;
					}
				} else if(filterName == 'top-search') {
					var delAction = '<label data-filter-tag-name="'+filterName+'" class="enable-delete-icon" id="filter-delete-handler-'+filterId+'"  onclick="$(\'#search_searchtext\').val(\'\');$(\'#lnr-catalog-search\').data(\'lnrcatalogsearch\').searchAction(\'top-search\');"></label>';
					menustr += $('#search_searchtext').val();
					if($('#search_searchtext').val().toLowerCase() != Drupal.t('LBL304').toLowerCase()) {
						showFilter = true;
						filterLength++;
					}
				}
				menustr += '</span></span>'+delAction;
				// console.log('filterName', filterName, 'displayName', displayName, 'showFilter', showFilter);
				if($('#paintContent .catalog-applied-filters').length == 0 && filterLength != 0) {
					$('<div class="catalog-applied-filters-wrapper"><div id="catalog-applied-filter-grid" class="catalog-applied-filters"></div></div>').insertBefore('#paintContent #no-records');
				}
				if (filterName != undefined && displayName != undefined && showFilter) {
					$('#paintContent #catalog-applied-filter-grid').append("<div class='checkedmenu' id='filter-"+filterId+"'>"+menustr+"</div>");
				}
			});
			$('.checkedmenu').each(function() {
				try {
					if($(this).find('.cls-filtermenu-wrapper').width() < $(this).find('.cls-filtermenu').width()) {
						$(this).find('.cls-filtermenu-wrapper').append('<span class="cls-filtermenu-fade"></span>');
					}
					var displayName = $(this).find('.cls-filtermenu').contents().get(0).nodeValue;
					if(displayName != null && displayName != '') {
						$(this).find('.cls-filtermenu-wrapper').attr('title', displayName).addClass('vtip');
					}
				} catch(e) {
					//window.console.log(e, e.stack);
				}
			});
			vtip('.checkedmenu');
			// add clear all handler
			if($('#catalog-applied-filter-grid').find('.cls-filter-clear').length == 0 && $('#paintContent #catalog-applied-filter-grid').find('.checkedmenu').length > 0) {
				var clearAllStr = Drupal.t('clear all');
				$("<a class=\"cls-filter-clear\" onclick=\"$('#lnr-catalog-search').data('lnrcatalogsearch').clearAllSearch();\">"+clearAllStr+"</a>").appendTo('#catalog-applied-filter-grid');
			} else {
				$('#catalog-applied-filter-grid .cls-filter-clear').appendTo('#catalog-applied-filter-grid');
			}

		} catch(e) {
			// window.console.log(e, e.stack);
		}
	},
	
	removeFilters: function(filters, filterCounter) {
		try {
			var filtersList = [];
			if($.isArray(filters)) {
				filtersList = filters;
			} else {
				filtersList.push(filters);
			}
			$(filtersList).each(function(index, filterId) {
				$('#filter-'+filterId).remove();
			});
			if($('#catalog-applied-filter-grid').find('.checkedmenu').length == 0) {
				$('#catalog-applied-filter-grid').find('.cls-filter-clear').remove();
				// $(".catalog-applied-filters-wrapper #catalog-applied-filter-grid").remove();
				$(".catalog-applied-filters-wrapper").remove();
			}
			
		} catch(e) {
			//window.console.log(e, e.stack);
		}
	},
	getGetJSONCookie: function(cookieName, value) {
		try {
			//if value is present, given value will be saved. else existing value of the cookie will be returned
			if(value !== undefined) {
				// $.cookie(cookieName, JSON.stringify(value));
				value = value.filter(function(item, i, ar){ return ar.indexOf(item) === i; });

				Drupal.settings['catalog_variables'][cookieName] = value;
				// console.log('cookie value set', Drupal.settings['catalog_variables'][cookieName]);
			} else {
				// return (($.cookie(cookieName) !== undefined && $.cookie(cookieName) != null) ? JSON.parse($.cookie(cookieName)) : []);
				var cookie = [];
				// console.log(typeof Drupal.settings['catalog_variables'][cookieName], Drupal.settings['catalog_variables'][cookieName]);
				if(typeof Drupal.settings['catalog_variables'][cookieName] != 'undefined' && Drupal.settings['catalog_variables'][cookieName] != null) {
					cookie = (Drupal.settings['catalog_variables'][cookieName]);
				}
				// console.log('cookie value get', cookie);
				return cookie;
			}
		} catch(e) {
			// window.console.log(e, e.stack);
		}
	},
	clearAllSearch: function(triggerSearch) {
		try {
		// console.log('clearall search');
			var obj = this;
			//clear all checkboxes, textboxes and call if clear methods available for controls and call searchAction
			$('.ot-others, .mro-others, .dt-others, .lang-others, .jobrole-others, .country-others, .rating-others').each(function() {
				$(this).attr('checked', false);
				$(this).parent().removeClass('checkbox-selected').addClass('checkbox-unselected');
			});
			obj.clearPriceSlider(false);
			obj.clearField("Date", false);
			obj.clearField("Location", false);
			obj.clearField("Tag", false);
			$('#search_searchtext').css({
				'color': '#999999',
				'font-size': '12px'
			})
			.val(Drupal.t('LBL304'));
			// console.log('triggerSearch = ', triggerSearch);
			if(triggerSearch === undefined || triggerSearch != false) {
				obj.searchAction('clearAll');
			}
		} catch (e) {
			// window.console.log(e, e.stack);
		}
	},
	refreshLastAccessedCatalogRow: function(gridRow) {
		try {
			var rowFound = false;
			// console.log('refreshLastAccessedCatalogRow', console.trace());
			var grid = $('#paintContentResults');
			if(gridRow === undefined) {
				var gridRow = grid.jqGrid('getGridParam', 'lastAccessedRow');
			}
			if(gridRow !== undefined && gridRow != null) {
				// console.log(gridRow, gridRow.id);
				var rowData = ((typeof gridRow.id != 'undefined' && gridRow.id) ? gridRow.id.split('-') : null);
				var options = {
					data: {
						ent_id: rowData[0],
						'ent_type': rowData[1]
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
	// var objStr = $("#lnr-catalog-search").data("lnrcatalogsearch");
});

$.extend($.ui.lnrcatalogsearch.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true,catStart:true}});


})(jQuery);

$(function() {
	try{
	$( "#lnr-catalog-search" ).lnrcatalogsearch();
	var curUrl	 = window.location.search.substring(1);
	var splitUrl = curUrl.split('/');
	var loadProfile = splitUrl[1];

	$("#search_searchtext").keyup(function(event){
		try{
		  if(event.keyCode == 13){
			  if(loadProfile != 'catalog-search') {
					var title = $('#search_searchtext').val();
					var objecttype = "Search";
					if(title != undefined) title=title.replace('/','@@');
					aUrl=resource.base_url;
					aUrl+='?q=learning/catalog-search/'+escape(objecttype)+"|"+escape(title);
					location.href=aUrl;
			  }else{
	 			   $("#lnr-catalog-search").data("lnrcatalogsearch").searchAction('top-search');
			  }
		   }
		}catch(e){
			// to do
		}
		})
		.attr('data-filter-id', 'top-search')
		.attr('data-filter-name', 'top-search')
		.attr('data-filter-label', Drupal.t('LBL304'));

	$('#searchTxtFrmPages').click(function() {
		try{
		if(loadProfile != 'catalog-search') {
			var title = $('#search_searchtext').val();
			var langtitle = Drupal.t("LBL304").toUpperCase();
			if((title.toLowerCase()) == (langtitle.toLowerCase()))
				title='';				
			var objecttype = "Search";
			if(title != undefined) title=title.replace('/','@@');
			aUrl=resource.base_url;
			aUrl+='?q=learning/catalog-search/'+escape(objecttype)+"|"+escape(title);
			location.href=aUrl;
	  }else{
		  $("#lnr-catalog-search").data("lnrcatalogsearch").clearSearchParam('title');
		  $("#lnr-catalog-search").data("lnrcatalogsearch").searchAction('top-search');
	  }
		}catch(e){
			// to do
		}
	});

	if($('#search_searchtext')) {
		try{
		$('#search_searchtext').autocomplete(
				"/?q=learning/catalog-autocomplete",{
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
	}
/*	$('.find-trng-sortby').each(function() {
		$('.find-trng-sortby-link').addClass('highlight-light-gray');
		$('.find-trng-sortby-link').removeClass('highlight-light-gray');
	});
	$("#search_searchtext").click(function(){
		$(this).val("");
	});*/
		datePickerOpen = false;
		$("#ad_startdate1, #ad_startdate2").live('focus', function(){
			 try {
				datePickerOpen = true;
			 } catch(e) {
				// console.log(e, e.stack);
			 }
		});
	}catch(e){
		// to do
		// console.log(e, e.stack);
	}
});
function preventBackspace(e) {
    var evt = e || window.event;
    if (evt) {
        var keyCode = evt.charCode || evt.keyCode;
        if (keyCode === 8 || keyCode === 13) { // prevent backspace and enter
            if (evt.preventDefault) {
                evt.preventDefault();
            } else {
                evt.returnValue = false;
            }
        }
    }
}
//42055: Provide an option in the admin share widget screen to pass site url
function passUrlCommonCall(click_id){
	//console.log($("#paintContentResults").jqGrid('getGridParam', 'rowNum'))
	var iframe_url = window.location.href;
	var new_url =  iframe_url.substring(0,iframe_url.indexOf('&'));
	var redirect_url = new_url.replace('widget/catalog-search','share/training')
	var pagenumber = 	$('.ui-pg-input').val();
	var rownumber = 	$('.ui-corner-bottom #data-table-page-view .page_count_view').html();
	redirect_url = redirect_url+'&page_number='+pagenumber+'&click_id='+click_id+'&row_number='+rownumber;
	window.open(redirect_url,'_blank');
//	iframe.contentWindow.location.reload();
//	var params = [
//	              'height='+screen.height,
//	              'width='+screen.width,
//	              'fullscreen=yes' // only works in IE, but here for completeness
//	              ].join(',');
//	window.open(redirect_url, 'new window', params);
}
function replaceAll(find, replace, str) {
	  return str.replace(new RegExp(find, 'g'), replace);
}

function filtersearch(val){
	try{
	//	alert("inside filtersearch"+val);
		var srchstr ='';
		var filter ='';
		var id='';
		if (val.id == 'language' || val=='language') {
			 srchstr = $("#search_language input.language").val();
			 filter = $('.search_filter_language');
			 id = $('.languagefilter').attr('id');
		} else if (val.id == 'country' || val=='country') {
			 srchstr = $("#search_country input.country").val();
			 filter = $('.search_filter_country');
			 id = $('.countryfilter').attr('id');
		}
		if (srchstr && srchstr != Drupal.t('LBL304')){
		//	alert(srchstr);
			srchstr = srchstr.replace(/\s+/g, '');
			filter.hide();
			var div = $('#' + id + ' div[id*=' + srchstr + ']');
		  			div.show();
		} else {
			filter.show();
	          }
	}catch(e){
		//console.log(e);
		// to do
	}}
function searchKeyPress(event, obj) {
	try{
		// alert("inside press"+obj.id);
		if(event.keyCode == 13 || srch == 1){
			if (obj.id == 'language_searchtext') {
				filtersearch('language');
			} else if (obj.id == 'country_searchtext') {
				filtersearch('country');
				}
		}
	}catch(e){
		// console.log(e);
		// to do
	}
}