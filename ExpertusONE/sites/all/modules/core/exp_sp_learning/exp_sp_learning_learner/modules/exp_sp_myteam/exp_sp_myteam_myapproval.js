(function($) {
$.widget("ui.lnrmyteamapproval", {
	_init: function() {
		try{
		//alert(232);
		var self = this;
		//var searchStr = this.searchActionCheck();
		var searchStr = '';
		this.searchStrValue = searchStr;
		
		this.renderSearchResults(searchStr);	
		this.blockHighlight();
		}catch(e){
			// to do
		}
	},
	blockHighlight : function() {
	try {
		if($('#block-system-main-menu a[href="/?q=learning/myteam-search"]').hasClass('active') == false){
        	$('#block-system-main-menu a[href="/?q=learning/myteam-search"]').addClass('active-trail active');
        	$('#block-system-main-menu a[href="/?q=learning/myteam-search"]').parent().addClass('active-trail');
    	}
    	}catch(e){
		 		//console.log(e);
				// to do
			}
	},
	
	renderSearchResults : function(searchStr){	
		 try{
			// alert("new");
		  this.currTheme = Drupal.settings.ajaxPageState.theme;
		 	if(this.currTheme == "expertusoneV2"){
				var teamGWidth = 726;
				var iconWidth = 70;
				var detailWidth = 475;
				var actionWidth = 100;
			}else{
				var lang = Drupal.settings.user.language;
				if(lang == 'it') {
					var teamGWidth = 764;
					var iconWidth = 68;
					var detailWidth = 519;
					var actionWidth = 200;
					
				}else {
					var teamGWidth = 764;
					var iconWidth = 68;
					var detailWidth = 589;
					var actionWidth = 130;
				}
				
		    }
			this.createLoader('lnr-myteam-search');
			var obj = this;
			var urlStr = (searchStr != '') ? ('&title='+encodeURIComponent(searchStr)) : '';
			var objStr = '$("#lnr-myteam-approval").data("lnrmyteamapproval")';
			$("#paintContentResults").jqGrid({
				url:obj.constructUrl("learning/myteam-myapproval/search/all/"+urlStr),
				datatype: "json",
				mtype: 'GET',
				colNames:['Icons','Details','Action'],
				colModel:[ {name:'Icons',index:'Icons', title:false, width:iconWidth,'widgetObj':objStr,formatter:obj.paintLPSearchIcons},
				           {name:'Details',index:'Details', title:false, width:detailWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResults},
				           {name:'Action',index:'Action', title:false, width:actionWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResultsAction}],
				rowNum:10,
				rowList:[10,20,30],
				pager: '#pager',
				viewrecords:  true,
				emptyrecords: "",
				toppager:true,
				height: 'auto',
				width: teamGWidth,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadui:false,
				loadComplete:obj.callbackLoader
			}).navGrid('#pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});
			$('.sort-by-links li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
			});
			$('.type1').addClass('sortype-high-lighter');
		 	}catch(e){
		 	//	console.log(e);
				// to do
			}
		},
	
		callbackLoader : function(response, postdata, formid){
			try{
		
			var recs = parseInt($("#paintContentResults").getGridParam("records"),10);
			 $("#search-link-content").html(response.links);
			    $('#paintContentResults').show();
	        if (recs == 0) {
	         /* if(($("#lnr-myteam-approval").data("lnrmyteamapproval").searchStrValue == '') ||
	        	             (($("#lnr-myteam-approval").data("lnrmyteamapproval").searchAction("AZ","type1"))==null) ||
	        	                  (($("#lnr-myteam-approval").data("lnrmyteamapproval").searchAction("ZA","type2"))==null) ||
	        	                         (($("#lnr-myteam-approval").data("lnrmyteamapproval").searchAction("Time","type3"))==null)) {
	                var html = Drupal.t("MSG413");
	               
	                $("#gview_paintContentResults"). css('display','none');
	        	}*/
	        	//else {
	        		$("#gview_paintContentResults"). css('display','none');
	                var html = Drupal.t("MSG403")+'.';
	        	//}
	        	$('#no-records').css('display','block').css('padding','10px').css('color','#777777').css('textAlign','center').css('lineHeight','40px');
	          $("#no-records").html(html);       
	        }
	        else {
	        	$("#gview_paintContentResults"). css('display','block');
	        	$('#no-records'). css('display','none');
	        	$("#no-records").html("");
	        }
	        
	        var obj = $("#lnr-myteam-approval").data("lnrmyteamapproval");

	        // Show pagination only when search results span multiple pages
	        var reccount = parseInt($("#paintContentResults").getGridParam("reccount"), 10);
	        var hideAllPageControls = true;
	        if (response.records > 10) { // 10 is the least view per page option.
	           hideAllPageControls = false;
	         
	          //console.log('callbackLoader() : hideAllPageControls set to false');
	        }
	       if (recs <= reccount) {
	          //console.log('callbackLoader() : recs <= reccount : hide pagination controls');
	          obj.hidePageControls(hideAllPageControls);	         
	        }
	        else {
	          //console.log('callbackLoader() : recs > reccount : show pagination controls');
	           obj.showPageControls();	          
	        }      
		      //$("#dummy_link").trigger("click");
	        $("#lnr-myteam-approval").data("lnrmyteamapproval").destroyLoader('lnr-myteam-approval');
	        $("#lnr-myteam-approval").data("lnrmyteamapproval").destroyLoader('lnr-myteam-approval');
	       $("#lnr-myteam-approval").data("lnrmyteamapproval").destroyLoader('searchRecordsPaint');
			   if($("#lnr-myteam-approval").data("lnrmyteamapproval").defaults.start) {
				    $("#search-filter-content").html(response.filter);
				     
				    $("#find-trng-sort-display").show();
				    $("#lnr-myteam-approval").data("lnrmyteamapproval").defaults.start = false;
			    } else {
				    $("#lnr-myteam-approval").data("lnrmyteamapproval").unselectFilter(response);
			    }
			  $("#search-link-content").html(response.links);
			  $("#search-filter-content").html(response.filter);
			 // $("#lnr-myteam-approval").data("lnrmyteamapproval").checkboxValidation();
			    //Vtip-Display toolt tip in mouse over
			    vtip();	
			    resetFadeOutByClass('#paintContentResults','content-detail-code','line-item-container','team');
			    $('.limit-title').trunk8(trunk8.myteamname_title);
		        $('#lnr-myteam-search #paintCountry').jScrollPane({}); 
			}catch(e){
			console.log(e);
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
		//alert(1212121);
		return rowObject['image'];
		}catch(e){
	     //	console.log(e);
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
	showMoreApprovalAction : function(obj,user_id) {
		try{
			var cur_obj_dis = 0;
			if($('#more-'+user_id).css('display')=='block'){
				cur_obj_dis = 1;
			}
			$('.team-more-action').hide();
			if(cur_obj_dis==0){
				$('#more-'+user_id).css('display','block')
			}else{
				$('#more-'+user_id).css('display','none')
			}
		}catch(e){
			// to do
		}
			},
			paintUsrNameAutocomplete : function(){
		try{
		$('#srch_criteria_location').autocomplete(
			"/?q=learning/myteam-myapproval/username-autocomplete",{
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
	highlightedText : function(ID,lang) {
	 try{	
		 if(lang == 'it' || lang == 'ru'){
			var font_size = '10px';			
		 }else{
			var font_size = '12px';
		 }
	  var defaultText = $('#' + ID).data('default-text');
		var obj = this;
		$("#"+ID).blur(function(){
   			if($("#"+ID).val() != defaultText) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(defaultText).css('color','#999999').css('fontSize',font_size);
        		if(obj.currTheme != "expertusoneV2"){
        			$(this).css('fontStyle','italic');
        		}
    		}
		});
		$("#"+ID).focus(function(){
   			if($("#"+ID).val() != defaultText) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == defaultText) {
        		$(this).val('').css('color','#333333').css('fontSize',font_size);
        		if(obj.currTheme != "expertusoneV2"){
        			$(this).css('fontStyle','italic');
        		}
    		}
		});
		$("#"+ID).change(function(){
   			if($("#"+ID).val() != defaultText) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(defaultText).css('color','#999999').css('fontSize',font_size);
        		if(obj.currTheme != "expertusoneV2"){
        			$(this).css('fontStyle','italic');
        		}
    		}
		});
	 }catch(e){
			// to do
	 }
	},	
	searchAction : function(sortbytxt,className) {
		try{
		
		var searchStr = '';		
		var cy_type = '';
		$('.country-others').each(function(){
			if($(this).is(':checked')){
				cy_type+=cy_type!=''?'|':'';
				cy_type+=$(this).val();
			}
		});

		/*-------Title-------*/
		var title 	  = $('#search_searchtext').val();
			if(title == Drupal.t("LBL304").toUpperCase())
				title='';
		
		var startdate   = $("#approval_startdate1").val();
		var enddate   = $("#approval_startdate2").val();	
		var regEx = /^\d{2}-\d{2}-\d{4}$/;
		
		if(startdate == Drupal.t("LBL112") ||  startdate.match(regEx) == null)
				startdate = "";
			else			
				$('#date-clr').css('display','block');
	
		if(enddate == Drupal.t("LBL113") || enddate == Drupal.t("LBL112")|| enddate.match(regEx) == null)
				enddate = "";
			else		
				$('#date-clr').css('display','block');
		
		/*-------Location-------*/
		var location 	= $('#srch_criteria_location').val();
		
			if(location == Drupal.t('Type Name')){
				location='';
			}else{
			
				$('#location-clr').css('display','block');
				}

		/*-------Username-------*/
		
		
		
		
		
		var username 	= $('#srch_criteria_username').val();
		
			if(username == Drupal.t('LBL181'))
				username='';
			else
				$('#username-clr').css('display','block');

		/*-------Sort By-------*/
		var sortby = sortbytxt;
		//var atLeastOneIsChecked = $('.report-others:checkbox:checked').length > 0;
		//console.log('atLeastOneIsChecked==='+atLeastOneIsChecked);
		var reporttype = '';
		$('.report-others').each(function(){
			if($(this).is(':checked')){
			 
				reporttype+=reporttype!=''?'|':'';
				
				
				reporttype+=$(this).val();	
			}

		});
		/*------Header Search------*/
		var headersearch 	= $('#myteam_searchtext').val();
		if(headersearch.toLowerCase() == (Drupal.t('LBL304').toLowerCase()))
			headersearch='';
		
		// Note: exp_sp_myteam.inc:setSearchParam() is expecting @full_name which is not passed below instead of @title.
		// However, currently the top search filter shown in my team page is for catalog search.
		// Remove the username filter in the narrow search filter - ticket no : 28622
		//searchStr	= '&title='+encodeURIComponent(title)+'&reporttype='+reporttype+'&location='+location+'&username='+username+'&cy_type='+cy_type+'&sortby='+sortby+'&searchusername='+headersearch;
		searchStr	= '&title='+encodeURIComponent(title)+'&reporttype='+reporttype+'&startdate='+startdate+'&enddate='+enddate+'&location='+location+'&cy_type='+cy_type+'&sortby='+sortby+'&searchusername='+headersearch;
		this.searchStrValue = searchStr;
		
		// Close all open accordions
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
		$('#paintContentResults tr.cus-accord:not(.noborder)').prev('tr').children("td").css("border-bottom","1px solid #EDEDED");
	 	}
	 	else
	 	{
		$('#paintContentResults tr.cus-accord:not(.noborder)').prev('tr').children("td").css("border-bottom","dotted 1px #ccc");
	 	}
		$('#paintContentResults tr.cus-accord').remove();
		
    this.createLoader('searchRecordsPaint');

		$('#paintContentResults').setGridParam({url: this.constructUrl('learning/myteam-myapproval/search/all/'+searchStr)});
	    $("#paintContentResults").trigger("reloadGrid",[{page:1}]);
	   
	   
	    this.checkboxValidation();
	   
	    //Highlight sort type 
		$('.sort-by-links li').each(function() {
			$(this).find('a').removeClass('sortype-high-lighter');
		});
		if(!className){
			$('.type1').addClass('sortype-high-lighter');
		}else{
			$('.'+className).addClass('sortype-high-lighter');
		}
		
		 
	
		
		//alert(45656565);
		}catch(e){
		//console.log(e);
			// to do
		}
	},
	
	paintAfterReady : function() {		
		try{
		var dates = $('#approval_startdate1').datepicker({
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
			//  beforeShow: $("#lnr-myteam-approval").data("lnrmyteamapproval").customRangeDate
		});
		var dates = $('#approval_startdate2').datepicker({
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
			  beforeShow:$("#lnr-myteam-approval").data("lnrmyteamapproval").customRangeDate
		});
		
		}catch(e){
			// to do
		}
	},
	
	customRangeDate : function (input) 
	{ 
		try{
	        var dateMin = null;
	        var dateMax = null;

	        if (input.id == "approval_startdate1" && $("#approval_startdate2").datepicker("getDate") != null)
	        {
	        		dateMin = new Date();
	                dateMax = $("#approval_startdate2").datepicker("getDate");
	                dateMax.setDate(dateMin.getDate() + 20000);
	                                      
	        }
	        else if (input.id == "approval_startdate2")
	        {
	                dateMax = new Date();
	                if ($("#approval_startdate1").datepicker("getDate") != null)
	                {
	                        dateMin = $("#approval_startdate1").datepicker("getDate");
	                        dateMax = $("#approval_startdate1").datepicker("getDate");
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
	
	clearField : function (txt) {
		try{
		if(txt=='Location'){
			$('#srch_criteria_location').val(Drupal.t("Type Name")).css('color','#999999').css('fontSize','11px');
			if(this.currTheme != "expertusoneV2"){
				$('#srch_criteria_location').css('fontStyle','italic');
			}
			$('#location-clr').css('display','none');
		}else if(txt=='Username'){
			$('#srch_criteria_username').val(Drupal.t("LBL181")).css('color','#999999').css('fontSize','11px');
			if(this.currTheme != "expertusoneV2"){
				$('#srch_criteria_username').css('fontStyle','italic');
			}
			$('#username-clr').css('display','none');
		}else if(txt=='Date'){
			//this.clearSearchParam('startdate');
			//this.clearSearchParam('enddate');
			$('#approval_startdate1').val(Drupal.t("LBL251")+ ':' +Drupal.t("LBL112")).css('color','#999999').css('fontSize','11px');
			$('#approval_startdate2').val(Drupal.t("LBL113")).css('color','#999999').css('fontSize','11px');
			$('#date-clr').css('display','none');
		}
		this.searchAction();
		}catch(e){
			// to do
		}
	},
	
	locationEnterKey : function(){
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
	
	VerifyRejectCert : function(CertId,status){
		try{
		var pageNo = $(".ui-pg-input").val();		
		
		var url = '/?q=learning/profile/certificate/verify/'+CertId+'/'+status+'';
	
		var obj = this;
		$.ajax({
			type: 'POST',
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function() {
				
			//var searchStr='';
			//$('#paintContentResults').setGridParam({url: this.constructUrl('learning/myteam-myapproval/search/all/'+searchStr)});
	    	$("#paintContentResults").trigger("reloadGrid",[{page:pageNo}]);
			}
		
	            
	  });
		}catch(e){
			// to do
			//console.log(e);
		}
	},
	
	
	
	checkboxValidation : function() {
		try{
		
		
		
		$('#searchopts-content').find('input[type=checkbox]').each(function() {
		//alert(4);
	
			if($(this).is(':checked')){	
		
			  		
				if($(this).val() != 'All'){
				
					$(this).parent().next('label').removeClass('highlight-light-blue');
					$(this).parent().next('label').addClass('highlight-light-gray');
					$(this).parent().removeClass('checkbox-unselected');				
					$(this).parent().addClass('checkbox-selected');
				}
				
			} else {
			
			//alert(2);			
				if($(this).val() != 'All'){
					$(this).parent().next('label').removeClass('highlight-light-gray');
					$(this).parent().next('label').addClass('highlight-light-blue');
					$(this).parent().removeClass('checkbox-selected');				
					$(this).parent().addClass('checkbox-unselected');
				}
				
			}
		});	 
		//alert(5);
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
		$('#srch_criteria_catkeyword').flushCache();
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
				
	   			if($("#"+ID).val() != textType) {
        		$(this).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(textType).css('color','#999999').css('fontSize','10px').css('fontStyle',fontStyle);
    		}
		});
		$("#"+ID).focus(function(){
				
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
		var v1=$('#approval_startdate1').val();
		var v2=$('#approval_startdate2').val();
				
		if($('#approval_startdate1').val() == 'Start: mm-dd-yyyy')
			v1 = '';

		if($('#approval_startdate2').val() == 'End: mm-dd-yyyy')
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

		this.searchAction('date');
		
	 }catch(e){
			// to do
	 }
	},
	
	hideMoreApprovalAction : function(obj) {
		try{
			this.prevMoreLPObj = '';
		}catch(e){
			// to do
		}
	},
	
	ShowCertificateDet : function(certName,Company,certNumber,validity,fileName) {	
		try{
		
		var obj = this;

			var rhtml = '';
			rhtml += '<div  class = "cert_name"><label>'+Drupal.t("LBL205")+':</label>';
			rhtml += '<span  class = "cls-crt-lbl-val vtip " title = "'+certName+'"> '+titleRestrictionFadeoutImage(certName, 'my-cert-name-fadeout-container')+'</span></div>';
			rhtml += '<div  class = "cert_name"><label>'+Drupal.t("Company")+':</label>';
			rhtml += '<span  class = "cls-crt-lbl-val vtip" title = "'+Company+'">'+titleRestrictionFadeoutImage(Company,'my-company-name-fadeout-container')+'</span></span></div>';
			rhtml += '<div  class = "cert_name"><label>'+Drupal.t('LBL205')+' '+Drupal.t("LBL161")+':</label>';
			rhtml += '<span  class = "cls-crt-lbl-val vtip" title = "'+certNumber+'">'+titleRestrictionFadeoutImage(certNumber,'my-cert-number-fadeout-container')+'</span></div>';
			rhtml += '<div  class = "cert_name"><label>'+Drupal.t("LBL604")+':</label>';
			rhtml += '<span  class = "cls-crt-lbl-val vtip" title = "'+validity+'">'+titleRestrictionFadeoutImage(validity,'my-validity-date-fadeout-container')+'</span></div>';
			rhtml += '<div  class = "cert_name"><label>'+Drupal.t('LBL610')+' '+Drupal.t("LBL205").toLowerCase()+':</label>';
			rhtml += '<span  class = "cls-crt-lbl-val vtip" title = "'+fileName+'">'+titleRestrictionFadeoutImage(fileName,'my-file-name-fadeout-container')+'</span></div>';
			//rhtml += '</form>';
			
			//var noteTxt ='<div class="refer-note"><span title="This field is required." class="require-text vtip" style="display:inline-block;margin-right:5px;margin-left:7px;">*</span><span class="refer-note-text">'+SMARTPORTAL.t(result[0]['labelmsg']['msg8'])+'</span></div>';
			var dlgDiv = '<div id="certificate_det_container" class="view_cert-container"></div>';
			$('#certificate_det_container').remove();
			$("#cert_dialouge_holder").remove();
			$('body').append(dlgDiv);
			$('#certificate_det_container').html(rhtml);

           var closeButt={};
			closeButt[SMARTPORTAL.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#certificate_det_container').remove();};
			
			$("#certificate_det_container").dialog({
				bgiframe: true,
				width:420,
				resizable:false,
				draggable:false,
				closeOnEscape: false,
				modal: true, 
				title: Drupal.t('Certificate'),
				buttons: closeButt,
				close: function(){
					$("#certificate_det_container").remove();
					$("#cert_dialouge_holder").remove();
				},
				overlay:
				{
				   opacity: 0.4,
				   background: '#000000'
				 }	
			});
			
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').attr("tabindex",6);
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').attr("tabindex",7);
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			if(this.currTheme == "expertusoneV2"){
			changeDialogPopUI();
			}
			//$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
			$('.ui-dialog').wrap("<div id='cert_dialouge_holder' class='refer-course-holder'></div>");
			$('#cert_dialouge_holder a.ui-dialog-titlebar-close').html('X');
			//$('.ui-dialog-buttonset').before(noteTxt);
			$('.refer-class-container').css('width','93.2%');
			$('.refer-class-container').css('height','auto');
			vtip('.view_cert-container');
		
		//return rowObject['details'];
		}catch(e){
			// to do
			//console.log(e);
		}
	},
	
	viewCertificate : function(certName, imgPath, fileFormat) {	
		try{
		
		var obj = this;

			var rhtml = '';
			
			rhtml += '<div  class = "cert_name">';
			if(fileFormat == "pdf" || fileFormat == "PDF"){
			rhtml += '<iframe src="'+imgPath+'" class="vtip" /></div>';
			}
			else if(fileFormat == "png" || fileFormat == "PNG"){
			rhtml += '<img src="'+imgPath+'" class="vtip" /></div>';
			}
			//rhtml += '</form>';
			
			//var noteTxt ='<div class="refer-note"><span title="This field is required." class="require-text vtip" style="display:inline-block;margin-right:5px;margin-left:7px;">*</span><span class="refer-note-text">'+SMARTPORTAL.t(result[0]['labelmsg']['msg8'])+'</span></div>';
			var dlgDiv = '<div id="certificate_det_container" class="view_cert-container pdfWrap" style="padding:0"></div>';
			$('#certificate_det_container').remove();
			$("#cert_dialouge_holder").remove();
			$('body').append(dlgDiv);
			
			$('#certificate_det_container').html(rhtml);

           var closeButt={};
			closeButt[SMARTPORTAL.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#certificate_det_container').remove();};
			
			if(certName.length >70){
			var titleDisplay = '<div class="line-item-container float-left cusSpanStyle"><span title="'+Drupal.t(certName)+'" class="vtip">'+titleRestrictionFadeoutImage( Drupal.t(certName),'certificateTitle')+'</span></div>';
			}
			else{
			var titleDisplay = '<div class="line-item-container float-left cusSpanStyle"><span title="'+Drupal.t(certName)+'" class="vtip">'+Drupal.t(certName)+'</span></div>';
			}
			$("#certificate_det_container").dialog({
				bgiframe: true,
				width:688,
				resizable:false,
				draggable:false,
				closeOnEscape: false,
				modal: true, 
				title: titleDisplay,
				buttons: closeButt,
				close: function(){
					$("#certificate_det_container").remove();
					$("#cert_dialouge_holder").remove();
				},
				overlay:
				{
				   opacity: 0.4,
				   background: '#000000'
				 }	
			});
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').attr("tabindex",6);
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').attr("tabindex",7);
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			if(this.currTheme == "expertusoneV2"){
			changeDialogPopUI();
			}
			//$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
			$('.ui-dialog').wrap("<div id='cert_dialouge_holder' class='refer-course-holder'></div>");
			$('#cert_dialouge_holder a.ui-dialog-titlebar-close').html('X');
			//$('.ui-dialog-buttonset').before(noteTxt);
			$('.refer-class-container').css('width','93.2%');
			$('.refer-class-container').css('height','auto');
		
		
		//return rowObject['details'];
		}catch(e){
			// to do
			//console.log(e);
		}
	},
	
	 hidePageControls : function(hideAll) {
	try{
    var lastDataRow = $('#paintContentResults tr.ui-widget-content').filter(":last");
    
    
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
	 try{  
    //console.log('showPageControls() : show all control');
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
 
		/*
	 * To render the user profile information for the given user id
	 */
});
$.extend($.ui.lnrmyteamapproval.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true,catStart:true}});

})(jQuery);

$(function() {
	try{
	//alert(23);
	$( "#lnr-myteam-approval" ).lnrmyteamapproval();
/*	$('#myteam-cat-delivery-type').event(function(){
		alert("Event");
	});
*/
	}catch(e){
	// to do
	}
});

function highlightLink(){
$('#myteamlink').css('text-decoration', '');
        		$('#myapprovallink').css('text-decoration', 'underline');
}


$('#myteam-search-txt').click(function() {
		try{
	if ( $( ".page-myapproval-myteam-certificate" ).length > 0) {
	//alert(22);
	
	$("#lnr-myteam-approval").data("lnrmyteamapproval").searchAction();
	}
		}catch(e){
			// to do
		}
	});