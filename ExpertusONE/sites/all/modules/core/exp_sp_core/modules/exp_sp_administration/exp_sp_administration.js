//var reloadFlagForPagination=true;
(function($) {
	try{
	$.widget("ui.narrowsearch",{
						// Static variables
						widgetName : 'narrowsearch',
						widgetInitId : 'root_admin',
						widgetSelector : '#root_admin',
						searchBasePath : '',
						firstTime : true,
						filterSets : new Array(), // Each element of this  array is an object, e.g. {type: "checkbox", code: "orgstatus"}
						lastSortType : '',
						lastSortTypeHtmlId : '',
						exportPath : '',
						printPath : '',
						showTopTextFilter: true,
						topTextFilterLabel:'',
						tabContentType:'narrow_search',
						qtipLoadSet : new Array(),
						qtipLenth : 0,

						_init:function(){
							//this.initAdminLinks();
							this.currTheme = Drupal.settings.ajaxPageState.theme;
							var pageUrl = window.location.search;
							if (pageUrl.indexOf("admincalendar") < 0) // search call restricted for admin cal
								this.getSearchContentDisp();

						},

						getUrlVars : function (name) {
							var vars = {};
							var parts = window.location.href.replace(/[?&]+([^=&]+)=([^&]*)/gi, function(m,key,value) {
								vars[key] = value;
							});
							return vars[name];
						},

						getModuleURLs : function(){
							var qPath = this.getUrlVars('q');
							if(qPath == 'administration'){
								//qPath = $('#root-admin-links-holder li:first-child a').attr('href');
								//window.location.href = qPath;
								return false;
							}
							var qPathArray = qPath.split('/');
							var urlArray = new Array();
							urlArray['main_module'] = qPathArray[1];
							urlArray['sub_module'] = qPathArray[2];
							return urlArray;
						},

						getSearchContentDisp : function() {
							var obj = this;
							var curObj = this;

							var moduleURLs = obj.getModuleURLs();
							if (moduleURLs === false) {
							  return false;
							}

							/* Highlight root admin menu */
							if($('#block-system-main-menu a[href="/?q=administration"]').hasClass('active') == false){
								$('#block-system-main-menu a[href="/?q=administration"]').addClass('active-trail active');
								$('#block-system-main-menu a[href="/?q=administration"]').parent().addClass('active-trail');
							}
							
						  /* Highlight selected menu links */
						  $("ul.list-item-administrator li a").removeClass("root-admin-links-selected");
						  $("#"+moduleURLs['main_module']).addClass("root-admin-links-selected");

						  /* THE BELOW LINES WRITTEN FOR NEW THEME BY VJ*/
						  $("ul.list-item-administrator li.admin-left-panel-module-list").removeClass("root-admin-module-selected");
						  $("#"+moduleURLs['main_module']).parent().addClass("root-admin-module-selected");
							curObj.createLoader("root-admin-search-right-col");
 							url = obj.constructUrl("administration/search-filter/"+moduleURLs['sub_module']);
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(data){

								   curObj.destroyLoader("root-admin-search-right-col");

								   var detailsObj = jQuery.parseJSON(data);

								   // Extend Drupal.settings, if any settings are present in the data
								   if (detailsObj!=null && detailsObj.drupal_settings) {
								     $.extend(true, Drupal.settings, detailsObj.drupal_settings);
								   }

								   /* Display the various admin tabs, etc. including the tab-content-main div */
                   $("#root-admin-results").html(detailsObj.rendered_main_div);
                   $("#root-admin-results").show();

                   /* Display the content in the main content tab.
                    * For narrow_search, this will render place holder HTML elements for search results jqGrid, sortbar, action bar, etc */
                   $("#tab-content-main").html(detailsObj.rendered_tab_content_main);
                   
                 //Added by vetrivel.P for #0070900
                   if(moduleURLs['sub_module'] == 'user'){
                   var sec = readCookie('user_upload_message');
                   if(sec !== null){
                   		sec = urldecode(sec);
                   		var error = new Array();
                   		error[0] = sec;
                   		$('#show_suspend_message').remove();
                   		 var message = "<div id='show_suspend_message' style='position:absolute;left:0;right:0;'></div></div>";
                   		 var msg='<div id="message-container" style="visibility:visible"><div class="messages error"><ul><li><span>'+error[0]+'</span></li></ul><div class="msg-close-btn" onclick="$(\'#show_suspend_message\').remove();eraseCookie(\'message\');"></div></div></div><img onload="$(\'body\').data(\'learningcore\').showHideMultipleLi();" style="display:none;" src="sites/all/themes/core/expertusone/expertusone-internals/images/close.png" height="0" width="0"/>';
                   		 $('#tab-content-main').prepend(message);
                   		 $("#tab-content-main #show_suspend_message").append(msg);
                   	eraseCookie('user_upload_message');
                   	}
                   }else if(moduleURLs['sub_module'] == 'catalog'){
                       var sec = readCookie('bulk_enrollment_upload_message');
                   if(sec !== null){
                   		sec = urldecode(sec);
                   		var error = new Array();
                   		error[0] = sec;
                   		$('#show_suspend_message').remove();
                   		 var message = "<div id='show_suspend_message' style='position:absolute;left:0;right:0;'></div></div>";
                   		 var msg='<div id="message-container" style="visibility:visible"><div class="messages error"><ul><li><span>'+error[0]+'</span></li></ul><div class="msg-close-btn" onclick="$(\'#show_suspend_message\').remove();eraseCookie(\'message\');"></div></div></div><img onload="$(\'body\').data(\'learningcore\').showHideMultipleLi();" style="display:none;" src="sites/all/themes/core/expertusone/expertusone-internals/images/close.png" height="0" width="0"/>';
                   		 $('#tab-content-main').prepend(message);
                   		 $("#tab-content-main #show_suspend_message").append(msg);
                   	eraseCookie('bulk_enrollment_upload_message');
                   	}
                   }
                  
								   if (detailsObj.rendered_tab_content_type == "narrow_search") { /* If narrow search */
									   /* Display left side filters*/
										 $("#narrow-search").html(detailsObj.rendered_narrow_search_filters);
										 $("#narrow-search").show();
										 if ($('#group_list_count').val() > 4) {
											$('#group_filterset').css({'height': '90px', 'overflow': 'hidden', 'width': '160px'});
											$("#group_filterset").jScrollPane();
										 }

						                /* if ($('#rescountry_filterset #adminFilterlist_country').val() > 5) {
							  				$('#rescountry_filterset').css({'height': '150px', 'overflow': 'hidden', 'width': '160px'});
							  		   		$('#rescountry_filterset').jScrollPane();
						  			     }	*/					              
										 // Ticket No:0080037 (Revision No:0275605, Point No: 2.3) #custom_attribute_0078975
										 var tabHeight= $("#sort-bar-V2").css('height');
										 if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
											 tabHeight = parseInt(tabHeight)-2;
										 
										 /* Display top bar */
										 if (Drupal.settings.ajaxPageState.theme == 'expertusoneV2') {
											 //$("#admin-maincontent_tab").html("<img style='width:24px;height:24px;padding-top:5px;cursor:pointer;' onclick='drawCalendar();' src='/sites/all/themes/core/expertusoneV2/images/calendaricon.png'></img>");
											 $("#admin-maincontent_tab").append(detailsObj.rendered_narrow_search_sortbar);
											 $("#narrow-search-actionbar").remove();
											 // Ticket No:0080037 (Revision No:0275605, Point No: 2.3) #custom_attribute_0078975
											 if(moduleURLs['main_module']=='manage' && (moduleURLs['sub_module']=='announcement' || moduleURLs['sub_module']=='customattribute') && $('#carousel_inner').parent('#carousel_container')) {
												 $("#sort-bar-V2").css('height','0').css('overflow','hidden');
											 }
											 $("#sort-bar-V2").append(detailsObj.rendered_narrow_search_actionbar);
										 }
										 else {
											 $("#narrow-search-results-topbar").html(detailsObj.rendered_narrow_search_sortbar);
											 $("#narrow-search-actionbar").remove();
											 $("#admin-maincontent_tab").append(detailsObj.rendered_narrow_search_actionbar);
										 }
										 $("#narrow-search-results-topbar").show();

									/*	 if (Drupal.settings.ajaxPageState.theme == "expertusoneV2") {
										   var adminSubModelLength = $("ul.AdminsublinktabNavigation li").length;
											 if(adminSubModelLength >= 4) {
											 	 $("ul.AdminsublinktabNavigation li a span span").css('padding','0px 5px 0px 5px');
											 }
										 }*/
										 /* Add class "selected" to this selected tab and raise it up to the front */
										 $( "ul.AdminsublinktabNavigation li").removeClass("selected");
										 $("#admin-tab-"+moduleURLs['sub_module']).parent("li").addClass("selected");
										 resetMainTab(); // Display selected tab if it is in hidden
										 //#custom_attribute_0078975
										//Custom Attribute Changes 
										if(moduleURLs['main_module']=='manage' && $('#carousel_inner').parent('#carousel_container')) {
											var itemcnt = $('#carousel_container #carousel_inner ul li').size();
												// console.log('itemcnt: '+itemcnt);
											if(itemcnt === 6) {
												var sort_width = parseInt($('#sort-bar-V2').width());
													// console.log('load container width: '+sort_width);
												var actionbar_width = parseInt($('#narrow-search-actionbar').width());
													// console.log('load actionbar width: '+actionbar_width);
												var container_width = (sort_width - actionbar_width) - 45;
												var width_calc = '';
												var ulactual_width = $('#carousel_container #carousel_inner ul').width();
													// console.log('ul actual width: '+ulactual_width);
												$('#carousel_container #carousel_inner').css('width', 'auto');
												$('#carousel_container #carousel_inner ul').css('width', 'auto');
												var ul_width = parseInt($('#carousel_container #carousel_inner ul').width());
													// console.log('ul width: '+ul_width);
												var lastli_width = parseInt($('#carousel_container #carousel_inner ul li:last').width());
													// console.log('last li width: '+lastli_width);
												if(container_width > ul_width) {	
													width_calc = container_width;
													$('#carousel_container .last').css('display', 'none');
												} else 
													width_calc = (ul_width - lastli_width) - 4;
												// console.log('carousel_inner width: '+width_calc);
												$('#carousel_container #carousel_inner').css('width', width_calc);
												$('#carousel_container #carousel_inner ul').css('width', ulactual_width);
												
												if(moduleURLs['sub_module']=='customattribute')
													MoveTabNext(itemcnt);
											} else {
												if(moduleURLs['sub_module']=='announcement' || moduleURLs['sub_module']=='customattribute')
													MoveTabNext(itemcnt);
											}
										}
										// Ticket No:0080037 (Revision No:0275605, Point No: 2.3)
										if(moduleURLs['main_module']=='manage' && (moduleURLs['sub_module']=='announcement' || moduleURLs['sub_module']=='customattribute') && $('#carousel_inner').parent('#carousel_container')) {
											setTimeout(function(){
												$("#sort-bar-V2").css('height',tabHeight).css('overflow','visible');
											},500);
										}

										 var linkPath = 'administration/'+moduleURLs['main_module'];
										 var sublinkPath = 'administration/'+moduleURLs['main_module']+'/'+moduleURLs['sub_module'];
										 curObj.initPaths(detailsObj.rendered_tab_content_type);
										 curObj.initFilters();
										 curObj.renderSearchResults(linkPath, sublinkPath);

										 if($('#narrow-search-actionbar')){
								  	   $('#narrow-search-actionbar').hide();
										 }
										 if(typeof detailsObj.rendered_script != 'undefined'){
											 eval(detailsObj.rendered_script+"()");
										 }
										 $('.courselangtype-scroll-list-container, .coursecurrency-scroll-list-container, .contentlang-scroll-list-container, .tpcurrency-scroll-list-container, .surveydetailslang-scroll-list-container, .surveyquestionslang-scroll-list-container, .langtype-scroll-list-container, .grplang-scroll-list-container, .rescountry-scroll-list-container, .announcementlang-scroll-list-container, .ordercurrency-scroll-list-container').jScrollPane({});
				   }
								   else if (detailsObj.rendered_tab_content_type == "ajax-form") {
                     Drupal.attachBehaviors();
                   }
								}
							});
						},

						initPaths : function(tabContentType){   /* Initialize path for search, print, export, filter option */
							if(tabContentType!=""){
							  this.tabContentType=tabContentType;
							}
							if(this.tabContentType=="narrow_search"){
								/* Narrow search filter data lists */
								var filterSetData=$("#narrow-search-filters").attr("data");
								this.filterSets=eval(filterSetData); //commented for issue in Drupal translated strings for french language
								//this.filterSets=eval(filterSetData.replace(/\\u0026#39;/g,"'"));


								/*Sort bar data lists */
								var sortBarData=$("#narrow-search-sortbar").attr("data");
								var sortBarObj = jQuery.parseJSON(eval("'"+ sortBarData +"'"));
								this.lastSortType = sortBarObj.last_sort_type;
								this.lastSortTypeHtmlId = sortBarObj.last_sort_type_html_id;

								/* Action bar data lists */
								var actionBarData=$("#narrow-search-actionbar").attr("data");
								var actionBarObj = jQuery.parseJSON(eval("'"+ actionBarData +"'"));
								this.exportPath = actionBarObj.export_path;
								this.printPath = actionBarObj.print_path;

								/* Other - narrow search data lists */
								var narrowSearchOtherData=$("#narrow-search-results").attr("data");
								var narrowSearchOtherObj = jQuery.parseJSON(eval("'"+ narrowSearchOtherData +"'"));

								this.searchBasePath = narrowSearchOtherObj.search_base_path;
								this.text_filter_ac_path = narrowSearchOtherObj.text_filter_ac_path;
								this.showTopTextFilter = narrowSearchOtherObj.show_top_text_filter;
								this.topTextFilterLabel = narrowSearchOtherObj.top_text_filter_label;

								if(this.topTextFilterLabel==null || this.topTextFilterLabel=="undefined" || this.topTextFilterLabel==undefined){
									this.topTextFilterLabel = (Drupal.settings.ajaxPageState.theme == 'expertusoneV2') ? Drupal.t("LBL304") : Drupal.t("LBL304").toUpperCase();
								}

								if(this.showTopTextFilter){

								  this.initTextFilter(this.text_filter_ac_path);

								  $("#narrow-search-text-filter").val(this.topTextFilterLabel);

								  this.initTextDateFilterBlurStyle("#narrow-search-text-filter", this.topTextFilterLabel,"1");

								  $("#narrow-search-text-filter-container").show();

								}else{

								  $("#narrow-search-text-filter-container").hide();

								}
						   }else{ /* Non - NarrowSearch */

						   }

						},

						displayResultWizard : function(title,message){
						    $('#result-msg-wizard').remove();
						    var html = '';
						    html+='<div id="result-msg-wizard" style="display:none; padding: 0px;">';
						    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';

						    html+= '<tr><td height="40px">&nbsp;</td></tr>';
							html+= '<tr><td align="center" class="light-gray-color">'+message+'</td></tr>';

						    html+='</table>';
						    html+='</div>';
						    $("body").append(html);
						    var closeButt={};
						    closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#result-msg-wizard').remove();};

						    $("#result-msg-wizard").dialog({
						        position:[(getWindowWidth()-400)/2,200],
						        bgiframe: true,
						        width:400,
						        resizable:false,
						        modal: true,
						        title:title,//"Delete",
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
						    $('.ui-dialog-buttonpane .ui-difalog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

						    $("#result-msg-wizard").show();

							$('.ui-dialog-titlebar-close').click(function(){
						        $("#result-msg-wizard").remove();
						    });
							this.currTheme = Drupal.settings.ajaxPageState.theme;
						 	if(this.currTheme == "expertusoneV2"){
						 	 changeDialogPopUI();
							}
						},

						resetUserPassword : function(userEmail, userId, rowObj){
							var obj = this;
							$("#reset-password-loader-"+userId).remove();
							var cRow = $(rowObj).parents("tr").before("<tr><td colspan='3'><div id='reset-password-loader-"+userId+"'></div></td></tr>");
							this.createLoader("reset-password-loader-"+userId);
							url = obj.constructUrl("ajax/administration/people/reset-password" + '/' + userEmail );
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
									result = $.trim(result);
									result = result.split('|');
									$("#root-admin").data("narrowsearch").displayResultWizard(result[0],result[1]);
									$("#root-admin").data("narrowsearch").destroyLoader('reset-password-loader-'+userId);
								}
							});
						},

						publishAndUnpublishCatalog : function(catalogId, catalogType,actionStatus, rowObj, iscompliance) {
							var comp = iscompliance;
							var obj = this;
							url = obj.constructUrl("ajax/administration/learning/catalog/show-catalog" + '/' + catalogId);

							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(access){
									this.currTheme = Drupal.settings.ajaxPageState.theme;

									var show =	$.trim($('#publish-unpublish-'+catalogId).html());

									if(iscompliance == 1 && access == 0 && show == 'Show in Catalog'){
										var uniqueClassPopup = '';
										 $('#delete-msg-wizard').remove();
										    var html = '';
										    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
										    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
										    if(this.currTheme == 'expertusoneV2'){
										   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+Drupal.t("MSG711")+'</td></tr>';
										    } else {
										     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+Drupal.t("MSG711")+'</td></tr>';
										    }
										    html+='</table>';
										    html+='</div>';
										    $("body").append(html);

										    var closeButt={};
										    //closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};
										    closeButt[Drupal.t('LBL109')]=function(){ $("#root-admin").data("narrowsearch").publishAndUnpublishCat(catalogId, catalogType,actionStatus, rowObj);
										    };
										    if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
										    	var esignObj = {'popupDiv':'catalog-publish-unpublis-dialog',
										    			'esignFor':'PublishUnpublishCatalog','catalogId':catalogId,'catalogType': catalogType,'actionStatus': actionStatus, 'rowObj': rowObj};
										    	closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj); };

										 }else{
											 closeButt[Drupal.t('Yes')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();
											 setTimeout(function() {
													$('#access-visibility-'+catalogId).click();
												}, 10);
											 };

   										 }
										    $("#delete-msg-wizard").dialog({
										        position:[(getWindowWidth()-400)/2,200],
										        bgiframe: true,
										        width:300,
										        resizable:false,
										        modal: true,
										        title:Drupal.t('LBL749'),
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
										    $('.ui-dialog').wrap("<div id='catalog-publish-unpublis-dialog' class='"+uniqueClassPopup+"'></div>");
										    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
										    $(".removebutton").text(Drupal.t("No"));
										    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
										    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
										    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
										    $("#delete-msg-wizard").show();

											$('.ui-dialog-titlebar-close').click(function(){
										        $("#delete-msg-wizard").remove();
										    });
											$('.admin-save-button-middle-bg').click(function(){
										        $("#delete-msg-wizard").remove();
										    });
											$('.removebutton').click(function(){
										        $("#delete-msg-wizard").remove();
										    });
											if(this.currTheme == 'expertusoneV2'){
										    	changeDialogPopUI();
										    }




										}
									else{
										var show = $('#publish-unpublish-'+catalogId).html();
										if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
											var esignObj = {'popupDiv':'catalog-publish-unpublis-dialog',
													'esignFor':'PublishUnpublishCatalog','catalogId':catalogId,'catalogType': catalogType,'actionStatus': actionStatus, 'rowObj': rowObj};
											$.fn.getNewEsignPopup(esignObj); // #0040182 This Function is Missing
										}else{
											$("#root-admin").data("narrowsearch").publishAndUnpublishCat(catalogId, catalogType,actionStatus, rowObj);

										}
									}

								}
							});

						},

						publishAndUnpublishCat : function(catalogId, catalogType,actionStatus, rowObj){
							var obj = this;
							//$("#class-detail-loader-"+catalogId).remove();
							//var cRow = $(rowObj).parents("tr").parents("tr").before("<tr><td colspan='3'><div id='class-detail-loader-"+catalogId+"'></div></td></tr>");
							//this.createLoader("class-detail-loader-"+catalogId);
							url = obj.constructUrl("ajax/administration/learning/catalog/publish-and-unpublish" + '/' + catalogId + '/' + catalogType + '/' + actionStatus);
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
								result = result.split('|');
									if(result[0]=='lrn_crs_sts_atv' || result[0]=='lrn_cls_sts_atv'){
										$("#publish-unpublish-"+catalogId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+catalogId).addClass("crs-unpublish-btn");
									}
									else if(result[0]=='lrn_crs_sts_itv' || result[0]=='lrn_cls_sts_itv'){
										$("#publish-unpublish-"+catalogId).removeClass("crs-unpublish-btn");
										$("#publish-unpublish-"+catalogId).addClass("crs-publish-btn");
									}
									else if(result[0]=='lrn_cls_sts_dld'){
										$("#publish-unpublish-"+catalogId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+catalogId).removeClass("action-crs-publish-btn");
										$("#publish-unpublish-"+catalogId).addClass("crs-pub-enrolled-btn");
										$("#publish-unpublish-"+catalogId).removeAttr("onclick").unbind("click");
									}else if(result[0]=='lrn_cls_sts_can'){
										$("#publish-unpublish-"+catalogId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+catalogId).removeClass("action-crs-publish-btn");
										$("#publish-unpublish-"+catalogId).addClass("crs-pub-enrolled-btn");
										$("#publish-unpublish-"+catalogId).removeAttr("onclick").unbind("click");
									}
									$("#publish-unpublish-"+catalogId).html(result[1]);
								}
							});
						},

						publishAndUnpublishProgram : function(programId, rowObj) {
							 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
							    	var esignObj = {'popupDiv':'program-publish-unpublis-dialog',
							    			'esignFor':'PublishUnpublishProgram','programId':programId,'rowObj': rowObj};
							    	 $.fn.getNewEsignPopup(esignObj);
							 }else{
								 $("#root-admin").data("narrowsearch").publishAndUnpublishPrg(programId, rowObj);
							 }

						},

						publishAndUnpublishPrg : function(programId, rowObj){
							var obj = this;
							url = obj.constructUrl("ajax/administration/learning/program/publish-and-unpublish" + '/' + programId);
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
								result = result.split('|');
									if(result[0]=='lrn_lpn_sts_atv'){
										$("#publish-unpublish-"+programId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+programId).addClass("crs-unpublish-btn");
									}
									else if(result[0]=='lrn_lpn_sts_itv'){
										$("#publish-unpublish-"+programId).removeClass("crs-unpublish-btn");
										$("#publish-unpublish-"+programId).addClass("crs-publish-btn");
									}
									$("#publish-unpublish-"+programId).html(result[1]);
								}
							});
						},
						publishAndUnpublishSurveyAssessment : function(surveyId, surveyType, rowObj) {
							 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
							    	var esignObj = {'popupDiv':'survey-publish-unpublis-dialog',
							    			'esignFor':'publishAndUnpublishSurveyAssessment','surveyId':surveyId,'surveyType':surveyType,'rowObj': rowObj};
							    	 $.fn.getNewEsignPopup(esignObj);
							 }else{
								 $("#root-admin").data("narrowsearch").publishAndUnpublishSur(surveyId, surveyType, rowObj);
							 }

						},

						publishAndUnpublishSur : function(surveyId, surveyType, rowObj){
							var obj = this;
							if(surveyType =='Assessment'){
								url = obj.constructUrl("ajax/administration/assessment/survey-assessment/publish-and-unpublish" + '/' + surveyId + '/' + surveyType);
							}
							else{
								url = obj.constructUrl("ajax/administration/survey/survey-assessment/publish-and-unpublish" + '/' + surveyId + '/' + surveyType);
							}
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
								result = result.split('|');
									if(result[0]=='sry_det_sry_atv'){
										$("#publish-unpublish-"+surveyId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+surveyId).addClass("crs-unpublish-btn");
									}
									else if(result[0]=='sry_det_sry_itv'){
										$("#publish-unpublish-"+surveyId).removeClass("crs-unpublish-btn");
										$("#publish-unpublish-"+surveyId).addClass("crs-publish-btn");
									}
									$("#publish-unpublish-"+surveyId).html(result[1]);
								}
							});
						},

						publishAndUnpublishSurveyAssessmentQuestions : function(questionId, rowObj) {
							 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
							    	var esignObj = {'popupDiv':'survey-publish-unpublis-dialog',
							    			'esignFor':'publishAndUnpublishSurveyAssessmentQuestions','questionId':questionId,'rowObj': rowObj};
							    	 $.fn.getNewEsignPopup(esignObj);
							 }else{
								 $("#root-admin").data("narrowsearch").publishAndUnpublishQus(questionId, rowObj);
							 }

						},

						publishAndUnpublishQus : function(questionId, rowObj){
							var obj = this;
							url = obj.constructUrl("ajax/administration/survey/survey-assessment-questions/publish-and-unpublish" + '/' + questionId );
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
								result = result.split('|');
									if(result[0]=='sry_qtn_sts_atv'){
										$("#publish-unpublish-"+questionId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+questionId).addClass("crs-unpublish-btn");
									}
									else if(result[0]=='sry_qtn_sts_itv'){
										$("#publish-unpublish-"+questionId).removeClass("crs-unpublish-btn");
										$("#publish-unpublish-"+questionId).addClass("crs-publish-btn");
									}
									$("#publish-unpublish-"+questionId).html(result[1]);
								}
							});
						},
			setPositionToCtoolPop : function(path) {
//#custom_attribute_0078975
				// console.log('hello setPositionToCtoolPop');

				if(path.indexOf('group') > 1 || path.indexOf('customattribute') > 1){
					$('#create-dd-list').css('visibility','hidden');
				}
				if(path.indexOf('course-class') > 1 || path.indexOf('program') > 1 || path.indexOf('banner') > 1
						|| path.indexOf('announcement') > 1) {
					var v = $(window).scrollTop();
					if(v < 150) {
						$(window).scrollTop(150);
					}
				}
				// Added by Vincent on 07, Jan 2014 for #0029687: Refresh on Admin Enrollments page
				updatePaginationCookie(1);
				//Custom Attributes for User Registration in Site settings page #custom_attribute_0078975
				setTimeout(function(){
					$('#userregister_scrollbar_container').jScrollPane();
				}, 500);
			},

            publishAndUnpublishContent : function(contentId, rowObj) {
              if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
                   var esignObj = {'popupDiv':'content-publish-unpublis-dialog',
                       'esignFor':'publishAndUnpublishContent', 'contentId':contentId, 'rowObj': rowObj};
                    $.fn.getNewEsignPopup(esignObj);
              }else{
                $("#root-admin").data("narrowsearch").publishAndUnpublishCnt(contentId, rowObj);
              }

           },

           publishAndUnpublishCnt : function(contentId, rowObj){
             var obj = this;
             url = obj.constructUrl("ajax/administration/manage/content/publish-and-unpublish" + '/' + contentId );
             $.ajax({
               type: "POST",
               url: url,
               data:  '',
               success: function(result){
               result = $.trim(result);
               result = result.split('|');
                 if(result[0]=='lrn_cnt_sts_atv'){
                   $("#publish-unpublish-"+contentId).removeClass("crs-publish-btn");
                   $("#publish-unpublish-"+contentId).addClass("crs-unpublish-btn");
                 }
                 else if(result[0]=='lrn_cnt_sts_itv'){
                   $("#publish-unpublish-"+contentId).removeClass("crs-unpublish-btn");
                   $("#publish-unpublish-"+contentId).addClass("crs-publish-btn");
                 }
                 $("#publish-unpublish-"+contentId).html(result[1]);
               }
             });
           },
           
           
           
           publishAndUnpublishVideo : function(contentId, rowObj) {
               if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
                    var esignObj = {'popupDiv':'content-publish-unpublis-dialog',
                        'esignFor':'publishAndUnpublishVideo', 'contentId':contentId, 'rowObj': rowObj};
                     $.fn.getNewEsignPopup(esignObj);
               }else{
                 $("#root-admin").data("narrowsearch").publishAndUnpublishCntVideo(contentId, rowObj);
               }

            },

            publishAndUnpublishCntVideo : function(contentId, rowObj){
              var obj = this;
              url = obj.constructUrl("ajax/administration/contentauthor/video/publish-and-unpublish" + '/' + contentId );
              $.ajax({
                type: "POST",
                url: url,
                data:  '',
                success: function(result){
                result = $.trim(result);
                result = result.split('|');
                  if(result[0]=='lrn_cnt_sts_atv'){
                    $("#publish-unpublish-"+contentId).removeClass("crs-publish-btn");
                    $("#publish-unpublish-"+contentId).addClass("crs-unpublish-btn");
                    $("#share-visibility-"+contentId).removeClass("tab-disable");
                    $("#share-visibility-"+contentId).parent().parent().find(".disable-share-tab-icon").addClass("share-tab-icon");
                    $("#share-visibility-"+contentId).parent().parent().find(".disable-share-tab-icon").removeClass("disable-share-tab-icon");
                    $("#share-visibility-"+contentId).attr("onclick","javascript:void(0);");
                    		$("#share-visibility-"+contentId).click(function()
                    		{
                    										callVisibility({'entityId':contentId,
      														 'entityType':'cre_sys_obt_cnt',
      														 'url':'administration/contentauthor/content-share/'+contentId+'/cre_sys_obt_cnt/0',
      														 'popupDispId':'narrow_search_qtip_share_disp__cre_sys_obt_cnt',
      														 'catalogVisibleId':'narrow_search_qtipShareqtip_visible_disp__cre_sys_obt_cnt',
      														 'wid':645,
      														 'heg':'300',
      														 'postype':'middle',
      														 'poslwid':'50',
      														 'linkid':'share-visibility-'+contentId});
      						});
      														 
                  }
                  else if(result[0]=='lrn_cnt_sts_itv'){
                    $("#publish-unpublish-"+contentId).removeClass("crs-unpublish-btn");
                    $("#publish-unpublish-"+contentId).addClass("crs-publish-btn");
                    $("#share-visibility-"+contentId).addClass("tab-disable");
                    $("#share-visibility-"+contentId).addClass("tab-disable");
                    $("#share-visibility-"+contentId).parent().parent().find(".share-tab-icon").addClass("disable-share-tab-icon");
                    $("#share-visibility-"+contentId).parent().parent().find(".share-tab-icon").removeClass("share-tab-icon");
                    $("#share-visibility-"+contentId).attr("onclick","javascript:void(0);");
                    $("#share-visibility-"+contentId).unbind("click",null);
                  }
                  $("#publish-unpublish-"+contentId).html(result[1]);
                }
              });
            },
            
            
            publishAndUnpublishPresentation : function(contentId, rowObj) {
                if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
                     var esignObj = {'popupDiv':'content-publish-unpublis-dialog',
                         'esignFor':'publishAndUnpublishPresentation', 'contentId':contentId, 'rowObj': rowObj};
                      $.fn.getNewEsignPopup(esignObj);
                }else{
                  $("#root-admin").data("narrowsearch").publishAndUnpublishCntPresentation(contentId, rowObj);
                }

             },

            
             
             publishAndUnpublishCntPresentation : function(contentId, rowObj){
                 var obj = this;
                 url = obj.constructUrl("ajax/administration/contentauthor/presentation/publish-and-unpublish" + '/' + contentId );
                 $.ajax({
                   type: "POST",
                   url: url,
                   data:  '',
                   success: function(result){
                   result = $.trim(result);
                   result = result.split('|');
                     if(result[0]=='lrn_cnt_sts_atv'){
                       $("#publish-unpublish-"+contentId).removeClass("crs-publish-btn");
                       $("#publish-unpublish-"+contentId).addClass("crs-unpublish-btn");
                   $("#share-visibility-"+contentId).removeClass("tab-disable");
                      $("#share-visibility-"+contentId).parent().parent().find(".disable-share-tab-icon").addClass("share-tab-icon");
                      $("#share-visibility-"+contentId).parent().parent().find(".disable-share-tab-icon").removeClass("disable-share-tab-icon");
                      $("#share-visibility-"+contentId).attr("onclick","javascript:void(0);");
                      		$("#share-visibility-"+contentId).click(function()
                      		{
                      										callVisibility({'entityId':contentId,
        														 'entityType':'cre_sys_obt_cnt',
        														 'url':'administration/contentauthor/content-share/'+contentId+'/cre_sys_obt_cnt/0',
        														 'popupDispId':'narrow_search_qtip_share_disp__cre_sys_obt_cnt',
        														 'catalogVisibleId':'narrow_search_qtipShareqtip_visible_disp__cre_sys_obt_cnt',
        														 'wid':645,
        														 'heg':'300',
        														 'postype':'middle',
        														 'poslwid':'50',
        														 'linkid':'share-visibility-'+contentId});
        						});
        								   
                     }
                     else if(result[0]=='lrn_cnt_sts_itv'){
                       $("#publish-unpublish-"+contentId).removeClass("crs-unpublish-btn");
                       $("#publish-unpublish-"+contentId).addClass("crs-publish-btn");
                       $("#share-visibility-"+contentId).addClass("tab-disable");
                       $("#share-visibility-"+contentId).addClass("tab-disable");
                       $("#share-visibility-"+contentId).parent().parent().find(".share-tab-icon").addClass("disable-share-tab-icon");
                       $("#share-visibility-"+contentId).parent().parent().find(".share-tab-icon").removeClass("share-tab-icon");
                       $("#share-visibility-"+contentId).attr("onclick","javascript:void(0);");
                       $("#share-visibility-"+contentId).unbind("click",null);
                    
                     }
                     $("#publish-unpublish-"+contentId).html(result[1]);
                   }
                 });
               },
              
             
            
            

           publishAndUnpublishTax : function(taxId, rowObj) {
               if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
                    var esignObj = {'popupDiv':'tax-publish-unpublis-dialog',
                        'esignFor':'publishAndUnpublishTax', 'taxId':taxId, 'rowObj': rowObj};
                     $.fn.getNewEsignPopup(esignObj);
               }else{
                 $("#root-admin").data("narrowsearch").publishAndUnpublishTaxDialog(taxId, rowObj);
               }

            },

            publishAndUnpublishTaxDialog : function(taxId, rowObj){
              var obj = this;
              url = obj.constructUrl("ajax/administration/commerce/tax/publish-and-unpublish" + '/' + taxId );
              $.ajax({
                type: "POST",
                url: url,
                data:  '',
                success: function(result){
                obj.destroyLoader("paint-narrow-search-results");
                result = $.trim(result);
                result = result.split('|');
                  if(result[0]=='cme_tax_sts_atv'){
                    $("#publish-unpublish-"+taxId).removeClass("crs-publish-btn");
                    $("#publish-unpublish-"+taxId).addClass("crs-unpublish-btn");
                  }
                  else if(result[0]=='cme_tax_sts_itv'){
                    $("#publish-unpublish-"+taxId).removeClass("crs-unpublish-btn");
                    $("#publish-unpublish-"+taxId).addClass("crs-publish-btn");
                  }
                  $("#publish-unpublish-"+taxId).html(result[1]);
                }
              });
            },

            publishAndUnpublishDiscount : function(discountId, rowObj,allowPublishorNot) {
            	allowPublishorNot = (typeof allowPublishorNot == 'undefined') ? 0 : allowPublishorNot;
            	this.currTheme = Drupal.settings.ajaxPageState.theme;

				if(allowPublishorNot == 1){
					var uniqueClassPopup = '';
					 $('#delete-msg-wizard').remove();
					    var html = '';
					    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
					    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
					    if(this.currTheme == 'expertusoneV2'){
					   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+Drupal.t("ERR248")+'</td></tr>';
					    } else {
					     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+Drupal.t("ERR248")+'</td></tr>';
					    }
					    html+='</table>';
					    html+='</div>';
					    $("body").append(html);

					    var closeButt={};
					    closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

					    $("#delete-msg-wizard").dialog({
					        position:[(getWindowWidth()-400)/2,200],
					        bgiframe: true,
					        width:300,
					        resizable:false,
					        modal: true,
					        title:Drupal.t('LBL749'),
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
					    $('.ui-dialog').wrap("<div id='catalog-publish-unpublis-dialog' class='"+uniqueClassPopup+"'></div>");
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
					    $("#delete-msg-wizard").show();

						$('.ui-dialog-titlebar-close').click(function(){
					        $("#delete-msg-wizard").remove();
					    });

						if(this.currTheme == 'expertusoneV2'){
					    	changeDialogPopUI();
					    }
					}else{
						if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
		                     var esignObj = {'popupDiv':'discount-publish-unpublis-dialog',
		                         'esignFor':'publishAndUnpublishDiscount', 'discountId':discountId, 'rowObj': rowObj};
		                      $.fn.getNewEsignPopup(esignObj);
		                }else{
		                  $("#root-admin").data("narrowsearch").publishAndUnpublishDiscountDialog(discountId, rowObj);
		                }
					}


             },

             publishAndUnpublishDiscountDialog : function(discountId, rowObj){
               var obj = this;
               url = obj.constructUrl("ajax/administration/commerce/discounts/publish-and-unpublish" + '/' + discountId );
               $.ajax({
                 type: "POST",
                 url: url,
                 data:  '',
                 success: function(result){
                 obj.destroyLoader("paint-narrow-search-results");
                 result = $.trim(result);
                 result = result.split('|');
                   if(result[0] == 1){
                     $("#publish-unpublish-"+discountId).removeClass("crs-publish-btn");
                     $("#publish-unpublish-"+discountId).addClass("crs-unpublish-btn");
                   }
                   else if(result[0] == 0){
                     $("#publish-unpublish-"+discountId).removeClass("crs-unpublish-btn");
                     $("#publish-unpublish-"+discountId).addClass("crs-publish-btn");
                   }
                   $("#publish-unpublish-"+discountId).html(result[1]);
                 }
               });
             },

			 publishAndUnpublishGroupDialog : function(objectId, objectType, rowObj,is_admin,owner_cnt) {
				 	var title = Drupal.t('MSG773');
				 	var suspend = $('#action_icon_tooltip_'+objectId).hasClass( "Suspend" );
				 	if(suspend ){
						var uniqueClassPopup = '';
					    $('#delete-msg-wizard').remove();
					    var html = '';
					    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
					    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
					   
					    if(this.currTheme == 'expertusoneV2'){
					   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
					    } else {
					     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+'</td></tr>';
					    }
					    html+='</table>';
					    html+='</div>';
					    $("body").append(html);
	
					    var closeButt={};
					    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};
	
		                 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
		                      var esignObj = {'popupDiv':'object-activate-deactivate-dialog',
								    			'esignFor':'ActivateDeactivateObject','objectId':objectId,'objectType':objectType,'rowObj': rowObj};
		                      closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj); };
		                 }else{
		                	 closeButt[Drupal.t('Yes')]=function(){ $("#root-admin").data("narrowsearch").activateAndDeactivateObj(objectId, objectType, rowObj); };
		                 }
	
					    $("#delete-msg-wizard").dialog({
					        position:[(getWindowWidth()-400)/2,200],
					        bgiframe: true,
					        width:300,
					        resizable:false,
					        modal: true,
					        title:Drupal.t('Groups'),
					        buttons:closeButt,
					        closeOnEscape: false,
					        draggable:false,
					        overlay:
					         {
					           opacity: 0.9,
					           background: "black"
					         }
					    });
					    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");
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
				    }else{
				    	 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
					    	var esignObj = {'popupDiv':'object-activate-deactivate-dialog',
								    			'esignFor':'ActivateDeactivateObject','objectId':objectId,'objectType':objectType,'rowObj': rowObj};
					    	 $.fn.getNewEsignPopup(esignObj);
						 }else{
							 $("#root-admin").data("narrowsearch").activateAndDeactivateObj(objectId, objectType, rowObj);
						 }
				    }

           },
             publishAndUnpublishOrder : function(orderId, orderStatus, rowObj) {
					var uniqueClassPopup = '';
				    $('#delete-msg-wizard').remove();
				    var html = '';
				    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
				    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
				    //html+= '<tr><td height="40px">&nbsp;</td></tr>';
				    if(orderStatus == 'payment_received'){
	            		var title = Drupal.t('LBL1163');
	            	}else if(orderStatus == 'canceled'){
	            		var title = Drupal.t('LBL1164');
	            	}
				    if(this.currTheme == 'expertusoneV2'){
				   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
				    } else {
				     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+'</td></tr>';
				    }
				    html+='</table>';
				    html+='</div>';
				    $("body").append(html);

				    var closeButt={};
				    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

	                 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
	                      var esignObj = {'popupDiv':'order-publish-unpublis-dialog',
	                          'esignFor':'publishAndUnpublishOrder', 'orderId':orderId, 'orderStatus':orderStatus, 'rowObj': rowObj};
	                      closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj); };
	                 }else{
	                	 closeButt[Drupal.t('Yes')]=function(){ $("#root-admin").data("narrowsearch").publishAndUnpublishOrderDialog(orderId, orderStatus, rowObj); };
	                 }

				    $("#delete-msg-wizard").dialog({
				        position:[(getWindowWidth()-400)/2,200],
				        bgiframe: true,
				        width:300,
				        resizable:false,
				        modal: true,
				        title:Drupal.t('LBL820')+' - '+orderId,
				        buttons:closeButt,
				        closeOnEscape: false,
				        draggable:false,
				        overlay:
				         {
				           opacity: 0.9,
				           background: "black"
				         }
				    });
				    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");
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

           },

           publishAndUnpublishOrderDialog : function(orderId, orderStatus, rowObj){
         	$('#delete-msg-wizard').remove();
             var obj = this;
             obj.createLoader("main-wrapper");
             url = obj.constructUrl("administration/commerce/orderupdate/ajax" + '/' + orderId + '/' + orderStatus );
             $.ajax({
               type: "POST",
               url: url,
               data:  '',
               success: function(result){
             	//obj.destroyLoader("paint-narrow-search-results");
				if (typeof $("#root-admin").data("narrowsearch").refreshLastAccessedRow != 'undefined' && $("#root-admin").data("narrowsearch").refreshLastAccessedRow() == false) {
             	$("#narrow-search-results-holder").trigger("reloadGrid");
               }
               }
             });
           },


            publishAndUnpublishModule : function(moduleName, rowObj) {
            if(moduleName == 'exp_sp_administration_contentauthor' || moduleName == 'exp_sp_administration_customattribute')    // For enabling/disabling content Author
            	$('#delete-msg-wizard').remove();
              if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
                   var esignObj = {'popupDiv':'mdoule-publish-unpublis-dialog',
                       'esignFor':'publishAndUnpublishModule', 'moduleName':moduleName, 'rowObj': rowObj};
                    $.fn.getNewEsignPopup(esignObj);
              }else{
                $("#root-admin").data("narrowsearch").publishAndUnpublishModuleDialog(moduleName, rowObj);
              }

           },

           publishAndUnpublishModuleDialog : function(moduleName, rowObj){
             var obj = this;
             this.createLoader("main-wrapper");
             url = obj.constructUrl("ajax/administration/sitesetup/moduleinfo/publish-and-unpublish" + '/' + moduleName );
             var reloadgrid = obj.constructUrl("administration/sitesetup/moduleinfo/search/all");
             $.ajax({
               type: "POST",
               url: url,
               data:  '',
               success: function(result){
               result = $.trim(result);
               result = result.split('|');
                 if(result[0]==1){
                   $("#publish-unpublish-"+moduleName).removeClass("crs-publish-btn");
                   $("#publish-unpublish-"+moduleName).addClass("crs-unpublish-btn");
                   if(moduleName == 'exp_sp_administration_module_info_commerce' || moduleName == 'drupalchat'){
                	   window.location.href=window.location.href;
                   }
                 }
                 else if(result[0]==2){
                   $("#publish-unpublish-"+moduleName).removeClass("crs-unpublish-btn");
                   $("#publish-unpublish-"+moduleName).addClass("crs-publish-btn");
                   if(moduleName == 'exp_sp_administration_module_info_commerce' || moduleName == 'drupalchat'){
                	   window.location.href=window.location.href;
                   }
                 }
                 if(moduleName != 'exp_sp_administration_customattribute')
                 $("#publish-unpublish-"+moduleName).html(result[1]);
               //  $("#root-admin").data("narrowsearch").destroyLoader("main-wrapper");
               //  $("#root-admin").data("narrowsearch").createLoader("paint-narrow-search-results");
                 $('#narrow-search-results-holder').setGridParam({url:reloadgrid});
				 if (typeof $("#root-admin").data("narrowsearch").refreshLastAccessedRow != 'undefined' && $("#root-admin").data("narrowsearch").refreshLastAccessedRow() == false) {
				 	$('#narrow-search-results-holder').trigger("reloadGrid",[{page:$(".ui-pg-input").val()}]);
				 }
               }
             });
           },

           publishAndUnpublishPayment : function(paymentId, rowObj) {
             if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
                  var esignObj = {'popupDiv':'payment-publish-unpublis-dialog',
                      'esignFor':'publishAndUnpublishPayment', 'paymentId':paymentId, 'rowObj': rowObj};
                   $.fn.getNewEsignPopup(esignObj);
             }else{
               $("#root-admin").data("narrowsearch").publishAndUnpublishPaymentDialog(paymentId, rowObj);
             }

          },

          publishAndUnpublishPaymentDialog : function(paymentId, rowObj){
            var obj = this;
            this.createLoader("admin-commerce-payment-method-plan");
            url = obj.constructUrl("administration/commerce/setting/payment/action/"+paymentId);
            $.ajax({
              type: "POST",
              url: url,
              data:  '',
              success: function(result){
              result = $.trim(result);
              result = result.split('|');
                if(result[0]==1){
                  $("#publish-unpublish-"+paymentId).removeClass("crs-publish-btn");
                  $("#publish-unpublish-"+paymentId).addClass("crs-unpublish-btn");
                }

                else if(result[0]==0){
                  $("#publish-unpublish-"+paymentId).removeClass("crs-unpublish-btn");
                  $("#publish-unpublish-"+paymentId).addClass("crs-publish-btn");
                }
                $("#publish-unpublish-"+paymentId).html(result[1]);
                $("#root-admin").data("narrowsearch").destroyLoader("admin-commerce-payment-method-plan");

              }
            });
          },

           publishAndUnpublishBanner : function(bannerId, bannerType, rowObj) {
				 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
				    	var esignObj = {'popupDiv':'banner-publish-unpublis-dialog',
				    			'esignFor':'publishAndUnpublishBanner','bannerId':bannerId,'bannerType':bannerType,'rowObj': rowObj};
				    	 $.fn.getNewEsignPopup(esignObj);
				 }else{
					 $("#root-admin").data("narrowsearch").publishAndUnpublishBan(bannerId, bannerType, rowObj);
				 }

			},
//#custom_attribute_0078975
			publishAndUnpublishCustom : function(CustomId, rowObj) {
				 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
				    	var esignObj = {'popupDiv':'Custom-publish-unpublis-dialog',
				    			'esignFor':'publishAndUnpublishCustom','CustomId':CustomId,'rowObj': rowObj};
				    	// console.log('esignObj'+JSON.stringify(esignObj));
				    	 $.fn.getNewEsignPopup(esignObj);
				 }else{
					 $("#root-admin").data("narrowsearch").publishAndUnpublishCus(CustomId, rowObj);
				 }

			},

			publishAndUnpublishAnnouncement : function(annId, annType, rowObj) {
				 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
				    	var esignObj = {'popupDiv':'announcement-publish-unpublis-dialog',
				    			'esignFor':'publishAndUnpublishAnnouncement','annId':annId,'annType':annType,'rowObj': rowObj};
				    	 $.fn.getNewEsignPopup(esignObj);
				 }else{
					 $("#root-admin").data("narrowsearch").publishAndUnpublishAnn(annId, annType, rowObj);
				 }

			},
			publishAndUnpublishAnn : function(annId, annType, rowObj){
				var obj = this;
				url = obj.constructUrl("ajax/administration/manage/announcement/publish-and-unpublish" + '/' + annId + '/' + annType);
				$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
					result = $.trim(result);
					result = result.split('|');
						if(result[0]=='cre_sys_obt_not_atv'){
							$("#publish-unpublish-"+annId).removeClass("crs-publish-btn");
							$("#publish-unpublish-"+annId).addClass("crs-unpublish-btn");
						}
						else if(result[0]=='cre_sys_obt_not_itv'){
							$("#publish-unpublish-"+annId).removeClass("crs-unpublish-btn");
							$("#publish-unpublish-"+annId).addClass("crs-publish-btn");
						}
						$("#publish-unpublish-"+annId).html(result[1]);
					}
				});
			},
			publishAndUnpublishBan : function(bannerId, bannerType, rowObj){
				var obj = this;
				url = obj.constructUrl("ajax/administration/manage/banner/publish-and-unpublish" + '/' + bannerId + '/' + bannerType);
				$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
					result = $.trim(result);
					result = result.split('|');
						if(result[0]=='cbn_anm_sts_atv'){
							$("#publish-unpublish-"+bannerId).removeClass("crs-publish-btn");
							$("#publish-unpublish-"+bannerId).addClass("crs-unpublish-btn");
						}
						else if(result[0]=='cbn_anm_sts_itv'){
							$("#publish-unpublish-"+bannerId).removeClass("crs-unpublish-btn");
							$("#publish-unpublish-"+bannerId).addClass("crs-publish-btn");
						}
						$("#publish-unpublish-"+bannerId).html(result[1]);
					}
				});
			},
//#custom_attribute_0078975
			publishAndUnpublishCus : function(CustomId, rowObj){
				var obj = this;
				url = obj.constructUrl("ajax/administration/manage/customattribute/publish-and-unpublish" + '/' + CustomId );
				

				$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
					result = $.trim(result);
					result = result.split('|');

						if(result[0]=='cre_cattr_sts_atv'){
							

							$("#publish-unpublish-"+CustomId).removeClass("crs-publish-btn");
							$("#publish-unpublish-"+CustomId).addClass("crs-unpublish-btn");
						}
						else if(result[0]=='cre_cattr_sts_itv'){
							

							$("#publish-unpublish-"+CustomId).removeClass("crs-unpublish-btn");
							$("#publish-unpublish-"+CustomId).addClass("crs-publish-btn");
						}
						$("#publish-unpublish-"+CustomId).html(result[1]);
					}
				});
			},
			publishAndUnpublishLocation : function(locationId, rowObj) {
				 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
				    	var esignObj = {'popupDiv':'location-publish-unpublis-dialog',
				    			'esignFor':'publishAndUnpublishLocation','locationId':locationId,'rowObj': rowObj};
				    	 $.fn.getNewEsignPopup(esignObj);
				 }else{
					 $("#root-admin").data("narrowsearch").publishAndUnpublishLoc(locationId, rowObj);
				 }

			},

			publishAndUnpublishLoc : function(locationId, rowObj){
				var obj = this;
				url = obj.constructUrl("ajax/administration/manage/location/publish-and-unpublish" + '/' + locationId );
				$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
						result = $.trim(result);
						result = result.split('|');
						if(result[0]=='not_able_do'){
							this.currTheme = Drupal.settings.ajaxPageState.theme;

							var show =	$.trim($('#publish-unpublish-'+locationId).html());
							var uniqueClassPopup = 'unique-delete-class';
							var wSize = (wSize) ? wSize : 300;
						    $('#delete-msg-wizard').remove();
						    var html = '';
						    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
						    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';

						   // html+= '<tr><td height="40px">&nbsp;</td></tr>';
						    if(this.currTheme == 'expertusoneV2'){
						   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+Drupal.t("The location is associated to class")+'</td></tr>';
						    } else {
						     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+Drupal.t("The location is associated to class")+'</td></tr>';
						    }
						    html+='</table>';
						    html+='</div>';
						    $("body").append(html);
						    var closeButt={};
						    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};


						    $("#delete-msg-wizard").dialog({
						        position:[(getWindowWidth()-400)/2,200],
						        bgiframe: true,
						        width:wSize,
						        resizable:false,
						        modal: true,
						        title:result[1],//"title",
						        buttons:closeButt,
						        closeOnEscape: false,
						        draggable:false,
						        overlay:
						         {
						           opacity: 0.9,
						           background: "black"
						         }
						    });
						    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");

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
							/*-- 37211: Unable to delete a class in FF version 32.0 --*/
						    if($('div.qtip-defaults').length > 0) {
						    	var prevZindex = $('.qtip-defaults').css('z-index');
						    	$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
						    	$('.ui-widget-overlay').css('z-index', prevZindex+1);
						    }
						}else{
							if(result[0]=='lrn_res_loc_atv'){
								$("#publish-unpublish-"+locationId).removeClass("crs-publish-btn");
								$("#publish-unpublish-"+locationId).addClass("crs-unpublish-btn");
							}
							else if(result[0]=='lrn_res_loc_itv'){
								$("#publish-unpublish-"+locationId).removeClass("crs-unpublish-btn");
								$("#publish-unpublish-"+locationId).addClass("crs-publish-btn");
							}
							$("#publish-unpublish-"+locationId).html(result[1]);
						}
					}
				});
			},


			publishAndUnpublishNotification : function(notificationId, notificationType) {
				 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
				    	var esignObj = {'popupDiv':'notification-publish-unpublis-dialog',
				    			'esignFor':'publishAndUnpublishNotification','notificationId':notificationId,'notificationType':notificationType};
				    	 $.fn.getNewEsignPopup(esignObj);
				 }else{
					 $("#root-admin").data("narrowsearch").publishAndUnpublishNot(notificationId, notificationType);
				 }

			},

			publishAndUnpublishNot : function(notificationId, notificationType){
				var obj = this;
				if(notificationType == 'notification_template'){
				  url = obj.constructUrl("ajax/administration/manage/notification_template/publish-and-unpublish" + '/' + notificationId );
				}else {
				  url = obj.constructUrl("ajax/administration/manage/certificate/publish-and-unpublish" + '/' + notificationId );
				}
				$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
					result = $.trim(result);
					result = result.split('|');
						if(result[0]=='cre_ntn_sts_atv'){
							$("#publish-unpublish-"+notificationId).removeClass("crs-publish-btn");
							$("#publish-unpublish-"+notificationId).addClass("crs-unpublish-btn");
						}
						else if(result[0]=='cre_ntn_sts_itv'){
							$("#publish-unpublish-"+notificationId).removeClass("crs-unpublish-btn");
							$("#publish-unpublish-"+notificationId).addClass("crs-publish-btn");
						}
						$("#publish-unpublish-"+notificationId).html(result[1]);
					}
				});
			},




						activateAndDeactivateResource : function(resourceId, resourceType, rowObj) {
							 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
							    	var esignObj = {'popupDiv':'program-publish-unpublis-dialog',
							    			'esignFor':'ActivateDeactivateResource','resourceId':resourceId,'resourceType':resourceType,'rowObj': rowObj};
							    	 $.fn.getNewEsignPopup(esignObj);
							 }else{
								 $("#root-admin").data("narrowsearch").activateAndDeactivateRsc(resourceId, resourceType, rowObj);
							 }

						},

						activateAndDeactivateRsc : function(resourceId, resourceType,rowObj){
							var obj = this;
							url = obj.constructUrl("ajax/administration/learning/resource/activate-and-deactivate" + '/' + resourceId+ '/' + resourceType);
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
									if(result=='lrn_res_loc_atv' || result=='lrn_res_fac_atv' || result=='lrn_res_rms_atv'){
										$("#publish-unpublish-"+resourceType+"-"+resourceId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+resourceType+"-"+resourceId).addClass("crs-unpublish-btn");
										$("#publish-unpublish-"+resourceType+"-"+resourceId).html("Deactivate");
									}
									else if(result=='lrn_res_loc_itv' || result=='lrn_res_fac_itv' || result=='lrn_res_rms_itv'){
										$("#publish-unpublish-"+resourceType+"-"+resourceId).removeClass("crs-unpublish-btn");
										$("#publish-unpublish-"+resourceType+"-"+resourceId).addClass("crs-publish-btn");
										$("#publish-unpublish-"+resourceType+"-"+resourceId).html("Activate");
									}
								}
							});
						},

						activateAndDeactivateObject : function(objectId, objectType, rowObj) {
							 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
							    	var esignObj = {'popupDiv':'object-activate-deactivate-dialog',
							    			'esignFor':'ActivateDeactivateObject','objectId':objectId,'objectType':objectType,'rowObj': rowObj};
							    	 $.fn.getNewEsignPopup(esignObj);
							 }else{
								 $("#root-admin").data("narrowsearch").activateAndDeactivateObj(objectId, objectType, rowObj);
							 }

						},

						activateAndDeactivateObj : function(objectId, objectType, rowObj){
							//alert(objectType);
							var obj = this;
							if(objectType=='User'){
								url = obj.constructUrl("ajax/administration/people/activate-and-deactivate" + '/' + objectId);
							}
							else if(objectType=='Organization'){
								url = obj.constructUrl("ajax/administration/people/organization/activate-and-deactivate" + '/' + objectId);
							}
							else if(objectType=='Grp'){
								$("#delete-msg-wizard").remove();
								url = obj.constructUrl("ajax/administration/people/group/activate-and-deactivate" + '/' + objectId);
							}
							else if(objectType=='location'){
								url = obj.constructUrl("ajax/administration/location/activate-and-deactivate" + '/' + objectId);
							}
							else if(objectType=='surveyDetails'){
								url = obj.constructUrl("ajax/administration/survey/survey-assessment/publish-and-unpublish" + '/' + objectId + '/'+objectType);
							}
							else if(objectType=='assessmentDetails'){
								url = obj.constructUrl("ajax/administration/assessment/survey-assessment/publish-and-unpublish" + '/' + objectId + '/'+objectType);
							}
							else if(objectType=='surveyQuestion'){
								url = obj.constructUrl("ajax/administration/survey/survey-assessment-questions/publish-and-unpublish" + '/' + objectId + '/'+objectType);
							}
							else if(objectType=='assessmentQuestion'){
								url = obj.constructUrl("ajax/administration/assessment/survey-assessment-questions/publish-and-unpublish" + '/' + objectId + '/'+objectType);
							}
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
								if(objectType=='User' && result=='activeinstr'){
									var error = new Array();
									error[0] = Drupal.t("MSG768");
									$('#show_suspend_message').remove();
							 	    var message = "<div id='show_suspend_message' style='position:absolute;left:0;right:0;'></div></div>";
							 	    var msg='<div id="message-container" style="visibility:visible"><div class="messages error"><ul><li><span>'+error[0]+'</span></li></ul><div class="msg-close-btn" onclick="$(\'#show_suspend_message\').remove();"></div></div></div><img onload="$(\'body\').data(\'learningcore\').showHideMultipleLi();" style="display:none;" src="sites/all/themes/core/expertusone/expertusone-internals/images/close.png" height="0" width="0"/>';
							 	    $('#gview_narrow-search-results-holder').prepend(message);
							 	    $("#gview_narrow-search-results-holder #show_suspend_message").append(msg);
								}else{
									$("#suspend_activate_"+objectId).html(result);
									//alert("objectId" + objectId + "result" + result);
									//44133: German-old-ui-Icon not changing when user is activate and suspend
									if(result==Drupal.t('LBL573')){
										$("#action_icon_tooltip_"+objectId).removeClass('Suspend');
										$("#action_icon_tooltip_"+objectId).addClass('Activate');

									}else{
										$("#action_icon_tooltip_"+objectId).removeClass('Activate');
										$("#action_icon_tooltip_"+objectId).addClass('Suspend');
									}
									if(objectType=='User'){
										$("#action_icon_tooltip_"+objectId).attr('title', result);
									}else{
										$("#suspend_activate_"+objectId).parent('span').attr('title', result);
									}
								}
								}
							});
						},

						initAdminLinks : function() {
							var curObj = this;
							var loadingDiv = "root-admin-search-right-col";

							/* Hide the filter and top search text box,result area */
							$("#narrow-search-text-filter-container").hide();
							$("#narrow-search").hide();
							$("#root-admin-results").hide();
							$("ul.AdminsublinktabNavigation li").removeClass("selected");

							// For Left Navigation Admin Menu Links
							$("ul.list-item-administrator li a").click(function(event,mainMenuId) {

									var linkId;
									if(mainMenuId=="" || mainMenuId=="undefined" || mainMenuId==null){
										 linkId = $(this).attr("id");
									}else{
										 linkId=mainMenuId;
									}
									var ancData = $("#"+linkId).metadata();
									var linkPath = ancData.link_path;

									/* Hides/Show narrow search and text filter */
									$("#narrow-search-text-filter-container").hide();
									$("#narrow-search").hide();
									$("#narrow-search-actionbar-list").hide();
									$("#root-admin-results").hide();

									curObj.createLoader(loadingDiv);

									var url = '?q=ajax/get/modules/'+linkPath;
									$.ajax({
											type : "POST",
											url : url,
											data : '',
											datatype : 'json',
											success : function(data) {
												curObj.destroyLoader(loadingDiv);

												$("#root-admin-results").html(data);
												$("#root-admin-results").show();
												var contentLoadingDiv = 'root-admin-results';

												/* For top tabular submenu in result page.*/
												$("ul.AdminsublinktabNavigation li a").click(function(event,firstSubmenuId){
													if($('#narrow-search-actionbar')){
													  $('#narrow-search-actionbar').hide();
													}

												  $("#narrow-search-text-filter-container").hide();
												  $("#narrow-search").hide();
												  $("#tab-content-main").html('');

													var submenuId;
													if(firstSubmenuId=="" || firstSubmenuId=="undefined" || firstSubmenuId==null){
														 submenuId = $(this).attr("id");
													}else{
														submenuId=firstSubmenuId;
													}

													var subAncData = $("#"+submenuId).metadata();

													/* Add class "selected"  if select  the tab */
													$( "ul.AdminsublinktabNavigation li").removeClass("selected");
													$("#"+ submenuId).parent("li").addClass("selected");

													/* Start - To get the submodules according to tab selection */
													curObj.createLoader(contentLoadingDiv);

													var sublinkPath=subAncData.link_path;
													var sublinkUrl = '?q='+sublinkPath;

													$.ajax({
														type : "POST",
														url : sublinkUrl,
														data : '',
														datatype : 'json',
														success : function(data) {
													     curObj.destroyLoader(contentLoadingDiv);
													     //console.log('ajax success data');
													     //console.log(data);
									   				   var detailsObj = jQuery.parseJSON(data);
									   				   //console.log('ajax success detailsObj');
                               //console.log(detailsObj);
											               // Extend Drupal.settings, if any settings are present in the data
												           if (detailsObj.drupal_settings) {
														     $.extend(true, Drupal.settings, detailsObj.drupal_settings);
														   }
														   /*Display the content for Tab wheather narrow search or non narrow search */
														   $("#tab-content-main").html(detailsObj.rendered_tab_content_main);

														   if(detailsObj.rendered_tab_content_type == "narrow_search"){ /* If narrow search */
															   /* Display left side filters and top bar*/
															   $("#narrow-search").html(detailsObj.rendered_narrow_search_filters);
															   $("#narrow-search").show();

															   // curObj.renderSearchResults();

															   $("#narrow-search-results-topbar").html(detailsObj.rendered_narrow_search_sortbar);
															   $("#narrow-search-actionbar").remove();
															   $("#admin-maincontent_tab").append(detailsObj.rendered_narrow_search_actionbar);
															   $("#narrow-search-results-topbar").show();

															   /* Initialise filters once modules loaded */
															   curObj.initPaths(detailsObj.rendered_tab_content_type);
															   curObj.initFilters();
															   curObj.renderSearchResults(linkPath,sublinkPath);
															   if($('#narrow-search-actionbar')){
													  			 $('#narrow-search-actionbar').hide();
															   }
														   }
														   else if (detailsObj.rendered_tab_content_type == "ajax-form"){
	                               Drupal.attachBehaviors();
														   }
													   } // end success
													}); /* End - To get the submodules according to tab selection */

													/* Show  the text filter */
													$("#narrow-search-actionbar-list").show();
												});

												/* Auto load first sub menu */
												var tmpFirstSubmenuId=$("ul.AdminsublinktabNavigation li a:first").attr("id");
												if(tmpFirstSubmenuId!="" && tmpFirstSubmenuId!="undefined" && tmpFirstSubmenuId!=null){
													 $("#"+tmpFirstSubmenuId).trigger("click",[tmpFirstSubmenuId]);
												}

											  }
										});

									/* Highlight selected menu links */
									$("ul.list-item-administrator li a").removeClass("root-admin-links-selected");
									$("#"+linkId).addClass("root-admin-links-selected");

									/* THE BELOW LINES WRITTEN BY NEW THEME BY VJ*/
									$("ul.list-item-administrator li.admin-left-panel-module-list").removeClass("root-admin-module-selected");
									$("#"+linkId).parent().addClass("root-admin-module-selected");

							 });

							/* Auto load main menu */
							var tmpFirstMainMenuId=$("ul.list-item-administrator li a:first").attr("id");
							//var tmpFirstMainMenuId="collaboration";
							if(tmpFirstMainMenuId!="" && tmpFirstMainMenuId!="undefined" && tmpFirstMainMenuId!=null){
							  $("#"+tmpFirstMainMenuId).trigger("click",[tmpFirstMainMenuId]);
							}

						},

						abstractFunctionAlert : function(functionName) {
							alert('UNDER CONSTRUCTION. Please implement function ' + functionName + ' in admin sub-module\'s js file.');
						},

						missingInitParamAlert : function(paramName) {
							alert('UNDER CONSTRUCTION. Missing init data ' + paramName);
						},

						narrowSearch : function() {
							var curObj=this;
							this.refreshGrid();
						},

						sortSearchResults : function(sortType, sortLinkHtmlId) {
							this.refreshGrid(sortType, sortLinkHtmlId);
						},

						narrowSearchByText : function() {
							this.refreshGrid();
						},

						narrowSearchByAddlText : function(code) {
							this.refreshGrid();
						},

						narrowSearchByDateRange : function(code) {
							this.refreshGrid();
						},

						narrowSearchBySlider : function(code) {
							this.refreshGrid();
						},

						printSearchResults : function() {
							if (this.printPath == undefined || this.printPath == null || this.printPath == '') {
								this.missingInitParamAlert('print_path');
								return; // Impt
							}
							window.location = this.constructUrl(this.printPath + this.getNarrowSearchURLArgs(this.lastSortType,this.lastSortTypeHtmlId));
						},

						exportSearchResults : function() {
							if (this.exportPath == undefined || this.exportPath == null || this.exportPath == '') {
								this.missingInitParamAlert('export_path');
								return; // Impt
							}

							window.location = this.constructUrl(this.exportPath + this.getNarrowSearchURLArgs(this.lastSortType,this.lastSortTypeHtmlId));
						},
//#custom_attribute_0078975						
						downloadSampleUserupload : function() {
							window.location = this.constructUrl("administration/userfeed/download/csv");
                        },

						addItem : function() {
							this.abstractFunctionAlert('addItem');
							return false;
						},

						editItem : function() {
							this.abstractFunctionAlert('editItem');
							return false;
						},

						showHide : function(strOne, strTwo) {
							$('#' + strTwo).toggle();
							var classShowHide = $('#' + strOne).hasClass(
									'cls-show');
							if (classShowHide) {
								$('#' + strOne).removeClass('cls-show');
								$('#' + strOne).addClass('cls-hide');
							} else {
								$('#' + strOne).removeClass('cls-hide');
								$('#' + strOne).addClass('cls-show');
							}
						},

						showHideCourseClass : function(type){
							if (type == 'Class') {
								$('#catalogcoursestatus_container').hide();
								$('#courselangtype_container').hide();
								$('#catalogcoursetag_container').hide();
								$('#catalogcoursemanageby_container').hide();
								$('#classhiddenmanageby_container').hide();
								$('#catalogcoursecurrency_container').hide();
								$('#coursePrice_container').hide();

								$('#catalogclassstatus_container').show();
								$('#classdeliverytype_container').show();
								$('#classPrice_container').show();
								$('#classdaterange_container').show();
								$('#classlangtype_container').show();
								$('#catalogclasstag_container').show();
								/*$('.tagscloud-scrollable:not(.jspScrollable)').each(function () {
									if ($(this).is(':visible')) {
									  var tagsHeight = $(this).height();
									  if (tagsHeight > 175) {
									    $(this).height(175);
										$(this).css('max-height', '175px');
									    $(this).jScrollPane({});
								      }
									}
								});*/

								$('#classLocation_container').show();
								$('#classInstructor_container').show();

								$('#classSurvey_container').show();
								$('#classAssessment_container').show();
								$('#classContent_container').show();
								$('#catalogclassmanageby_container').show();
								$('#catalogclasscurrency_container').show();
								$('#tagentityType').val('tagtipclass');

							} else {
								$('#catalogcoursestatus_container').show();
								$('#courselangtype_container').show();
								$('#catalogcoursetag_container').show();
								$('#catalogcoursemanageby_container').show();
								$('#catalogcoursecurrency_container').show();
								$('#coursePrice_container').show();

								$('#catalogclassstatus_container').hide();
								$('#classdeliverytype_container').hide();
								$('#classPrice_container').hide();
								$('#classdaterange_container').hide();
								$('#classlangtype_container').hide();
								$('#catalogclasstag_container').hide();
								$('#classLocation_container').hide();
								$('#classInstructor_container').hide();
								$('#catalogclassmanageby_container').hide();
								$('#catalogclasscurrency_container').hide();

								$('#classSurvey_container').hide();
								$('#classAssessment_container').hide();
								$('#classContent_container').hide();
								$('#tagentityType').val('tagtip');
							}
						},

						renderSearchResults : function(curModule,subModule) {
						  //console.log (curModule);
						  //console.log (subModule);
							var curObj = this;
							if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
								var gridWidth 		= 720;
								var detailsWidth 	= 520;
								var actionWidth 	= 150;
								var iconsWidth 		= 74;
							}else{
								var detailsWidth 	= 595;
								var actionWidth 	= 130;
								var gridWidth 		= 770;
								var iconsWidth 		= 54;
							}
							//Condition for commerce module showindg search widget in JQgrid table format
							if(curModule == 'administration/learning' || subModule == 'administration/manage/content' || subModule == 'administration/contentauthor/video' || subModule == 'administration/contentauthor/presentation' ||  subModule == 'administration/manage/banner'  || subModule == 'administration/sitesetup/moduleinfo'
								|| subModule == 'administration/commerce/order' || subModule == 'administration/manage/announcement') {
								var tmp_html=$('#narrow-search-results-holder').html();
								if(tmp_html=="<TBODY></TBODY>" || tmp_html=="<tbody></tbody>"){
									$('#narrow-search-results-holder').html("");
								}
								if($('#narrow-search-results-holder').html().length <= 0) {
									//("renderSearchResults");
									curObj.createLoader("main-wrapper");
								
									var curObjStr = '$("#' + curObj.widgetInitId + '").data("' + curObj.widgetName + '")';
									//admininister page jqgrid declaration here
									$("#narrow-search-results-holder").jqGrid( {
										url : curObj.constructUrl(curObj.searchBasePath),
										datatype : "json",
										mtype : 'GET',
										colNames : [ 'Icons','Details', 'Action' ],
										//colNames : [ 'Details' ],
										colModel : [ {
											name : 'Icons',
											index : 'Icons',
											classes:'rowpaddTB',
											title : false,
											width : iconsWidth,
											'widgetObj' : curObjStr,
											formatter : curObj.paintSearchResultsIcons
										},  {
											name : 'Details',
											index : 'Details',
											classes:'rowpaddTB',
											title : false,
											width : detailsWidth ,
											'widgetObj' : curObjStr,
											formatter : curObj.paintSearchResults
										}, {
											name : 'Action',
											index : 'Action',
											classes:'rowpaddTB',
											title : false,
											width : actionWidth,
											'widgetObj' : curObjStr,
											formatter : curObj.paintSearchResultsAction
										} ],
										rowNum : 10,
										rowList : [ 10, 25, 50 ],
										pager : '#narrow-search-results-pager',
										viewrecords : true,
										emptyrecords : "",
										sortorder : "desc",
										toppager : true,
										height : 'auto',
										width : gridWidth,
										loadtext : "",
										recordtext : "",
										pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
										loadui : false,
										userdata: "userdata",
										loadComplete : curObj.loadComplete
									});
									/* .navGrid('#narrow-search-results-pager', {
										add : false,
										edit : false,
										del : false,
										search : false,
										refreshtitle : true
									}); */
									// curObj.hidePageControls(true); // show in loadComplete()
								}else{
								curObj.refreshGrid();
								if($('#narrow-search-actionbar')){
								  $('#narrow-search-actionbar').show();
								}
							}
						} else if((curModule == 'administration/people' && subModule != 'administration/people/organization'&&
						            subModule != 'administration/people/group')  ||
						               subModule == 'administration/commerce/setting'  ||  subModule == 'administration/people/setup' ||  subModule == 'administration/manage/announcement' ||  subModule == 'administration/sitesetup/config') {
              				var iconsWidth = 38;
              				if (subModule == 'administration/commerce/setting' || subModule == 'administration/people/setup' || subModule == 'administration/sitesetup/config') {
                				iconsWidth = 80;
                				detailsWidth = 440;
              				}
              			
							var tmp_html=$('#narrow-search-results-holder').html();
							if(tmp_html=="<TBODY></TBODY>" || tmp_html=="<tbody></tbody>"){
								$('#narrow-search-results-holder').html("");
							}
							if($('#narrow-search-results-holder').html().length <= 0) {
								//("renderSearchResults");
								curObj.createLoader("main-wrapper");
								var curObjStr = '$("#' + curObj.widgetInitId + '").data("' + curObj.widgetName + '")';

								if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
									var iconsWidth 		= 50;
								}
								$("#narrow-search-results-holder").jqGrid( {
									url : curObj.constructUrl(curObj.searchBasePath),
									datatype : "json",
									mtype : 'GET',
									colNames : [ 'Icons','Details'],
									//colNames : [ 'Details' ],
									colModel : [ {
										name : 'Icons',
										index : 'Icons',
										classes:'rowpaddTB',
										title : false,
										width : iconsWidth,
										'widgetObj' : curObjStr,
										formatter : curObj.paintSearchResultsIcons
									},  {
										name : 'Details',
										index : 'Details',
										classes:'rowpaddTB',
										title : false,
										width : detailsWidth,
										'widgetObj' : curObjStr,
										formatter : curObj.paintSearchResults
									}],
									rowNum : 10,
									rowList : [ 10, 25, 50 ],
									pager : '#narrow-search-results-pager',
									viewrecords : true,
									emptyrecords : "",
									sortorder : "desc",
									toppager : true,
									height : 'auto',
									width : gridWidth,
									loadtext : "",
									recordtext : "",
									pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
									loadui : false,
									loadComplete : curObj.loadComplete
								});
								/* .navGrid('#narrow-search-results-pager', {
									add : false,
									edit : false,
									del : false,
									search : false,
									refreshtitle : true
								}); */
								// curObj.hidePageControls(true); // show in loadComplete()
							}else{
								curObj.refreshGrid();
								if($('#narrow-search-actionbar')){
								  $('#narrow-search-actionbar').show();
								}
							}
						}
//#custom_attribute_0078975
						else if(curModule == 'administration/survey' || subModule == 'administration/survey/surveydetails' || 
								subModule == 'administration/assessment/assessmentdetails' || subModule == 'administration/assessment/assessmentquestions' || 
								subModule == 'administration/survey/surveyquestions' || subModule == 'administration/contentauthor/video' 
									||subModule == 'administration/contentauthor/presentation' 
										||subModule == 'administration/manage/location'
										|| subModule == 'administration/manage/customattribute' || subModule == 'administration/manage/notification_template' || subModule == 'administration/manage/certificate'
							  || subModule == 'administration/commerce/tax' || subModule == 'administration/commerce/discounts'){
							var tmp_html=$('#narrow-search-results-holder').html();
							if(tmp_html=="<TBODY></TBODY>" || tmp_html=="<tbody></tbody>"){
								$('#narrow-search-results-holder').html("");
							}

							if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
								var detailsWidth 	= 600;
								var actionWidth 	= 150;
							}else{
								var detailsWidth 	= 655;
								var actionWidth 	= 130;
							}
							if($('#narrow-search-results-holder').html().length <= 0) {
								//("renderSearchResults");
								curObj.createLoader("main-wrapper");
								var curObjStr = '$("#' + curObj.widgetInitId + '").data("' + curObj.widgetName + '")';

								$("#narrow-search-results-holder").jqGrid( {
									url : curObj.constructUrl(curObj.searchBasePath),
									datatype : "json",
									mtype : 'GET',
									colNames : [ 'Details', 'Action' ],
									colModel : [ {
										name : 'Details',
										index : 'Details',
										classes:'rowpaddTB',
										title : false,
										width : detailsWidth,
										'widgetObj' : curObjStr,
										formatter : curObj.paintSearchResults
									}, {
										name : 'Action',
										index : 'Action',
										classes:'rowpaddTB',
										title : false,
										width : actionWidth,
										'widgetObj' : curObjStr,
										formatter : curObj.paintSearchResultsAction
									}],
									rowNum : 10,
									rowList : [ 10, 25, 50 ],
									pager : '#narrow-search-results-pager',
									viewrecords : true,
									emptyrecords : "",
									sortorder : "desc",
									toppager : true,
									height : 'auto',
									width : gridWidth,
									loadtext : "",
									recordtext : "",
									pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
									loadui : false,
									loadComplete : curObj.loadComplete
								});
								/* .navGrid('#narrow-search-results-pager', {
									add : false,
									edit : false,
									del : false,
									search : false,
									refreshtitle : true
								});
								curObj.hidePageControls(true); // show in loadComplete() */
							}else{
								curObj.refreshGrid();
								if($('#narrow-search-actionbar')){
								  $('#narrow-search-actionbar').show();
								}
							}
						}
						else {
							var tmp_html=$('#narrow-search-results-holder').html();
							if(tmp_html=="<TBODY></TBODY>" || tmp_html=="<tbody></tbody>"){
								$('#narrow-search-results-holder').html("");
							}
							if($('#narrow-search-results-holder').html().length <= 0) {
								//("renderSearchResults");
								curObj.createLoader("main-wrapper");
								var curObjStr = '$("#' + curObj.widgetInitId + '").data("' + curObj.widgetName + '")';

								$("#narrow-search-results-holder").jqGrid( {
									url : curObj.constructUrl(curObj.searchBasePath),
									datatype : "json",
									mtype : 'GET',
									//colNames : [ 'Details', 'Action' ],
									colNames : [ 'Details' ],
									colModel : [ {
										name : 'Details',
										index : 'Details',
										classes:'rowpaddTB',
										title : false,
										width : detailsWidth,
										'widgetObj' : curObjStr,
										formatter : curObj.paintSearchResults
									}/*, {
										name : 'Action',
										index : 'Action',
										title : false,
										width : 130,
										'widgetObj' : curObjStr,
										formatter : curObj.paintSearchResultsAction
									}*/ ],
									rowNum : 10,
									rowList : [ 10, 25, 50 ],
									pager : '#narrow-search-results-pager',
									viewrecords : true,
									emptyrecords : "",
									sortorder : "desc",
									toppager : true,
									height : 'auto',
									width : gridWidth,
									loadtext : "",
									recordtext : "",
									pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
									loadui : false,
									loadComplete : curObj.loadComplete
								});
								/* .navGrid('#narrow-search-results-pager', {
									add : false,
									edit : false,
									del : false,
									search : false,
									refreshtitle : true
								});
								curObj.hidePageControls(true); // show in loadComplete() */
							}else{
								curObj.refreshGrid();
								if($('#narrow-search-actionbar')){
								  $('#narrow-search-actionbar').show();
								}
							}
						}
					},

						paintSearchResults : function(cellvalue, options,rowObject) {
							return rowObject['details'];
						},

						paintSearchResultsAction : function(cellvalue, options,rowObject) {
							return rowObject['action'];
						},
						paintSearchResultsIcons : function(cellvalue, options,rowObject) {
							return rowObject['image'];
						},

						/* hidePageControls : function(hideAll) {
						//Ayyappan
						console.log('hide pager heer');
						  var lastDataRow = $('#narrow-search-results-holder tr.ui-widget-content').filter(":last");
						  //console.log(lastDataRow.length);

              // if (hideAll) {
                //$('#narrow-search-results-pager').hide();
                 if(this.currTheme == "expertusoneV2")
    	  			$('#root-admin-results .block-footer-left').show();
      				$('#narrow-search-results-pager').hide();

                //remove bottom dotted border from the last row in result if records are present
                if (lastDataRow.length != 0) {
                  //console.log('hidePageControls() : hideAll - last data row found');
                  lastDataRow.children('td').css('border', '0 none');
                }
              }
              else {
              	$('#narrow-search-results-pager').show();
                //console.log('hidePageControls() : hide only next/prev page control');
                if(this.currTheme == "expertusoneV2")
                $('#root-admin-results .block-footer-left').hide();
                $('#narrow-search-results-pager #narrow-search-results-pager_center #first_narrow-search-results-pager').hide();
                $('#narrow-search-results-pager #narrow-search-results-pager_center #last_narrow-search-results-pager').hide();
                $('#narrow-search-results-pager #narrow-search-results-pager_center #next_narrow-search-results-pager').hide();
                $('#narrow-search-results-pager #narrow-search-results-pager_center #prev_narrow-search-results-pager').hide();
                $('#narrow-search-results-pager #narrow-search-results-pager_center #sp_1_narrow-search-results-pager').parent().hide();
                //if (lastDataRow.length != 0) {
                  //add bottom dotted border from the last row in result if records are present
                  //lastDataRow.children('td').css('border-bottom', '1px dotted #CCCCCC');
                //}
              }
						}, */

	showPageControls : function() {
	  //console.log('showPageControls() : show all control');
	  $('#narrow-search-results-pager').show();
	  if(this.currTheme == "expertusoneV2")
	  $('#root-admin-results .block-footer-left').hide();
	  $('#narrow-search-results-pager #narrow-search-results-pager_center #first_narrow-search-results-pager').show();
	  $('#narrow-search-results-pager #narrow-search-results-pager_center #last_narrow-search-results-pager').show();
      $('#narrow-search-results-pager #narrow-search-results-pager_center #next_narrow-search-results-pager').show();
      $('#narrow-search-results-pager #narrow-search-results-pager_center #prev_narrow-search-results-pager').show();
      $('#narrow-search-results-pager #narrow-search-results-pager_center #sp_1_narrow-search-results-pager').parent().show();
             // issue In Pagination Width
      if(this.currTheme == "expertusone")
    	  $('#narrow-search-results-pager #narrow-search-results-pager_center').removeAttr("style");
      //add bottom dotted border from the last row in result if records are present
      //var lastDataRow = $('#narrow-search-results-holder tr.ui-widget-content').filter(":last");
      //if (lastDataRow.length != 0) {
        //lastDataRow.children('td').css('border-bottom', '1px dotted #CCCCCC');
      //}
				},

						loadComplete : function(response, postdata, formid, updateShowMore) {
							var curObj=this;
							curObj.widgetInitId="root-admin";
							curObj.widgetName="narrowsearch";

							Drupal.attachBehaviors(); // for initializing ctools.
							$('#narrow-search-results-holder').show();
							//alert("Load Complete");
							if($('#narrow-search-actionbar')){
							  $('#narrow-search-actionbar').show();
							}
							//38455: Managed by filters me is not checked when login as multilanguage
							// Managed By behavior
							var userData = $('#narrow-search-results-holder').jqGrid('getGridParam', 'userData');
				              if (typeof userData.managedBy != 'undefined' && userData.managedBy != '') {
				                var managedByCheckboxId = '#managed_by_1 input';	//change by ayyappans for 0040223: See more link is not working in narrow filter in user grid page
				                if (typeof userData.adminPage != 'undefined' && userData.adminPage == 'class') {
				                  managedByCheckboxId = '#managed_by_1 input';
				                }

				                if (typeof userData.adminPage != 'undefined' && userData.adminPage == 'class') {
				                  $('#checkbox_clspgopened').attr('checked', 'checked');
				                  $('#checkbox_clspgopened').parent().removeClass('narrow-search-filterset-checkbox-unselected');
				                  $('#checkbox_clspgopened').parent().addClass('narrow-search-filterset-checkbox-selected');
				                }

				                if (userData.managedBy == 'me') {
				                  $(managedByCheckboxId).attr('checked', 'checked');
				                  $(managedByCheckboxId).parent().removeClass('narrow-search-filterset-checkbox-unselected');
				                  $(managedByCheckboxId).parent().addClass('narrow-search-filterset-checkbox-selected');
				                  $('#managed_by_1').children('.narrow-search-filterset-item-label-unselected').removeClass('narrow-search-filterset-item-label narrow-search-filterset-item-label-unselected').addClass('narrow-search-filterset-item-label narrow-search-filterset-item-label-selected');
				                }
				              }

							var numRecords = parseInt($("#narrow-search-results-holder").getGridParam("records"), 10);
							//console.log('loadComplete() : numRecords = ' + numRecords);
							if (numRecords == 0) {
								$('#narrow-search-no-records').show();
							} else {
								$('#narrow-search-no-records').hide();
							}

							// Show pagination only when search results span multiple pages
			              var recordsPerPage = parseInt($("#narrow-search-results-holder").getGridParam("reccount"), 10);
			              //console.log('loadComplete() : recordsPerPage = ' + recordsPerPage);

			              var hideAllPageControls = true;
			              if (numRecords > 10) { // 10 is the least view per page option.
			                hideAllPageControls = false;
			                //console.log('loadComplete() : hideAllPageControls set to false');
			                $("#page-container #narrow-search-results-holder tr.ui-widget-content:last-child > td.rowpaddTB").css("padding-bottom","9px");
			              }

			              /* if (numRecords <= recordsPerPage) {
			               //console.log('loadComplete() : numRecords <= recordsPerPage : hide pagination controls');
			                $('#' + curObj.widgetInitId).data(curObj.widgetName).hidePageControls(hideAllPageControls);
			              }
			              else {
			                //console.log('loadComplete() : numRecords <= recordsPerPage : show pagination controls including view per page control');
			                $('#' + curObj.widgetInitId).data(curObj.widgetName).showPageControls();
			              } */
							//$('#' + curObj.widgetInitId).data(curObj.widgetName).initGrid('#' + curObj.widgetInitId,curObj.widgetName);
							if ($('#' + curObj.widgetInitId).data(curObj.widgetName).firstTime) {
								$("#narrow-search-filters").show();
								$("#narrow-search-results-topbar").show();
								$("#" + response.initial_sort_type_html_id).addClass('sortype-high-lighter');
								$('#' + curObj.widgetInitId).data(curObj.widgetName).firstTime = false;
								$('#' + curObj.widgetInitId).data(curObj.widgetName).initGrid('#' + curObj.widgetInitId,curObj.widgetName);
							}
							else{
								if($('#' + curObj.widgetInitId).data(curObj.widgetName).lastSortTypeHtmlId)
								    $("#" + $('#' + curObj.widgetInitId).data(curObj.widgetName).lastSortTypeHtmlId).addClass('sortype-high-lighter');
								else
									$("#" + response.initial_sort_type_html_id).addClass('sortype-high-lighter');
							}
							$('#' + curObj.widgetInitId).data(curObj.widgetName).destroyLoader('paint-narrow-search-results');
							$('#' + curObj.widgetInitId).data(curObj.widgetName).destroyLoader('main-wrapper');
								//Vtip-Display toolt tip in mouse over
//							$('.fade-out-title-container').each(function() {
//								//console.log($(this).width());
//								//console.log($(this).find('.title-lengthy-text').width());
//								if($(this).width() >= $(this).find('.title-lengthy-text').width()) {
//									$(this).find('.fade-out-image').remove();
//								}
//								//title-lengthy-text
//							});
								 vtip();
								 resetFadeOutByClass('#narrow-search-results-holder','narrow-search-results-item-detail','vtip','admin'); /*Viswanathan added for #76811*/
							$('.limit-title').trunk8(trunk8.admin_title);
					 		$('.limit-desc').trunk8(trunk8.admin_desc);
						//	Drupal.attachBehaviors();
							// if ($("#paint-narrow-search-results").data('showmore') === undefined) {
							updateShowMore = ((updateShowMore == undefined || updateShowMore == null) ? true : updateShowMore);

							if(updateShowMore) {
								$("#paint-narrow-search-results").showmore({
									showAlways: true,
									'grid': '#narrow-search-results-holder',
									'gridWrapper': '#main-wrapper',
									'showMore': '#admin-narrow-search-show-more'
								});
							}
								//bind onclick event for the ajax edit narrow search result icon
								$('#narrow-search-results-holder tr.jqgrow:not(.click-binded)').bind('mousedown', function (e) {
									try {
										// console.log('click happens');
										$('#narrow-search-results-holder').setGridParam({
											'lastAccessedRow': $(e.target).closest("tr.jqgrow")[0]
										});
									} catch (e) {
										// console.log(e, e.stack);
									}
								})
								.addClass('click-binded');
							// }
							// console.log($("#paint-narrow-search-results"), $("#paint-narrow-search-results").data('showmore'));
						},

						getNarrowSearchURLArgs : function(sortType,sortTypeHtmlId) {

							var searchNarrowParams = '';
							var curObj=this;
							var numfilterSets = curObj.filterSets.length;

							for ( var i = 0; i < numfilterSets; i++) {
								searchNarrowParams += '&' + curObj.filterSets[i].code + '=';
								var appliedFilters = '';
								switch (curObj.filterSets[i].type) {
								case 'radio':
									appliedFilters += $('input[name="'+curObj.filterSets[i].code+'"]:checked').val();
									break;

								case 'checkbox':
									$('#' + curObj.filterSets[i].code + '_filterset .narrow-search-filterset-checkbox').each(function() {
														if ($(this).is(':checked')) {
															appliedFilters += (appliedFilters == '') ? '': '|';
															appliedFilters += $(this).val();
														}
									});
									break;

								case 'addltext':
									var tmpDefaultText;
									var curText = $('#' + curObj.filterSets[i].code + '-addltext-filter').val();
									if (curText == curObj.filterSets[i].defaultText || curText == undefined || curText === null || curText == "") {
										tmpDefaultText = '';
									} else {
										tmpDefaultText = curText;
									}
									appliedFilters += encodeURIComponent(tmpDefaultText);
									break;

								case 'tagscloud':
									var curText = $('#' + curObj.filterSets[i].code + '-addltext-filter').val();
									if(curText == undefined || curText === null || curText == ""){
										curText = '';
									}
									appliedFilters += encodeURIComponent(curText);
									break;

								case 'slider':
									var prefix = $('#'+curObj.filterSets[i].code+'_filterset').data("prefix");
									var suffix = $('#'+curObj.filterSets[i].code+'_filterset').data("suffix");
									var tempprice;
									var Lprice = '';
									var Rprice = '';

									if($( '#value-slide-left-'+curObj.filterSets[i].code+'' ).val() != undefined){
										Lprice = $( '#value-slide-left-'+curObj.filterSets[i].code+'' ).val().replace(prefix+suffix,'');
									}
									if($( '#value-slide-right-'+curObj.filterSets[i].code+'' ).val() != undefined){
										Rprice = $( '#value-slide-right-'+curObj.filterSets[i].code+'' ).val().replace(prefix+suffix,'');
									}
									
									if((Lprice == undefined || Lprice === null || Lprice == "") || (Rprice == undefined || Rprice === null || Rprice == "")) {
										tempprice = '';
									}
									else {
										tempprice = $.trim(Lprice+'|'+Rprice);
									}
//									if(tempprice == 'undefined-undefined' || tempprice == undefined-undefined || tempprice =='|' || tempprice == undefined || tempprice === null || tempprice == "")
//										tempprice = '';

									appliedFilters += tempprice;
									break;

								case 'daterange':
									var clrLinkSelector = '#' + curObj.filterSets[i].code + '-daterange-clr';

									var fromDate = $('#' + curObj.filterSets[i].code + '-daterange-from-date').val();
									var toDate = $('#' + curObj.filterSets[i].code + '-daterange-to-date').val();

									if (fromDate == curObj.filterSets[i].from_default_text || fromDate == undefined || fromDate === null || fromDate == "") {
										fromDate = '';
									} else {
										$(clrLinkSelector).css('display','block');
									}

									if (toDate == curObj.filterSets[i].to_default_text || toDate == undefined || toDate === null || toDate == "") {
										toDate = '';
									} else {
										$(clrLinkSelector).css('display','block');
									}
									appliedFilters += fromDate + '|' + toDate;
									break;
								} // end switch

								searchNarrowParams += appliedFilters;
							} // end for loop

							// Append sortby to the search string
							searchNarrowParams += '&sortby=' + sortType;

							// Append text substring from Search By Text filter
							var textFilter = ($('#narrow-search-text-filter').val() == curObj.topTextFilterLabel) ? '' : $('#narrow-search-text-filter').val();
							 //44552: search not working in survey and assessment page.
							searchNarrowParams += '&textfilter=' + encodeURIComponent(textFilter);

							return searchNarrowParams;
						},

						refreshGrid : function(sortType, sortTypeHtmlId) {
							if($('#narrow-search-actionbar')){
							  //$('#narrow-search-actionbar').hide();
							}
							var curObj=this;

							if (sortType == undefined || sortType == null || sortType == '') {
								sortType = curObj.lastSortType;
							}
							if (sortTypeHtmlId == undefined || sortTypeHtmlId == null || sortType == '') {
								sortTypeHtmlId = curObj.lastSortTypeHtmlId;
							}
							$('#gview_narrow-search-results-holder').css('min-height', '100px');
                         //   if(curObj.searchBasePath == "administration/learning/catalog/search/all/")
                            curObj.createLoader('main-wrapper');
                         //   else
							//curObj.createLoader('paint-narrow-search-results');

							var ajaxUrl = curObj.constructUrl(curObj.searchBasePath + curObj.getNarrowSearchURLArgs(sortType,sortTypeHtmlId));
							//console.log('ajaxUrl', ajaxUrl);
							if(ajaxUrl.indexOf('catalogtype=Class') !== -1) {
								$('#narrow-search-results-holder').setGridParam({jsonReader: {id: "jqgrid-rowid"}});;
							} else {
								$('#narrow-search-results-holder').setGridParam({jsonReader: {id: "id"}});;
							}
							$('#narrow-search-results-holder').setGridParam({url : ajaxUrl});

							$("#narrow-search-results-holder").trigger("reloadGrid", [ {page : 1} ]);

							curObj.updateCheckboxFiltersDisplay();

							// Highlight correct sort type
							$('.narrow-search-sortbar-sortlinks li').each(function() {
										$(this).find('a').removeClass('sortype-high-lighter');
							});

							if (sortTypeHtmlId.length > 0) {
								$('#' + sortTypeHtmlId).addClass('sortype-high-lighter');
								curObj.lastSortType = sortType;
								curObj.lastSortTypeHtmlId = sortTypeHtmlId;
							}
							vtip();
						},

						updateCheckboxFiltersDisplay : function() {
							$('.narrow-search-filtersets-holder').find('input[type=checkbox]').each(function() {
												if ($(this).is(':checked')) {
													$(this).parent().next('label').removeClass('narrow-search-filterset-item-label-unselected');
													$(this).parent().next('label').addClass('narrow-search-filterset-item-label-selected');
													$(this).parent().removeClass('narrow-search-filterset-checkbox-unselected');
													$(this).parent().addClass('narrow-search-filterset-checkbox-selected'); // The checkbox itself is animage
												} else {
													$(this).parent().next('label').removeClass('narrow-search-filterset-item-label-selected');
													$(this).parent().next('label').addClass('narrow-search-filterset-item-label-unselected');
													$(this).parent().removeClass('narrow-search-filterset-checkbox-selected');
													$(this).parent().addClass('narrow-search-filterset-checkbox-unselected'); // The checkbox itself is an image
												}
											});

							$('.narrow-search-filtersets-holder').find('input[type=radio]').each(function() {
								if ($(this).is(':checked')) {
									$(this).parent().next('label').removeClass('narrow-search-filterset-item-label-unselected');
									$(this).parent().next('label').addClass('narrow-search-filterset-item-label-selected');
									$(this).parent().removeClass('narrow-search-filterset-radio-unselected');
									$(this).parent().addClass('narrow-search-filterset-radio-selected'); // The radio itself is animage
									// the data is undefined,added this condition.							
									if($(".classlangtype-scroll-list-container").jScrollPane({}).data('jsp') != undefined)
									{
									$(".classlangtype-scroll-list-container").jScrollPane({}).data('jsp').destroy();
									}
									$(".classlangtype-scroll-list-container").jScrollPane();	//Apply scroll
									if($(".classcurrency-scroll-list-container").jScrollPane({}).data('jsp') != undefined)
									{
									$(".classcurrency-scroll-list-container").jScrollPane({}).data('jsp').destroy();
									}
									$(".classcurrency-scroll-list-container").jScrollPane();	//Apply scroll	
								} else {
									$(this).parent().next('label').removeClass('narrow-search-filterset-item-label-selected');
									$(this).parent().next('label').addClass('narrow-search-filterset-item-label-unselected');
									$(this).parent().removeClass('narrow-search-filterset-radio-selected');
									$(this).parent().addClass('narrow-search-filterset-radio-unselected'); // The checkbox itself is an image

								}
							})
							vtip();
						},

						initGrid : function(widgetSelector, widgetName) {
							$('#first_narrow-search-results-pager').click(function(e) {
												if (!$('#first_narrow-search-results-pager').hasClass('ui-state-disabled')) {
													$('#narrow-search-results-holder').hide();
													$('#gview_narrow-search-results-holder').css('min-height','100px');
													$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
												}
											});
							$('#prev_narrow-search-results-pager').click(function(e) {
												if (!$('#prev_narrow-search-results-pager').hasClass('ui-state-disabled')) {
													$('#narrow-search-results-holder').hide();
													$('#gview_narrow-search-results-holder').css('min-height','100px');
													$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
												}
											});

							$('#next_narrow-search-results-pager').click(function(e) {
												if (!$('#next_narrow-search-results-pager').hasClass('ui-state-disabled')) {
													$('#narrow-search-results-holder').hide();
													$('#gview_narrow-search-results-holder').css('min-height','100px');
													$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
												}
											});
							$('#last_narrow-search-results-pager').click(function(e) {
												if (!$('#last_narrow-search-results-pager').hasClass('ui-state-disabled')) {
													$('#narrow-search-results-holder').hide();
													$('#gview_narrow-search-results-holder').css('min-height','100px');
													$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
												}
											});

							$('.ui-pg-selbox').bind('change',function() {
												$('#narrow-search-results-holder').hide();
												$('#gview_narrow-search-results-holder').css('min-height','100px');
												$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
												$(widgetSelector).data(widgetName).hidePageControls(false);
												this.blur();
											});

							$(".ui-pg-input").keyup(function(event) {
												if (event.keyCode == 13) {
													$('#narrow-search-results-holder').hide();
													$('#gview_narrow-search-results-holder').css('min-height','100px');
													$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
												}
											});

							if(this.currTheme == "expertusoneV2"){
								$('#narrow-search-results-pager .page-show-prev').bind('click',function() {
									if(parseInt($("#narrow-search-results-pager .page_count_view").attr('id')) < 0){
										$("#narrow-search-results-pager .page_count_view").attr('id','0');
									}else{
										$('#narrow-search-results-holder').hide();
										$('#gview_narrow-search-results-holder').css('min-height','100px');
										$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
										$(widgetSelector).data(widgetName).hidePageControls(false);
									}
								});

								$('#narrow-search-results-pager .page-show-next').bind('click',function() {
									if(parseInt($("#narrow-search-results-pager .page_count_view").attr('id')) >= parseInt($("#narrow-search-results-pager .page-total-view").attr('id'))){
										$("#narrow-search-results-pager .page_count_view").attr('id',($("#narrow-search-results-pager .page_count_view").attr('id')-1));
									}else{
										$('#narrow-search-results-holder').hide();
										$('#gview_narrow-search-results-holder').css('min-height','100px');
										$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
										$(widgetSelector).data(widgetName).hidePageControls(false);
									}
								});

							}
							vtip();
						},

						initTextFilter : function(textFilterACPath) {

							if ($('#narrow-search-text-filter')) {
								var obj = this;
								// Destroy previous autocomplete setup on this field
								$('#narrow-search-text-filter').removeData();

								$("#narrow-search-text-filter").keyup(function(event) {
											if (event.keyCode == 13) {
												obj.narrowSearchByText();
												$('.ac_results').css('display', 'none');
											}
								});

								$('#narrow-search-text-filter-go').click(function() {
									        /*this.currTheme = Drupal.settings.ajaxPageState.theme;*/
											obj.narrowSearchByText();
											 /*var selector='#narrow-search-text-filter';
                                             $(selector).addClass('header-search-text-filter');
                                             $(selector).removeClass('narrow-search-filterset-daterange-nonempty');
                                             $(selector).addClass('narrow-search-filterset-daterange-empty');
                                             if(this.currTheme == 'expertusoneV2'){
                                            	 $(selector).val(Drupal.t('LBL304'));
                                             }else{
                                            	 $(selector).val((Drupal.t('LBL304')).toUpperCase());
                                             }*/

								});

								if (textFilterACPath != undefined && textFilterACPath != 'undefined' && textFilterACPath != null && textFilterACPath != '') {
									textFilterACPath = "/?q="+ textFilterACPath;
									$('#narrow-search-text-filter').autocomplete(textFilterACPath, {
												minChars : 3,
												max : 50,
												autoFill : true,
												mustMatch : false,
												matchContains : false,
												inputClass : "ac_input",
												loadingClass : "ac_loading"
											});
								}
							}

							this.initTextDateFilterBlurStyle("#narrow-search-text-filter", obj.topTextFilterLabel,"1");
							$("#narrow-search-text-filter").focus(function () {
								try {
									var inputElem = $(this);
									inputElem.val() == '' || addClearIcon(inputElem);
								} catch(e) {
									// window.console.log(e, e.stack);
								}
							})
							.blur(function(e) {
								try {
									var inputElem = $(this);
									removeClearIcon(inputElem)
								} catch(e) {
									// window.console.log(e, e.stack);
								}
							}).keyup(function () {
								var inputElem = $(this);
								if(inputElem.siblings('.eol-search-clearance').length < 1) {
									inputElem.val() == '' || addClearIcon(inputElem);
								} else {
									inputElem.val() == '' && removeClearIcon(inputElem);
								}
							});
							vtip();						
						},


						initFilters : function() {

							// init each filter as appropriate
							for ( var i = 0; i < this.filterSets.length; i++) {
								var filterSet = this.filterSets[i];
								 //alert(filterSet.toSource());
								switch (filterSet.type) {
								case 'addltext':
									this.initAddlTextFilter(filterSet);
									break;

//								case 'tagscloud':
//									if ($('#' + filterSet.code + '_tagscloud').is(':visible')) {
//								      var tagsHeight = $('#' + filterSet.code + '_tagscloud').height();
//								      if (tagsHeight > 175) {
//								        $('#' + filterSet.code + '_tagscloud').height(175);
//									    $('#' + filterSet.code + '_tagscloud').css('max-height', '175px');
//										$('#' + filterSet.code + '_tagscloud').jScrollPane({});
//									  }
//	                                }
//									break;

								case 'daterange':
									this.initDateRangeFilter(filterSet);
									break;

								case 'slider' :
									this.initSlider(filterSet);
									break;

								case 'checkbox':
									if (filterSet.code =='group') {
										if ($('#' + filterSet.code + '_filterset').is(':visible')) {
											($('#' + filterSet.code + '_filterset').removeClass('display-more'));
											var groupHeight = $('#' + filterSet.code + '_filterset').height();
										      if (groupHeight > 10) {
											    $('#' + filterSet.code + '_filterset').css({'width':'160px'});
											    //$('#' + filterSet.code + '_filterset').css({'height':'100px','max-height', '100px'});
											    //$('.jspContainer').css('width','160px');
												//$('#' + filterSet.code + '_filterset').jScrollPane({});
										}
									}
								  }
								} // end switch
							} // end for
							//Removing last filter type border
							var filterCount = $('.narrow-search-filterset').filter(':visible').length-1;
							$('.narrow-search-filterset:eq('+filterCount+')').css({'border-bottom':'none','padding-bottom':'0px'});
						},


					    initTextDateFilterBlurStyle : function(selector,defaultText,fromTopFilterTextSearch) {

							var data = selector + '&' + defaultText + '&' + fromTopFilterTextSearch;
							var curObj=this;

							$(selector).blur(data,function(event) { // Can pass a string data only. but not an object data as objects are passed by reference
												var data = event.data;
												var tokens = data.split("&");
												var selector = tokens[0];
												var fieldValue = $(selector).val();
												var defaultText = tokens[1];
												var fromTopFilterTextSearch = tokens[2];

												if(fromTopFilterTextSearch=="1"){
													defaultText=curObj.topTextFilterLabel;
												}

												if (fieldValue == '' || fieldValue == defaultText) {
													$(selector).removeClass('narrow-search-filterset-daterange-nonempty');
													$(selector).addClass('narrow-search-filterset-daterange-empty');
													$(selector).val(defaultText);
												} else if (fieldValue != defaultText) {
													$(selector).removeClass('narrow-search-filterset-daterange-empty');
													$(selector).addClass('narrow-search-filterset-daterange-nonempty');
												}

											});

							$(selector).focus(data,function(event) { // Can pass a string data only.but not an object data as objects are passed by  reference
												var data = event.data;
												var tokens = data.split("&");
												var selector = tokens[0];
												var fieldValue = $(selector).val();
												var defaultText = tokens[1];
												//$(selector).val('');
												if (fieldValue == defaultText) {
													$(selector).val('');
												}
												$(selector).removeClass('narrow-search-filterset-daterange-empty');
												$(selector).addClass('narrow-search-filterset-daterange-nonempty');

											});

							$(selector).change(data,function(event) { // Can pass a string data only.but not an object data as objects are passed by reference
												var data = event.data;
												var tokens = data.split("&");
												var selector = tokens[0];
												var fieldValue = $(selector).val();
												var defaultText = tokens[1];
												var fromTopFilterTextSearch = tokens[2];

												if(fromTopFilterTextSearch=="1"){
													defaultText=curObj.topTextFilterLabel;
												}

												if (fieldValue == '' || fieldValue == defaultText) {
													$(selector).removeClass('narrow-search-filterset-daterange-nonempty');
													$(selector).addClass('narrow-search-filterset-daterange-empty');
													$(selector).val(defaultText);
												} else {
													$(selector).removeClass('narrow-search-filterset-daterange-empty');
													$(selector).addClass('narrow-search-filterset-daterange-nonempty');
												}

											});

							var fieldValue = $(selector).val();

							if (fieldValue == defaultText) {
								$(selector).addClass('header-search-text-filter');
								$(selector).removeClass('narrow-search-filterset-daterange-nonempty');
								$(selector).addClass('narrow-search-filterset-daterange-empty');
								$(selector).val(defaultText);
							}

							vtip();
						},


						initAddlTextFilter : function(filterSet) {
							if ($('#' + filterSet.code + '-addltext-filter')) {

								var obj = this;

								$('#' + filterSet.code + '-addltext-filter').keyup(filterSet.code,function(event) {
													if (event.keyCode == 13) {
														obj.narrowSearchByAddlText(event.data);

														//Display clear link in the arrow search filter
														$('#' + filterSet.code + '-addltext-clr').css('display','block');
														$('.ac_results').css('display', 'none');
													}
								});

								$('#' + filterSet.code + '-addltext-filter-go').click(filterSet.code,function(event) {
									if($("#" + filterSet.code + "-addltext-filter").val() != filterSet.defaultText){
										obj.narrowSearchByAddlText(event.data);
									}
								});

								if (filterSet.acpath != undefined && filterSet.acpath != 'undefined' && filterSet.acpath != null && filterSet.acpath != '') {
									var addlTextFilterACPath = "/?q=" + filterSet.acpath;
									var multipleval = '';
									if(filterSet.code == 'grporg' || filterSet.code == 'grpjobrole' || filterSet.code == 'grploc'|| filterSet.code == 'grpempl' || filterSet.code == 'grpdep' || filterSet.code == 'grpusrtyp' || filterSet.code == 'grpcontry'|| filterSet.code == 'grplang')
										multipleval = true;
									else
										multipleval = false;
									$('#' + filterSet.code + '-addltext-filter') .autocomplete(addlTextFilterACPath,{
														minChars : 3,
														max : 50,
														autoFill : true,
														mustMatch : false,
														matchContains : false,
														inputClass : "ac_input",
														loadingClass : "ac_loading",
														multiple: multipleval

								    });
								} // endif !empty(textFilterACPath)
							} // end filterset exists

							var selector = "#" + filterSet.code + '-addltext-filter';
							var defaultText = filterSet.defaultText;
							this.initTextDateFilterBlurStyle(selector,defaultText);
						},

						initDateRangeFilter : function(filterSet) {
							if ($('#' + filterSet.code + '-daterange-from-date')) {
								filterSet.__init = 'FROM';
								this.initDateRangeDateField(filterSet);
							}
							if ($('#' + filterSet.code + '-daterange-to-date')) {
								filterSet.__init = 'TO';
								this.initDateRangeDateField(filterSet);
							}
						},

						initDateRangeDateField : function(filterSet) {
							var selectorSuffix = (filterSet.__init == 'FROM') ? '-daterange-from-date' : '-daterange-to-date';
							var selector = '#' + filterSet.code + selectorSuffix;
							var buttonText = (filterSet.__init == 'FROM') ? filterSet.from_tooltip : filterSet.to_tooltip;
							var calendarIcon = resource.base_url + '/' + themepath + '/expertusone-internals/images/calendar_icon.JPG';
							// alert(calendarIcon);
							$(selector).datepicker({
												duration : '',
												showTime : false,
												constrainInput : false,
												stepMinutes : 5,
												stepHours : 1,
												time24h : true,
												dateFormat : "mm-dd-yy",
												buttonImage : calendarIcon,
												buttonImageOnly : true,
												firstDay : 0,
												showOn : 'both',
												buttonText : buttonText,
												showButtonPanel : true,
												changeMonth : true,
												changeYear : true,
												daterangeDiffDays : 20000,

												beforeShow : function(input) {

													var data = $("#" + input.id).metadata();
													var dateFieldCode = data.dateFieldCode;
													var dateShowOption = data.dateRangeShowOption;

													if (dateShowOption == "" || dateShowOption == null || dateShowOption == "undefined") {
														dateShowOption = "all";
													}
													if (dateFieldCode == "" || dateFieldCode == null || dateFieldCode == "undefined") {
														var tokens = data .split("-");
														var dateFieldCode = tokens[0];
													}

													var dateMin = null;
													var dateMax = null;
													var fromSelectorCode = dateFieldCode + '-daterange-from-date';
													var toSelectorCode = dateFieldCode + '-daterange-to-date';
													var fromSelector = '#' + fromSelectorCode;
													var toSelector = '#' + toSelectorCode;

													if (input.id == fromSelectorCode && $(fromSelector).datepicker("getDate") != null) {
														var daterangeDiffDays = $(fromSelector).datepicker("option","daterangeDiffDays");

														if (dateShowOption == "future") { // To show future and today date for From Date

															dateMin = new Date();
															dateMax = $(toSelector).datepicker("getDate");
															dateMax.setDate(dateMin.getDate()+daterangeDiffDays);

														} else if (dateShowOption == "past") { // Default is to Show Past and Today Date for From Date

															var toDateFull = $(toSelector).datepicker("getDate");
															var fromDateFull = $(fromSelector).datepicker("getDate");
															var fromDate = fromDateFull.getFullYear()+ ","+ fromDateFull.getMonth()+ ","+ fromDateFull.getDate();
															var toDate = toDateFull.getFullYear()+ ","+ toDateFull.getMonth()+ ","+ toDateFull.getDate();
															var dateFromObj = new Date(fromDate);
															var dateToObj = new Date(toDate);
															var diffDates = dateToObj - dateFromObj;

															dateMax = new Date();

															if (diffDates == 0) {
																dateMin = $(fromSelector).datepicker("getDate");
																dateMin.setDate(dateMin.getDate() - daterangeDiffDays);
																dateMax = new Date();
															} else {
																dateMax = $(toSelector).datepicker("getDate");
																dateMin = $(toSelector).datepicker("getDate");
																dateMin.setDate(dateMax.getDate() - daterangeDiffDays);
																dateMax.setDate(dateMax.getDate() - 0);
															}
														}
													} else if (input.id == toSelectorCode) {

														var daterangeDiffDays = $(toSelector).datepicker("option","daterangeDiffDays");

														if (dateShowOption == "future") { // To Show Future and Today Date for To Date
															dateMax = new Date();
															if ($(fromSelector).datepicker("getDate") != null) {
																dateMin = $(fromSelector).datepicker("getDate");
																dateMax = $(fromSelector).datepicker("getDate");
																dateMax.setDate(dateMax.getDate() + daterangeDiffDays);
															}
														} else if (dateShowOption == "past") { // Default is to Show Past and Today Date for To Date
															dateMax = new Date();
															if ($(fromSelector).datepicker("getDate") != null) {
																dateMin = $(fromSelector).datepicker("getDate");
																var minDate = dateMin.getFullYear()+ ","+ dateMin.getMonth()+ ","+ dateMin.getDate();
																var maxDate = dateMax.getFullYear()+ ","+ dateMax.getMonth()+ ","+ dateMax.getDate();
																var dateFromObj = new Date(minDate);
																var dateToObj = new Date(maxDate);
																var diffDates = dateToObj - dateFromObj;

																if (diffDates <= 0) {
																	dateMin.setDate(dateMin.getDate() - daterangeDiffDays);
																} else {
																	dateMin.setDate(dateMin.getDate() - 0);
																}
															}
														}
													}

													return {
														minDate : dateMin,
														maxDate : dateMax
													};

												}
											});

							var defaultText = (filterSet.__init == 'FROM') ? filterSet.from_default_text : filterSet.to_default_text;

							this.initTextDateFilterBlurStyle(selector,defaultText);
						},

						initSlider: function(filterSet){
							obj = this;
							var valueMin = $('#'+filterSet.code+'_filterset').data("startval");
							var valueMax = $('#'+filterSet.code+'_filterset').data("endval");
							var prefix = $('#'+filterSet.code+'_filterset').data("prefix");
							var suffix = $('#'+filterSet.code+'_filterset').data("suffix");
							if(valueMax > valueMin){

								$('#value-slider-range-' + filterSet.code + '').slider({
									range: true,
									min: valueMin,
									max: valueMax,
									values: [ valueMin, valueMax ],
									slide: function( event, ui ) {
										$( '#value-slide-left-'+filterSet.code+'' ).val( prefix + ui.values[ 0 ] + suffix);
										$( '#value-slide-right-'+filterSet.code+'' ).val( prefix + ui.values[ 1 ] + suffix);
										 //+ " - $" + ui.values[ 1 ] );
									},
									change: function(e,ui){
										//$("#prg-admin-search").data("prgsesarch").searchAction();
										 obj.narrowSearchBySlider();
								    }
								});
								$( '#value-slide-left-'+filterSet.code+'').val( prefix + $('#value-slider-range-'+filterSet.code+'' ).slider( 'values', 0 ) + suffix);
								$( '#value-slide-right-'+filterSet.code+'' ).val( prefix + $( '#value-slider-range-'+filterSet.code+'' ).slider( 'values', 1 ) + suffix);
								/*if(this.currTheme == "expertusoneV2"){
									$( '#value-slider-range-'+filterSet.code+' a:last').css('margin-left','-9px');
								}*/
							}

						},

						/*
						 * IsValidDate() - Check that given date is valid or not -
						 * TRUE if date is valid - FALSE if date is valid
						 */
						IsValidDate : function(Day, Mn, Yr) {
							var DateVal = Mn + "/" + Day + "/" + Yr;
							var dt = new Date(DateVal);

							if (dt.getDate() != Day) {
								//alert('Invalid Date');
								return (false);
							} else if (dt.getMonth() != Mn - 1) {
								// this is for the purpose JavaScript starts the
								// month from 0
								//alert('Invalid Date');
								return (false);
							} else if (dt.getFullYear() != Yr) {
								//alert('Invalid Date');
								return (false);
							}

							return (true);
						},

						/*
						 * validateDateRangeDates() - Action for the Go button
						 * in the daterange component - validates dates in the
						 * date range narrow search component. - On error,
						 * displays the error message in the component. - On
						 * success, submits the search request
						 */
						validateDateRangeDates : function(code,ID) {
                            var fromDefaultText = $('#' + ID).data('default-fromtext');
                            var toDefaultText =  $('#' + ID).data('default-totext');
							var fromDateSelector = '#' + code + '-daterange-from-date';

							var toDateSelector = '#' + code + '-daterange-to-date';

							var dateFrom = ($(fromDateSelector).val() == fromDefaultText) ? '' : $(fromDateSelector).val();
							var dateTo = ($(toDateSelector).val() == toDefaultText) ? '' : $(toDateSelector).val();

							if (dateFrom == '' && dateTo == '') {
								this.narrowSearchByDateRange(code);
								return;
							}

							var msg = "";
							var validFromDate = false;
							var validToDate = false;
							if (dateFrom != "") {
								var dateFromSplit = dateFrom.split('-');
								var fromDay = dateFromSplit[1];
								var fromMonth = dateFromSplit[0];
								var fromYear = dateFromSplit[2];
								validFromDate = this.IsValidDate(fromDay, fromMonth, fromYear);
								if (!validFromDate) {
									msg += Drupal.t("LBL1154");
								}
							}
							if (dateTo != "") {
								var dateToSplit = dateTo.split('-');
								var toDay = dateToSplit[1];
								var toMonth = dateToSplit[0];
								var toYear = dateToSplit[2];
								validToDate = this.IsValidDate(toDay, toMonth, toYear);
								if (!validToDate) {
									if (msg != "")
										msg += "<br/>";
									//msg += Drupal.t("LBL1146");
								}
							}
							if (validFromDate == true && validFromDate == true) {
								var dateFromObjStr = fromYear + '/' + fromMonth + '/' + fromDay;
								var dateToObjStr = toYear + '/' + toMonth + '/'+ toDay;
								var dateFromObj = new Date(dateFromObjStr);
								var dateToObj = new Date(dateToObjStr);
								var diffDates = dateToObj - dateFromObj;
								if (diffDates < 0) {
									if (msg != "")
										msg += "<br/>";
									msg += Drupal.t("LBL1155");
								}
							}
							errMsgDivSelector = '#' + code + '-daterange-errmsg';
							if (msg != "") {
								$(errMsgDivSelector).html(Drupal.t(msg));
								$(errMsgDivSelector).css('display','inline-block');
								$(errMsgDivSelector).show();
							} else {
								$(errMsgDivSelector).hide();
								this.narrowSearchByDateRange(code);
							}

						},

						/*
						 * clearDateRangeFields() - Action for the clear link in
						 * the daterange component
						 */
						clearDateRangeFields : function(code, ID) {
							// Prepare the jQuery selectors
							var fromDefaultText = $('#' + ID).data('default-fromtext');
                            var toDefaultText =  $('#' + ID).data('default-totext');
							var fromDateSelector = "#" + code + "-daterange-from-date";
							var toDateSelector = "#" + code + "-daterange-to-date";
							var clearDateRangeSelector = '#' + code + '-daterange-clr';
							var errMsgDivSelector = '#' + code + '-daterange-errmsg';

							// Show the default texts in date fields in default
							// color and font
							$(fromDateSelector).removeClass('narrow-search-filterset-daterange-nonempty');
							$(fromDateSelector).addClass('narrow-search-filterset-daterange-empty');
							$(fromDateSelector).val(fromDefaultText);

							$(toDateSelector).removeClass('narrow-search-filterset-daterange-nonempty');
							$(toDateSelector).addClass('narrow-search-filterset-daterange-empty');
							$(toDateSelector).val(toDefaultText);

							// Hide the clear link
							$(clearDateRangeSelector).css('display', 'none');

							// Hide the error message box
							$(errMsgDivSelector).hide();

							// Refresh the datepicker
							$(fromDateSelector).datepicker("refresh");
							$(toDateSelector).datepicker("refresh");

							// Search as per cleared filter settings
							this.narrowSearchByDateRange(code);

						},
						/*
						 * validatetextField() - Action for the clear link to appear near the
						 * relevant text box.
						 */

						validatetextField : function(code, ID) {
							var defaultText = $('#' + ID).data('default-text');
							var defaultTextSelector = "#" + code + "-addltext-filter";
							var showClearLink = "#" + code + "-addltext-clr";
							if($("#" + code + "-addltext-filter").val() != defaultText){
								$(showClearLink).css('display', 'block');
							}

						},
						/*
						 * clearTextBoxFields() - Action for the clear link in
						 * the username, organization and location component
						 */
						clearTextBoxFields : function(code, ID) {
							var defaultText = $('#' + ID).data('default-text');
							var defaultTextSelector = "#" + code + "-addltext-filter";
							var clearTextFieldSelector = '#' + code + '-addltext-clr';
							// Show the default texts in date fields in default
							// color and font
							$(defaultTextSelector).removeClass('narrow-search-filterset-daterange-nonempty');
							$(defaultTextSelector).addClass('narrow-search-filterset-daterange-empty');
							$(defaultTextSelector).val(defaultText);
							// Hide the clear link
							$(clearTextFieldSelector).css('display', 'none');
							this.narrowSearchByAddlText(code);

						},

						clearTagsCloudNarrowFilter : function(code) {
							$('#' + code + '-addltext-filter').val('');
							$('.tagscloud-tag').css('text-decoration', '');
							$('#' + code + '-tagscloud-clr').css('display', 'none');
							$('#root-admin').data('narrowsearch').refreshGrid();
						},

						moreListDisplay : function(recLen,dispId,html_id) {
							if(html_id==''){
				            		for(i=0;i<=recLen;i++){
				            			$('#'+dispId+'_'+i).css("display","block");
				            		}
				            		$('#'+dispId+'_more').css("display","none");
				            		$('#'+dispId+'_short').css("display","block");
							}else{
			            		for(i=0;i<=recLen;i++){
			            			$('#'+html_id+' #'+dispId+'_'+i).css("display","block");
			            		}
			            		$('#'+html_id+' #'+dispId+'_more').css("display","none");
			            		$('#'+html_id+' #'+dispId+'_short').css("display","block");
							}
							vtip();
		            	},

		            	shortListDisplay : function(recLen,dispId,html_id){
		            		if(html_id==''){
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
		            		}else{
			            		for(i=0;i<=recLen;i++){
			            			if(i<=4){
			            				$('#'+html_id+' #'+dispId+'_'+i).css("display","block");
			            				}
			            			else {
			            				$('#'+html_id+' #'+dispId+'_'+i).css("display","none");
			            				}
			            			}
			            		$('#'+html_id+' #'+dispId+'_short').css("display","none");
			            		$('#'+html_id+' #'+dispId+'_more').css("display","block");
		            		}
		            		vtip();
		            	},


						/*	callDeleteObject : function(objectId,objectType){
						 var obj = this;
							url = obj.constructUrl("ajax/admincore/deleteobject/" + objectId+'/'+objectType);
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
								//alert(result)
								   if(result > 0){
									   if(objectType == "Class") {
										   obj.callMessageWindow('Delete','This Class has been Enrolled/Futured. So cannot delete.');
									   }else if(objectType == "Course"){
										   obj.callMessageWindow('Delete','This Course has one of the Enrolled/Futured Class. So cannot delete.');
									   }else{
										   obj.callMessageWindow('Delete','Training Plan');
									   }

								   }else{
									   obj.displayDeleteWizard(objectId,objectType);
								   }
								}
						    });

					},	*/

					callDeleteProcess : function(objectId,objectType){
						$('#delete-msg-wizard').remove();
						var obj = this;
							url = obj.constructUrl("ajax/admincore/deleteprocessobject/" + objectId+'/'+objectType);
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
									Drupal.CTools.Modal.dismiss();
									// Admin Calendar - Start - Refresh calendar after delete announcement
									if(window.location.href.indexOf("admincalendar") >0 )
										getCalendarData();
									// Admin Calendar - End
									$("#root-admin").data("narrowsearch").refreshGrid();
								}
						    });

					},

					displayDeleteWizard : function(title,objectId,objectType,wSize){
						var uniqueClassPopup = '';
						if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
							if(objectType == "Class" && document.getElementById('catalog-course-basic-addedit-form')){
								uniqueClassPopup = 'unique-delete-class';
							}
						}
						var wSize = (wSize) ? wSize : 300;
						var closeButAction = '';
						var esignForType = '';
						var settingsList = ['cre_usr_dpt', 'cre_usr_etp', 'cre_usr_jrl', 'cre_usr_jtl', 'cre_usr_ptp'];
						if (settingsList.indexOf(objectType) != -1)  {
							closeButAction = function(){ $('#deleteAddedList-'+objectId).click(); };
							esignForType = 'DeleteAdminObjectSettingsList';
						} else {
							
							if(objectId == "cre_ste_mod_aut" || objectId == "cre_ste_mod_cattr"){   //For enable/Disable of Content Authoring module
								closeButAction = function(){ $("#root-admin").data("narrowsearch").publishAndUnpublishModule(objectType,''); };
							}else{
							closeButAction = function(){ $("#root-admin").data("narrowsearch").callDeleteProcess(objectId,objectType); };
							 }
							esignForType = 'DeleteAdminObject';
						}
							$('#delete-msg-wizard').remove();
					    var html = '';
					    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
					    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';

					   // html+= '<tr><td height="40px">&nbsp;</td></tr>';
					    if(this.currTheme == 'expertusoneV2'){
					    	if(objectType == 'Content')
					    		html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
					    	else
					    		html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+''+'?'+'</td></tr>';
					    } else {
					    	if(objectType == 'Content')
					    		html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: left;">'+title+'</td></tr>';
					    	else
					    		html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+''+'?'+'</td></tr>';
					    }
					    html+='</table>';
					    html+='</div>';
					    $("body").append(html);
					    var closeButt={};
					    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

					    if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != "" && objectId != "cre_ste_mod_aut" && objectId != "cre_ste_mod_cattr"){
					    	var esignObj = {'popupDiv':'delete-object-dialog','esignFor':esignForType,'objectId':objectId,'objectType':objectType};
					    	closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj);  };
					    }else{
					    	closeButt[Drupal.t('Yes')]= closeButAction;
					    }

					    if(objectType == 'UnlockUser'){
					    	var drupalTitle = Drupal.t("LBL930");
					    }else if(objectType == 'exp_sp_administration_contentauthor'){
					    	var drupalTitle = Drupal.t("LBL749");
					    }else if(objectType == 'exp_sp_administration_customattribute'){
					    	var drupalTitle = Drupal.t("LBL2015");
					    }else{
					    	var drupalTitle = Drupal.t("LBL286");
					    }

					    
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
					    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");

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
						/*-- 37211: Unable to delete a class in FF version 32.0 --*/
					    if($('div.qtip-defaults').length > 0) {
					    	var prevZindex = $('.qtip-defaults').css('z-index');
					    	$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
					    	$('.ui-widget-overlay').css('z-index', prevZindex+1);
					    }

					},
					
					displayH5PPreview : function(h5pplayer,objectId,objectType,wSize,hSize){
							var uniqueClassPopup = '';
						if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
							if(objectType == "Class" && document.getElementById('catalog-course-basic-addedit-form')){
								uniqueClassPopup = 'unique-delete-class';
							}
						}
						var wSize = (wSize) ? wSize : 300;
						var hSize = (hSize) ? hSize : 800;
						var closeButAction = '';
						var esignForType = '';
						$('#delete-msg-wizard').remove();
					    var html = '';
					    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
					    //html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';

					   
					   //html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
					   
					    
					    //html+='</table>';
					    html+='</div>';
					    $("body").append(html);
					    var closeButt={};
					    var drupalTitle = Drupal.t("LBL694");
					    $("#delete-msg-wizard").dialog({
					        position:'center',//[(getWindowWidth()-400)/2,200],
					        bgiframe: true,
					        width:wSize,
					        height:hSize,
					        resizable:false,
					        modal: true,
					        title:Drupal.t(drupalTitle),//"title",
					        buttons:closeButt,
					        closeOnEscape: true,
					        draggable:false,
					        zIndex : 10005,
					        overlay:
					         {
					           opacity: 0.9,
					           background: "black"
					         }
					    });
					    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");

					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

					    $("#delete-msg-wizard").show();
					    //display content
					    if(objectType != "customattr"){
					    window.setTimeout(function(){
					    	createLoaderNew("delete-msg-wizard");
					    	},500);
					    }
					    $("#delete-msg-wizard").html(h5pplayer);
					    $("#delete-msg-wizard").css("overflow","hidden");

						$('.ui-dialog-titlebar-close').click(function(){
					        $("#delete-msg-wizard").remove();
					    });
						if(this.currTheme == 'expertusoneV2'){
					    	changeDialogPopUI();
					    }
						/*-- 37211: Unable to delete a class in FF version 32.0 --*/
					    if($('div.qtip-defaults').length > 0) {
					    	var prevZindex = $('.qtip-defaults').css('z-index');
					    	$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
					    	$('.ui-widget-overlay').css('z-index', prevZindex+1);
					    }

					},
					

					clearBubblePopupSet : function() {
						//this.qtipLoadSet = array();
					},

					fillCompletionDate : function(uniqueId, uniqueId2){
						var overallDate = $('#overall_completion_date_'+uniqueId2).val();
						$('input[name="hidden_completion_date_'+uniqueId+'"]').val(overallDate);
						//$('#datagrid-completiondate-'+uniqueId).val(overallDate);
					},

					completionDateQTip : function(uniqueId,entityId,sessionStartdate){
					$('.qtip-popup-exempted').html('').hide();
						$('.catalog-pub-add-list').hide();
						$('#completion_date_container_'+uniqueId).show();
						$('#overall_completion_date'+uniqueId).datepicker( "destroy" );

						if(entityId!='' && entityId!=undefined){
							if(sessionStartdate !== undefined) {
								if(this.currTheme == 'expertusoneV2'){
									if($.trim(sessionStartdate)){
										$('#overall_completion_date_'+uniqueId).datepicker({minDate:new Date(sessionStartdate), dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
									}
									else{
									    $('#overall_completion_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
									}
								}else{
								    if($.trim(sessionStartdate)){
										  $('#overall_completion_date_'+uniqueId).datepicker({minDate:new Date(sessionStartdate), dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
										}
										else{
										  $('#overall_completion_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
										}
								} 
							}
							else {
								//will work for TP completion where sessionStartdate will be undefined
								url = this.constructUrl("ajax/administration/getClassStartDate/" + entityId );
								$.ajax({
									type: "POST",
									url: url,
									data:  '',
									success: function(result){
										var sesStartDate = result.replace(/\,/g, '/');
										this.currTheme = Drupal.settings.ajaxPageState.theme;
										if(this.currTheme == 'expertusoneV2'){
											if($.trim(sesStartDate)){
												$('#overall_completion_date_'+uniqueId).datepicker({minDate:new Date(sesStartDate), dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
											}
											else{
											    $('#overall_completion_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
											}
										}else{
										    if($.trim(sesStartDate)){
	 										  $('#overall_completion_date_'+uniqueId).datepicker({minDate:new Date(sesStartDate), dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
	 										}
	 										else{
	 										  $('#overall_completion_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
	 										}
										}
									}
								});
							}

						}else{
							this.currTheme = Drupal.settings.ajaxPageState.theme;
							if(this.currTheme =='expertusoneV2'){
								$('#overall_completion_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
							}else{
								$('#overall_completion_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
					        }
						}
					},

					fillRegistrationDate : function(uniqueId, uniqueId2){
						var overallDate = $('#overall_registration_date_'+uniqueId2).val();
						$('input[name="hidden_registration_date_'+uniqueId+'"]').val(overallDate);
						$('#registration_date_container_'+uniqueId).hide();
					},

					registrationDateQTip : function(uniqueId,entityId, isPastClass, sessionStartdate){
						$('.catalog-pub-add-list').hide();
						if(entityId!='' && entityId!=undefined){
							if(isPastClass) {
								$('#registration_date_container_'+uniqueId).show();
								$('#overall_registration_date'+uniqueId).datepicker("destroy");
								this.currTheme = Drupal.settings.ajaxPageState.theme;
								if(this.currTheme == 'expertusoneV2'){
									if($.trim(sessionStartdate)){
										$('#overall_registration_date_'+uniqueId).datepicker({maxDate:new Date(sessionStartdate), dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
									}
									else{
										$('#overall_registration_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
									}
								}else{
									if($.trim(sessionStartdate)){
										$('#overall_registration_date_'+uniqueId).datepicker({maxDate:new Date(sessionStartdate), dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
									}
									else{
										$('#overall_registration_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
									}
								}
							}
						}else{
							this.currTheme = Drupal.settings.ajaxPageState.theme;
							if(this.currTheme =='expertusoneV2'){
								$('#overall_registration_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
							}else{
								$('#overall_registration_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
					        }
						}
					},

					getBubblePopup : function(qtipObj) {
						try{						
							closeQtip('');					
							var popupId 	= qtipObj.popupDispId;	
							var qtipLen 	= this.qtipLenth;
							var inArrQtip = $.inArray(popupId, this.qtipLoadSet);
							this.qtipLoadSet[qtipLen] = popupId;
							this.qtipLenth = this.qtipLenth+1;
							var entityType = qtipObj.entityType;
							var courseId = qtipObj.courseId!=undefined?qtipObj.courseId:'';
							var url 			  = resource.base_host+'?q='+qtipObj.url; // resource.base_host+'?q=administration/catalogaccess/33/cre_sys_obt_crs';
							var wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
							var hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
							var setwBubble        = wBubble-20;
							var setBtmLeft        = (setwBubble-104)/2;
							var topElements = '<span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
							var bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-b"><table cellspacing="0" width="100%" cellpadding="0"><tbody><tr><td class="bubble-blt" style="width:'+setBtmLeft+'px"></td><td class="bubble-blm"></td><td class="bubble-blr" style="width:'+setBtmLeft+'px"></td></tr></tbody></table></td><td class="bubble-br"></td></tr></tbody></table>';
							var tipPos = qtipObj.tipPosition;
							var catalogVisibleId = qtipObj.catalogVisibleId;
							var mLeft = 0;
							var tipPosition;
							var messageStyle = '';
							var bwidth = '';
							var crTheme = Drupal.settings.ajaxPageState.theme;
							switch(qtipObj.tipPosition) {
								case 'bottomRight':
									wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
									hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
									setwBubble        = wBubble-20;
									setBtmLeft        = (setwBubble-104)/2;
									mLeft			  = (qtipObj.catalogVisibleId.indexOf('editclass')>0)?45:(qtipObj.catalogVisibleId.indexOf('AttachCrsIdqtip')>0)?58:(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0)?49:62;
									bwidth            = (qtipObj.catalogVisibleId.indexOf('addusers')>0)?wBubble:Math.round((wBubble/2)/5);
									setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('addusers')>0)?'214':(bwidth*6)+12;
									setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('editclass')>0)?'373':(qtipObj.catalogVisibleId.indexOf('addclass')>0)?'373':(qtipObj.catalogVisibleId.indexOf('AttachCrsIdqtip')>0)?'168':(bwidth*6)+12;
									if(crTheme == 'expertusoneV2') {
									  setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('addusers')>0)?'172':(bwidth*6)+12;
									  setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('editclass')>0)?'373':(qtipObj.catalogVisibleId.indexOf('addclass')>0)?'373':(qtipObj.catalogVisibleId.indexOf('AttachCrsIdqtip')>0)?'168':(bwidth*6)+12;
									}
									/*if (navigator.userAgent.indexOf("Chrome")>0){
									  setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('editclass')>0)?'373':(bwidth*6)+12;
									}else{

									}*/
									//Set buble tool tip arrow position
									tipPosition       = (qtipObj.tipPosition) ? qtipObj.tipPosition : 'bottomMiddle';
									topElements = '<span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
									bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-b"><table cellspacing="0" width="100%" cellpadding="0"><tbody><tr><td class="bubble-blt" style="width:'+setBtmLeft+'px"></td><td class="bubble-blr" style="width:'+setBtmLeft+'px"></td><td class="bubble-blm"></td></tr></tbody></table></td><td class="bubble-br"></td></tr></tbody></table>';
									$("#"+popupId).parent().parent().parent().parent().removeClass("qtip-access-control");
								break;
								case 'bottomCorner':
									wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
									hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
									setwBubble        = wBubble-20;
									if(crTheme != 'expertusoneV2') {
										if (navigator.userAgent.indexOf("Chrome")>0){
											setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?(wBubble==780?374:344):(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?7:(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?191:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?451:451;
											setBtmRight       = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?281:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?650:651):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?16:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?175:175;
											mLeft			  = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?325:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?712:712):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?59:218;
										}else{
											setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?(wBubble==780?374:344):(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?6:(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?191:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?458:451;
											setBtmRight       = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?283:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?535:619):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?16:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?178:175;
											mLeft			  = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?325:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?712:712):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?59:218;
										}
									}else {
										if (navigator.userAgent.indexOf("Chrome")>0){
											setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?(wBubble==780?374:344):(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?7:(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?191:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?472:451;
											setBtmRight       = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?281:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?650:651):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?16:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?185:175;
											mLeft			  = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?325:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?712:712):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?59:218;
										}else{
											setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?(wBubble==780?374:344):(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?6:(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?191:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?472:451;
											setBtmRight       = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?283:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?651:619):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?16:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?185:175;
											mLeft			  = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?325:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?712:712):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?59:218;
										}
									}

									//Set buble tool tip arrow position
									//var tipPosition       = (qtipObj.tipPosition) ? qtipObj.tipPosition : 'bottomMiddle';
									tipPosition       = 'bottomRight';
									topElements = '<span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
									bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-b"><table cellspacing="0" width="100%" cellpadding="0"><tbody><tr><td class="bubble-blt" style="width:'+setBtmLeft+'px"></td><td class="bubble-blm"></td><td class="bubble-blr" style="width:'+setBtmRight+'px"></td></tr></tbody></table></td><td class="bubble-br"></td></tr></tbody></table>';
								break;
								case 'rightBottom':
									//alert("right bottom");
									wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
									hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
									setwBubble        = wBubble-20;
									//setRightTop        = (hBubble-104)/2;
									tipPosition       = 'rightBottom';
									topElements = '<span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
									bottomElements ='</td><td class="bubble-cr-empty"><table cellspacing="0" width="100%" height="260" cellpadding="0"><tbody><tr><td class="bubble-brt" style="height: 169px"></td></tr><tr><td class="bubble-brm"></td></tr><tr><td class="bubble-brb" style="height:30px"></td></tr></tbody></table></td></tr><tr><td class="bubble-bl"></td><td class="bubble-bm"></td><td class="bubble-br"></td></tr></tbody></table>';
								break;
								case 'bottomLeft' :
									wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
									hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
									setwBubble        = wBubble-20;
									bwidth            = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?wBubble:Math.round((wBubble/2)/5);
									if(crTheme != 'expertusoneV2') {
									setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'17':bwidth+2;
									setBtmRight       = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'191':(bwidth*6)+12;
									setBtmMiddle	  = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'64':bwidth+2;
									 if (navigator.userAgent.indexOf("Chrome")>0){
										setBtmRight       = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'203':(bwidth*6)+12;
									 }
									}
									else{
										setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'14':bwidth+2;
										setBtmRight       = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'155':(bwidth*6)+12;
										setBtmMiddle	  = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'52':bwidth+2;
										if (navigator.userAgent.indexOf("Chrome")>0){
											setBtmRight       = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'194':(bwidth*6)+12;
										}
									}
									mLeft			  = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?-63:-110;
									//Set buble tool tip arrow position
									messageStyle      =  (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?"margin-left: -120px;":'';
									tipPosition       = 'bottomLeft';
									topElements = '<span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
									bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-b"><table cellspacing="0" width="100%" cellpadding="0"><tbody><tr><td class="bubble-blt" style="width:'+setBtmLeft+'px"></td><td class="bubble-blm" style="width:'+setBtmMiddle+'px"></td><td class="bubble-blr" style="width:'+setBtmRight+'px"></td></tr></tbody></table></td><td class="bubble-br"></td></tr></tbody></table>';
									$("#"+popupId).parent().parent().parent().parent().removeClass("qtip-access-control");
									break;
								default:
									wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
									hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
									setwBubble        = wBubble-20;
									setBtmLeft        = (setwBubble-104)/2;
									//Set buble tool tip arrow position
									tipPosition       = (qtipObj.tipPosition) ? qtipObj.tipPosition : 'bottomMiddle';
									topElements = '<span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
									bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-b"><table cellspacing="0" width="100%" cellpadding="0"><tbody><tr><td class="bubble-blt" style="width:'+setBtmLeft+'px"></td><td class="bubble-blm"></td><td class="bubble-blr" style="width:'+setBtmLeft+'px"></td></tr></tbody></table></td><td class="bubble-br"></td></tr></tbody></table>';
									$("#"+popupId).parent().parent().parent().parent().removeClass("qtip-access-control");

							}
							if(popupId == "qtip_addlistvalues_visible_disp_0_cre_usr_jtl" || popupId == "qtip_addlistvalues_visible_disp_0_cre_usr_dpt" || popupId == "qtip_addlistvalues_visible_disp_0_cre_usr_etp"  || popupId == "qtip_addlistvalues_visible_disp_0_cre_usr_jrl" || popupId == "qtip_addlistvalues_visible_disp_0_cre_usr_ptp"){
								$('.page-administration-people-setup #add_lst_pg_next,.page-administration-people-setup #add_lst_pg_previous,.page-administration-people-setup #add_lst_pg_last,.page-administration-people-setup #add_lst_pg_first,.page-administration-people-setup #add_lst_pg_txtfld,.page-administration-people-setup #listvalues-autocomplete,.page-administration-people-setup #search_listvalues').attr('style','pointer-events: none');								
							}
							try{
								$('#'+popupId).qtip("destroy");
							}catch(e){
								//Nothing to do
							}
							$('#'+qtipObj.catalogVisibleId).closest(".qtip").remove();
							//IMPORTANT !!! qtip-child, qtip-parent -- Do not set or use this class anywhere else -- this is meant to be for qtip issue fixes

							 var qtipClass = (qtipObj.qtipClass) ? qtipObj.qtipClass : 'qtip-child';
							 if(qtipSubClass != null) {
								 var qtipSubClass = (qtipObj.qtipSubClass) ? qtipObj.qtipSubClass : '';
								 qtipClass = (qtipSubClass != null) ? qtipClass+" "+qtipSubClass : qtipClass;
							 }
							 if(!document.getElementById(qtipObj.catalogVisibleId)) {
								if((!document.getElementById("popup_container_"+popupId))) {
									$('#'+popupId).qtip({
										overwrite: false, // Make sure another tooltip can't overwrite this one without it being explicitly destroyed
										content: {
										 	text: "<div style='min-height:75px;' id='"+qtipObj.catalogVisibleId+"'><div id='"+qtipObj.catalogVisibleId+"_disp' style='margin-left:"+mLeft+"px;min-height:75px;'>&nbsp;</div></div>",
										 	title: {
												      text: ' ',
												      button: '<div id="'+qtipObj.catalogVisibleId+'_close" class="admin-bubble-close" onclick="closeQtyp(\''+qtipObj.catalogVisibleId+'\',\''+courseId+'\')"> </div>'
											   	   }
											},
										   api:{
												beforeShow: function(){
													//	Hide qtip till the content load
													var qtupActive = $("#"+qtipObj.catalogVisibleId).closest(".qtip-active");
													$('#'+qtipObj.catalogVisibleId+"_close").css('visibility','hidden');
													$(qtupActive).find('.qtip-tip').css('visibility','hidden');
												},
										 		onShow: function() {
													// Hide qtip till the content load
													var qtupActive = $("#"+qtipObj.catalogVisibleId).closest(".qtip-active");
													$('#'+qtipObj.catalogVisibleId+"_close").css('visibility','hidden');
													$(qtupActive).find('.qtip-tip').css('visibility','hidden');
													$('.qtip').each(function(){
														var tmp = this;
														if($(tmp).find('#'+qtipObj.catalogVisibleId).each(function(){
															if($(tmp).attr('class').indexOf('qtip-active')<0){
																try{
																	$('#'+popupId).qtip("destroy");
																	$(this).remove();
																}catch(e){
																	//Nothing to do
																}
															}
														}));
													});
										 			if(tipPos != 'bottomCorner') {
								 						$("#"+catalogVisibleId).parent().parent().parent().parent().removeClass("qtip-access-control");

								 					}
													if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_addtemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0){
														var popTop = ($('#'+popupId).offset());
														var xtop = ($.browser.msie)?110:100;
														popTop = Math.round(popTop.top)-xtop;
														$("#"+qtipObj.catalogVisibleId).parents('.qtip-active').css('top',popTop+"px");
													}
													if(qtipObj.catalogVisibleId.indexOf('MoveUsersqtip')>0 && navigator.userAgent.indexOf("Chrome")>=0 ){
														var popTop = ($('#'+popupId).offset());
														var xtop = 150;
														popTop = Math.round(popTop.top)-xtop;
														$("#"+qtipObj.catalogVisibleId).parents('.qtip-active').css('top',popTop+"px");
													}
													$("#"+qtipObj.catalogVisibleId).parents('.qtip-active').addClass(qtipClass);
								 					//IMPORTANT !!! modal-qtips-close Do not set or use this class anywhere else -- this is meant to be for qtip issue fixes
								 					$("#"+qtipObj.catalogVisibleId).parents('.qtip-active').addClass('modal-qtips-close');
								 					$("#"+qtipObj.catalogVisibleId).parents('.qtip-active').siblings('.qtip-child').hide();
								 					if(qtipClass == "qtip-parent" || qtipClass == 'qtip-parent add-key-word-edit-list')
								 					$("#"+qtipObj.catalogVisibleId).parents('.qtip-active').siblings('.qtip-parent').hide();

													//if(!document.getElementById("paintContent"+popupId)) {
								 					if(1==1){
														EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader(qtipObj.catalogVisibleId+"_disp");
														var loadPos=0;
														if(tipPosition == 'rightBottom') {
									 						$(qtupActive).find('.qtip').addClass('qtip-access-control');
															$(qtupActive).find('.qtip-tip').attr('style','height: 64px');
															$(qtupActive).find('.qtip-tip').css('background','url("/sites/all/themes/core/expertusone/expertusone-internals/images/bubble_arrow_point_right.png") no-repeat -1px 3px');
															$(qtupActive).find('.qtip-tip').css('bottom','79px');
															$(qtupActive).find('.qtip-tip').css('left','638px');
															$(qtupActive).find('.qtip-tip').css('position','absolute');
															$('#'+qtipObj.catalogVisibleId+"_close").css('top','');
														}
														if(tipPosition == 'bottomRight' && mLeft!=0) {
															//$('.qtip-tip').css('right','-35px');
									 						var leftPos=0;
									 						var rightPos="0px";
									 						var bottomPos="0px";
									 						var zIndex;
									 						var crTheme = Drupal.settings.ajaxPageState.theme;
									 						var bnrAccleftPos = (crTheme == 'expertusoneV2') ? 672 : 663;
									 						this.currTheme = Drupal.settings.ajaxPageState.theme;

									 						if(qtipObj.tipPosition == 'bottomCorner'){ // Access control fix
									 							if(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls'){ // Class Access control
									 								loadPos = 520;
									 								leftPos = 308;
									 								rightPos = "-12px";
									 							}else if((qtipObj.catalogVisibleId.indexOf('Accessqtip')>0) && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not')){ // Banner / Announcement Access control
									 								loadPos = 715;
									 								leftPos = (crTheme == 'expertusoneV2') ? ((entityType=='cre_sys_obt_not')?693:692) : 692;;
									 								rightPos = (entityType=='cre_sys_obt_not')?"-31px":"-30px";
									 							}else if((qtipObj.catalogVisibleId.indexOf('Accessqtip')>0) && entityType!='cre_sys_obt_crs' && crTheme == 'expertusoneV2'){ // TP Access
									 								loadPos = 430;
									 								leftPos = 205;
									 								rightPos = "-6px";
									 							}else {
									 								loadPos = 430;
									 								leftPos = 201;
									 								rightPos = "-12px";
									 							}
									 						}else{ //class qtip position fix
									 							if(qtipObj.catalogVisibleId.indexOf('editclass')>0){
									 								loadPos = 380;
									 								leftPos = 28;
									 								rightPos = "-14px";
									 							}else if(qtipObj.catalogVisibleId.indexOf('AttachCrsIdqtip')>0){
									 								loadPos = 380;
									 								leftPos = 40;
									 								rightPos = "-27px";
									 							}else if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0){
									 								loadPos = 380;
									 								leftPos = 30;
									 								rightPos = "-18px";
									 								zIndex ="100";
									 								bottomPos = (this.currTheme == "expertusoneV2") ? "0px" : "-64px";
									 							}else if(qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0){
									 								loadPos = 380;
									 								leftPos = 44;
									 								rightPos = "-31px";
									 								zIndex ="100";
									 								bottomPos = (this.currTheme == "expertusoneV2") ? "0px" : "-64px";
									 							}else if(qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0){
									 								loadPos = 380;
									 								leftPos = 30;
									 								rightPos = "-18px";
									 								zIndex ="100";
									 							}else if(qtipObj.catalogVisibleId.indexOf('AttachIdqtip_addusers')>0){
									 								loadPos = 380;
									 								leftPos = 65;
									 								zIndex ="100";
									 								rightPos = "-41px";
									 							}else if(qtipObj.catalogVisibleId.indexOf('listvalues')>0){
									 								zIndex ="100";
									 							}else if(qtipObj.catalogVisibleId.indexOf('listvalpeoedit')>0){
									 								zIndex ="100";
									 							}else{
									 								loadPos = 380;
									 								leftPos = 44;
									 								rightPos = "-31px";
									 							}
									 						}

															$(qtupActive).find('.qtip').addClass('qtip-access-control');
															$(qtupActive).find('.qtip-tip').css('right',rightPos);
															$('#'+qtipObj.catalogVisibleId+"_close").css('left',leftPos);
															if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0){
																$(qtupActive).find('.qtip-tip').css('bottom',bottomPos);
																$(qtupActive).find('.qtip-tip').css('z-index',zIndex);
																var isAtLeastIE11 = !!(navigator.userAgent.match(/Trident/) && !navigator.userAgent.match(/MSIE/))? true : false;
																if(this.currTheme == "expertusoneV2" && $.browser.msie && parseInt($.browser.version, 10)=='10' || this.currTheme == "expertusoneV2" && isAtLeastIE11==1 ){
																	$(".qtip-button .admin-bubble-close").css({"top":"-2px"});
																	$(qtupActive).find('.qtip-tip').css('bottom','-8px');
																}
                                                               if(navigator.userAgent.indexOf("Safari")!=-1){
																	$(qtupActive).find('.qtip-tip').css('bottom','-8px');
																}
															}
															if(qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_addtemplate')>0){
																$(qtupActive).find('.qtip-tip').css('z-index',zIndex);
																if(this.currTheme == "expertusoneV2" && $.browser.msie && parseInt($.browser.version, 10)=='10'){
																	$(".qtip-button .admin-bubble-close").css({"top":"-2px"});
																}
																if(navigator.userAgent.indexOf("Safari")!=-1){
																	$(qtupActive).find('.qtip-tip').css('bottom','-8px');
																}
															}
															if(qtipObj.catalogVisibleId.indexOf('AttachIdqtip_addusers')>0){
																$(qtupActive).find('.qtip-tip').css('z-index',zIndex);
															}
															if(qtipObj.catalogVisibleId.indexOf('listvalues')>0){
																$(qtupActive).find('.qtip-tip').css('z-index',zIndex);
																if(this.currTheme == "expertusoneV2"){
																if($.browser.msie && parseInt($.browser.version, 10)=='10'){
																	$(qtupActive).find('.qtip-tip').css('bottom','1px');
																	$(".qtip-button .admin-bubble-close").css({"left":"43px","top":"-4px"});
																}else if($.browser.msie && parseInt($.browser.version, 10)=='9'){
																	$(qtupActive).find('.qtip-tip').css('bottom','0');
																	$(".qtip-button .admin-bubble-close").css({"left":"43px","top":"-4px"});
																}else if($.browser.msie && parseInt($.browser.version, 10)=='8'){
																	$(qtupActive).find('.qtip-tip').css({"bottom":"41px","right":"20px"});
																	$(".qtip-button .admin-bubble-close").css({"left":"43px","top":"-3px"});
																}else{
																	$(qtupActive).find('.qtip-tip').css('right','-31px');
																	$(".qtip-button .admin-bubble-close").css({"left":"44px"});
																}
																}else{
																	if($.browser.msie && parseInt($.browser.version, 10)=='10'|| $.browser.msie && parseInt($.browser.version, 10)=='9'){
																		$(qtupActive).find('.qtip-tip').css({'bottom':'0','right':'-31px'});
																		$(".qtip-button .admin-bubble-close").css({"left":"43px","top":"-4px"});
																	}else if($.browser.msie && parseInt($.browser.version, 10)=='8'){
																		$(qtupActive).find('.qtip-tip').css({"bottom":"40px","right":"19px"});
																		$(".qtip-button .admin-bubble-close").css({"left":"43px","top":"-3px"});
																	}else{
																		$(qtupActive).find('.qtip-tip').css('right','-31px');
																		$(".qtip-button .admin-bubble-close").css({"left":"44px"});
																	}

																}
															}

														}else if(tipPosition == 'bottomLeft' && mLeft!=0){
															var leftPos=0;
									 						var rightPos="0px";
															if(qtipObj.catalogVisibleId.indexOf('MoveUsersqtip')>0){
																loadPos = 380;
								 								leftPos = 407;
								 								rightPos = "-14px";
								 								if(this.currTheme != "expertusoneV2" && $.browser.msie && parseInt($.browser.version, 10)=='9'){
																	$(qtupActive).find('.qtip-tip').css('bottom','0');
																}
								 								if(this.currTheme != "expertusoneV2" && $.browser.msie && parseInt($.browser.version, 10)=='8'){
																	$(qtupActive).find('.qtip-tip').css('bottom','40px');
																	$(qtupActive).find('.qtip-tip').css('margin-bottom','38px');
																}
								 								if(this.currTheme = "expertusoneV2" && $.browser.msie && parseInt($.browser.version, 10)=='10'){
																	$(qtupActive).find('.qtip-tip').css('bottom','0');
																}
								 								$(qtupActive).find('.qtip-tip').css('right',rightPos).css("left","13px");
															}else if(qtipObj.catalogVisibleId.indexOf('listvalues')>0){
																leftPos = "-80px";
																$(qtupActive).find('.qtip-tip').css('left','16px');
																if (navigator.userAgent.indexOf("Chrome")>0){
																	$(qtupActive).find('.qtip-tip').css('left','13px');
																}
																if($.browser.msie && parseInt($.browser.version, 10)=='8'){
																	$(qtupActive).find('.qtip-tip').css('margin-bottom','38px');
																}
																if($.browser.msie && parseInt($.browser.version, 10)=='9' || $.browser.msie && parseInt($.browser.version, 10)=='10'){
																	$(qtupActive).find('.qtip-tip').css('bottom','0');
																}
															}
															//$(qtupActive).find('.qtip').addClass('qtip-access-control');
															$(qtupActive).find('.qtip-tip').css('right',rightPos);
															$('#'+qtipObj.catalogVisibleId+"_close").css('left',leftPos);
														}
														if(qtipObj.catalogVisibleId.indexOf("Access")>=0 || qtipObj.catalogVisibleId.indexOf("addclass")>=0
																|| qtipObj.catalogVisibleId.indexOf("editclass")>=0){
																$('#loaderdiv'+qtipObj.catalogVisibleId+"_disp").css('padding-left',loadPos);
														}
													   if(qtipObj.catalogVisibleId.indexOf("addclass")>=0){
														   if($.browser.msie && parseInt($.browser.version, 10)=='9' || $.browser.msie && parseInt($.browser.version, 10)=='10'){
																$(qtupActive).find('.qtip-tip').css('bottom','0');
																$(".qtip-button .admin-bubble-close").css({"left":"44px","top":"-1px"});
															}else if($.browser.msie && parseInt($.browser.version, 10)=='8'){
																$(qtupActive).find('.qtip-tip').css('bottom','40px');
																$(qtupActive).find('.qtip-tip').css('right','19px');
																$(qtupActive).find('.qtip-tip').css("margin-bottom","38px");
															}
													   }
													   if(qtipObj.catalogVisibleId.indexOf('editclass')>0){
															if($.browser.msie && parseInt($.browser.version, 10)=='8'){
																$(qtupActive).find('.qtip-tip').css('margin-bottom','76px');
																$(qtupActive).find('.qtip-tip').css('margin-right','50px');
															}
															if($.browser.msie && parseInt($.browser.version, 10)=='9' || $.browser.msie && parseInt($.browser.version, 10)=='10' ){
																$(qtupActive).find('.qtip-tip').css('bottom','0');
															}
														}
													   if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0){
														   if($.browser.msie && parseInt($.browser.version, 10)=='8' && this.currTheme == "expertusoneV2"){
																$(qtupActive).find('.qtip-tip').css('right','26px');
							 								}
								 						}
													   if(qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0){
														   if($.browser.msie && parseInt($.browser.version, 10)=='8' && this.currTheme == "expertusoneV2"){
																$(qtupActive).find('.qtip-tip').css('right','26px');
							 								}
														   if($.browser.msie && parseInt($.browser.version, 10)=='8' && this.currTheme != "expertusoneV2"){
																$(qtupActive).find('.qtip-tip').css('right','32px');
																$(qtupActive).find('.qtip-tip').css('bottom','32px');
							 								}
														   if($.browser.msie && parseInt($.browser.version, 10)=='10' && this.currTheme != "expertusoneV2"){
															   $(".qtip-button .admin-bubble-close").css("top","-3px");
														   }
														 }


														if(qtipObj.catalogVisibleId.indexOf('listvaledit')>0){
															$(qtupActive).find('.qtip-tip').css('right','-12px');
															$('#'+qtipObj.catalogVisibleId+"_close").css('left','40px');
														}
														var bubbleTop = $("#"+qtipObj.catalogVisibleId).closest(".qtip-active").css('top');
														bubbleTop = parseInt(bubbleTop.substring(0,bubbleTop.length-2));
														var bubblehb = $("#"+qtipObj.catalogVisibleId).css('height');
														bubblehb = parseInt(bubblehb.substring(0,bubblehb.length-2));
														if(document.getElementById('qtip_position')!=null){
															$('#qtip_position').val(qtipObj.catalogVisibleId+"#"+bubbleTop+"#"+bubblehb);
														}
											 			$.ajax({
											 				 type: "GET",
												   	         url: url,
												   	         data:  '',
												   	         success: function(data){
											 					var paintHtml = topElements+"<div id='paintContent"+popupId+"'><div id='show_expertus_message' style='"+messageStyle+"'></div>"+data.render_content_main+"</div>"+bottomElements;
											 					$.extend(true, Drupal.settings, data.drupal_settings);
											 					$("#"+qtipObj.catalogVisibleId+"_disp").html(paintHtml);
											 					EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader(qtipObj.catalogVisibleId+"_disp");

											 					var bubbleha = $("#"+qtipObj.catalogVisibleId).css('height');
											 					bubbleha = parseInt(bubbleha.substring(0,bubbleha.length-2));
																bubbleTop = bubbleTop-(bubbleha-bubblehb);
																$("#"+qtipObj.catalogVisibleId).closest(".qtip-active").css('top',bubbleTop);
																bubblehb = $("#"+qtipObj.catalogVisibleId).css('height');
																bubblehb = parseInt(bubblehb.substring(0,bubblehb.length-2));
																if(document.getElementById('qtip_position')!=null){
																	$('#qtip_position').val(qtipObj.catalogVisibleId+"#"+bubbleTop+"#"+bubblehb);
																}
																$('#'+qtipObj.catalogVisibleId+"_close").css('visibility','visible');
																$(qtupActive).find('.qtip-tip').css('visibility','visible');
																this.currTheme = Drupal.settings.ajaxPageState.theme;
																if ($.browser.msie) {
																	if($.browser.version == 9){
																	  // Conditionally set top to 0 to fix the issue reported in ticket #0032959
																	  if (qtipObj.catalogVisibleId.indexOf('qtipAttachIdqtip') < 0 &&
																	        qtipObj.catalogVisibleId.indexOf('renderTagsId') < 0 &&
																	          qtipObj.catalogVisibleId.indexOf('renderPrintCerId') < 0 &&
																	            qtipObj.catalogVisibleId.indexOf('qtipBusinessqtip') < 0) {
																		  $('#' + qtipObj.catalogVisibleId + "_close").css('top', '0px');
																	  }
																		$(qtupActive).find('.qtip-tip').addClass('qtip-tip-ie9');
																	}
																	if($.browser.version == 10){
																		if(qtipObj.catalogVisibleId.indexOf('addclass')>=0 || qtipObj.catalogVisibleId.indexOf('editclass')>=0){
																			 $('#' + qtipObj.catalogVisibleId + "_close").css('top', '-2px');
																		}
																	}
																	if($.browser.version == 8 && (qtipObj.catalogVisibleId.indexOf('qtip_addusers_visible_disp')>0 ||
																			((qtipObj.catalogVisibleId.indexOf('Accessqtip')>0) && crTheme == 'expertusoneV2'))){
																		$('#'+qtipObj.catalogVisibleId+"_close").css('top','0px');
																	}

																	if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0){
																		//var bot = ($.browser.version==7)?-8:-67;
																		if($.browser.version == 8){
																		bottomPos = (this.currTheme == "expertusoneV2") ?-5:-31;
																		rightPos = "25px";
																		$(qtupActive).find('.qtip-tip').css('bottom',bottomPos+'px');
																		$(qtupActive).find('.qtip-tip').css('right',rightPos);
																	  }else if($.browser.version == 9 || $.browser.version == 10){
																		bottomPos = (this.currTheme == "expertusoneV2") ?-8:-68;
																		$(qtupActive).find('.qtip-tip').css('bottom',bottomPos+'px');
																	  }
																	  else if($.browser.version == 10 && this.currTheme != "expertusoneV2"){
																		bottomPos = -68;
																		$(qtupActive).find('.qtip-tip').css('bottom',bottomPos+'px');
																		$(".qtip-button .admin-bubble-close").css("top","-3px");
																	 }
													        	   }
																	if(qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_addtemplate')>0){
																		var bot = ($.browser.version==8)?-5:-8;
																		$(qtupActive).find('.qtip-tip').css('bottom',bot+'px');
																		if($.browser.version==8){
																			$('.qtip-tip').css("margin-right","6px");
																			$('.qtip-tip').css("margin-bottom","37px");
																		}
																	}
																}
																if (navigator.userAgent.indexOf("Chrome")>0) {
																	if(qtipObj.catalogVisibleId.indexOf('renderTagsId')>=0 || qtipObj.catalogVisibleId.indexOf('AttachIdqtip_visible')>0
																			|| qtipObj.catalogVisibleId.indexOf('Businessqtip_visible')>0 || qtipObj.catalogVisibleId.indexOf('renderPrintCerId') >=0){
																		$('#'+qtipObj.catalogVisibleId+"_close").css('top','-10px');
																	}
																	if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_addtemplate')>0){
																		/*--#41331 - Chorome browser qtip position fix --*/
																		bottomPos = (this.currTheme == "expertusoneV2") ? "-8px" : "-68px";
																		$(qtupActive).find('.qtip-tip').css('bottom',bottomPos);
																	}
																	if(qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0){
																		$(qtupActive).find('.qtip-tip').css('bottom','-8px');
																	}
																}
																if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_addtemplate')>0){
																	$('.bubble-blr').attr('style','width: 124px');
																}
																if(qtipObj.catalogVisibleId.indexOf('BannerAccessqtip')>0 && (Drupal.settings.ajaxPageState.theme == 'expertusoneV2')){
																	$('#'+qtipObj.catalogVisibleId+"_close").css('left','669px');
																	$(qtupActive).find('.qtip-tip').css('right','-17px');
																	if (navigator.userAgent.indexOf("Chrome")>0 || ($.browser.msie && $.browser.version == 9)) {
																		$('.bubble-blt').attr('style','width: 19px');
																		$('.bubble-blr').attr('style','width: 627px');
																	}
																}
																if(qtipObj.catalogVisibleId.indexOf('MoveUsersqtip')>0 && $.browser.msie && $.browser.version != 7){
																	$('#'+qtipObj.catalogVisibleId+"_close").css('top','0px');
																	if($.browser.version == 8){
																		$('.bubble-blt').css('width','58px');
																	}else if($.browser.version == 10 || $.browser.version == 9){
																		$('.bubble-blt').css('width','58px');
																	    $('.bubble-blm').css('width','58px');
																	    $('.bubble-blr').css('width','350px');
																	}else{
																		$('.bubble-blt').css('width','48px');
																	    $('.bubble-blr').css('width','290px');
																	}
																}
															  if(qtipObj.catalogVisibleId.indexOf('MoveUsersqtip')>0){
																  if(navigator.userAgent.indexOf("Chrome")>0 || navigator.userAgent.indexOf("Safari")!=-1){
																	$('.bubble-blt').css('width','61px');
																    $('.bubble-blr').css('width','375px');
																  }
															  }
																if(qtipObj.catalogVisibleId.indexOf('AttachIdqtip_listvalues')>0){
																	if(navigator.userAgent.indexOf("Chrome")>0){
																		$('.bubble-blt').attr('style','width: 14px');
																		$('.bubble-blr').attr('style','width: 194px');
																	}else if(navigator.userAgent.indexOf("Safari")!=-1){
																		$('.bubble-blt').attr('style','width: 17px');
																	    $('.bubble-blr').attr('style','width: 191px');
																    }
																}
																if(qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_addtemplate')>0){
																	if(navigator.userAgent.indexOf("Chrome")>0){
																		$('.bubble-blt').attr('style','width: 131px');
																	}
																	else{
																		$('.bubble-blt').attr('style','width: 164px');
																		$('.bubble-blr').attr('style','width: 362px');
																	}
																	var isAtLeastIE11 = !!(navigator.userAgent.match(/Trident/) && !navigator.userAgent.match(/MSIE/))? true : false;
																	if(this.currTheme == "expertusoneV2" && isAtLeastIE11==1 ){
																	$(".qtip-button .admin-bubble-close").css({"top":"-2px"});
																	$(qtupActive).find('.qtip-tip').css('bottom','-8px');
																	}
																}
																if(qtipObj.catalogVisibleId.indexOf('MoveUsersqtip_visible')>0 && navigator.userAgent.indexOf("Chrome")>0){
																	$('.bubble-blt').attr('style','width: 61px');
																	$('.bubble-blr').attr('style','width: 375px');
																}
																
															if(tipPosition == 'bottomRight' && mLeft!=0) {
															if(qtipObj.catalogVisibleId.indexOf('listvalpeoedit')>0){
																leftPos = "44px";
																topPos  = "-2px";
																$(qtupActive).find('.qtip-tip').css({'z-index':zIndex,'right':'-31px'});
																$('#'+qtipObj.catalogVisibleId+"_close").css({'left':leftPos,'top':topPos});
																$('.bubble-blt').css('width','184px');
																$('.bubble-blr').css('width','0');
																$('.bubble-blm').css('width','58px');
																if(navigator.userAgent.indexOf("Chrome")>0){
																$('.bubble-blt').css('width','206px')
																}else if($.browser.version == 10 || $.browser.version == 9){
																$(qtupActive).find('.qtip-tip').css("bottom","0");
																$('.bubble-blt').css('width','185px');
																}
																
															}}
																/*if((qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0) &&
																		$.browser.msie ){
																	$('#edit-subject').focus();
																}*/
																// Fix for 0014517: Issue in Class Admin while adding multiple classes - Added by Vincent July 30, 2012
																if(qtipObj.catalogVisibleId.indexOf('addclass')>=0 || qtipObj.catalogVisibleId.indexOf('editclass')>=0
																		|| (qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')) || (qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate'))){
																		
																	var tipTop = $('#'+qtipObj.catalogVisibleId).offset().top;
																	var dScrollTop = $("#modal-content").scrollTop();
																	if(tipTop<=0){
																		var toScroll=dScrollTop-Math.abs(tipTop);
																		$("#modal-content").scrollTop(toScroll);
																		bubbleTop=bubbleTop-tipTop;
																		bubbleha = $("#"+qtipObj.catalogVisibleId).css('height');
													 					bubbleha = parseInt(bubbleha.substring(0,bubbleha.length-2));
																		bubbleTop = bubbleTop-(bubbleha-bubblehb);
																		$("#"+qtipObj.catalogVisibleId).closest(".qtip-active").css('top',bubbleTop);
																	}
																}
																Drupal.attachBehaviors();
											 					bubbleha = $("#"+qtipObj.catalogVisibleId).css('height');
											 					bubbleha = parseInt(bubbleha.substring(0,bubbleha.length-2));
																bubbleTop = bubbleTop-(bubbleha-bubblehb);
																$("#"+qtipObj.catalogVisibleId).closest(".qtip-active").css('top',bubbleTop);
											 					vtip();
											 					if(data.status_message != '' && data.cloned != 0){
											 						$("#bubble-face-table #show_expertus_message").html(data.status_message);
											 					}
										   	                 }
											   			});
											 			//remove style attr border and background to add/edit qtip
											 			$('.qtip-contentWrapper,.qtip-title').each(function(){
															$(this).removeAttr("style");
															$('.qtip-wrapper,.qtip-content').css("overflow","visible");
															if($.browser.version == 8){
																$('.qtip-content').css("padding-bottom","43px");
																if(qtipObj.catalogVisibleId.indexOf('AttachIdqtip_listvalues')>0)
																$('.qtip-tip').css("margin-bottom","76px");
															}
															$('.qtip').css("padding-bottom","24px");
															$(qtupActive).find('.qtip-tip').css("margin-left","-33px");
										          	    });
										 			}
									         }
									      },
									     show:{
											when:{
													event:'click',
													target:$("#"+popupId)
												},
											ready: true // Needed to make it show on first mouseover event
											//effect:'slide'
										 },
										 hide: {
												when:{
										   			event:'',
										   			target:''
												}
										//effect:'slide'
										},
										position: {
										    corner:{
										    	   	target: 'topMiddle',
										    	   	tooltip: tipPosition
										    	  }
										},
										style: {
											width:wBubble,
											height:hBubble,
											background: 'none',
											color: '#333333',
												tip: {
										         corner: tipPosition
												}
									   }
								 });
							}
						}
					}catch(sp){
						//alert(sp.toString());
					}
				},
				getQtipDiv : function(qtipObj,isVCSession,event) {
					var xCoordinate ='';
					var yCoordinate ='';
					var popupId 	= qtipObj.popupDispId;
					var qtipLen 	= this.qtipLenth;
					var inArrQtip = $.inArray(popupId, this.qtipLoadSet);
					this.qtipLoadSet[qtipLen] = popupId;
					this.qtipLenth = this.qtipLenth+1;
					var entityType = qtipObj.entityType;
					var entityId   = qtipObj.entityId;
					var courseId = qtipObj.courseId!=undefined?qtipObj.courseId:'';
					var url 			  = resource.base_host+'?q='+qtipObj.url; // resource.base_host+'?q=administration/catalogaccess/33/cre_sys_obt_crs';
					var wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
					var hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
					var setwBubble        = wBubble-20;
					var setBtmLeft        = (setwBubble-104)/2;
					var qtipLeftPos       = (wBubble > 700) ? 400 : (setBtmLeft + 20);
					var tipPos = qtipObj.tipPosition;
					var catalogVisibleId = qtipObj.catalogVisibleId;
					var mLeft = 0;
					var tipPosition;
					var qtipTopBottomDisplay;
					var topElements,bottomElements;
					var assignHtml  = qtipObj.assignHtml;
					var mtobj=this;
					var rowVal = qtipObj.rowVal;
					var qtip_tbl_clsName = qtipObj.qtip_tbl_clsName!=undefined ? qtipObj.qtip_tbl_clsName:'';
					var loaderId = (qtipObj.sessionPopupId == null || qtipObj.sessionPopupId == '') ?  popupId :qtipObj.sessionPopupId;
					$('body').live('click', function(e){
			    		xCoordinate = e.pageX;
			    		yCoordinate = e.pageY;
			    	});
					//console.log("qtipTopBottomDisplay001: "+ qtipObj.tipPosition);
					//EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader(qtipObj.catalogVisibleId+"_disp");
					// Added by Vincent for #0021894
					EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader(loaderId);
					if(isVCSession>=0){
						var i=isVCSession;
						var timeZone = $('#time_zone_'+i).val();
						var session_meetingid = $("#session_meetingid_"+i).val();
						var session_attendeepass = $("#session_attendeepass_"+i).val();
						var session_presenterpass = $("#session_presenterpass_"+i).val();
						var session_attendeeurl = $("#session_attendeeurl_"+i).val();
						var session_presenterurl = $("#session_presenterurl_"+i).val();
						var hid_session_details_id = $("#hid_session_details_id_"+i).val();

						var formDetail = '{"timezone":"'+timeZone+
									'","session_meetingid":"'+session_meetingid+
									'","session_attendeepass":"'+session_attendeepass+
									'","session_presenterpass":"'+session_presenterpass+
									'","session_attendeeurl":"'+escape(encodeURIComponent(session_attendeeurl))+
									'","session_presenterurl":"'+escape(encodeURIComponent(session_presenterurl))+
									'","instructor_id":"'+hid_session_details_id+'"}';

						//var tt = encodeURIComponent(formDetail)
						url = url + "/"+formDetail;
					}
					if(catalogVisibleId.indexOf('addpermissions')>=0){
						$('#loaderdiv'+popupId).css('top','-80px');
					}
					if(popupId.indexOf('qtip_visible_disp_addsession_iprange')>=0)
					{
						$('#loaderdiv'+popupId).css('left','180px').css('top','-60px');
					}
					if (popupId.indexOf("exp_meeting_qtip_visible_disp_addpresenter_0_presenter") >= 0) {
						$('#loaderdiv'+popupId).css('left','250px').css('top','-78px');
					}
					$.ajax({
		 				 type: "GET",
			   	         url: url,
			   	         data:  '',
			   	     //    async: false,
			   	         success: function(data){
							//alert('X='+xCoordinate+' Y='+yCoordinate);
			   	        	 
			   	        	if(qtipObj.qtipClass != 'admin-qtip-access-parent lrn_cls_vct_web' && qtipObj.qtipClass != 'admin-qtip-presenter-access-parent') {			   	        	 
			   	        		$(".active-qtip-div").remove();
			   	        	}			   	        	
			   	        	var span_width1 = $('.admin-addanother-session-details-info').width();
			   	        	var span_width2 = $('.chosen-meeting-type').width();
							var paintHtml = topElements+"<div id='paintContent"+popupId+"'><div id='show_expertus_message'></div>"+data.render_content_main+"</div>"+bottomElements;
							var contentDiv = qtipObj.catalogVisibleId+"_disp";
							
														
							if(qtipObj.qtipClass == 'admin-qtip-access-parent lrn_cls_vct_exp' || qtipObj.qtipClass == 'admin-qtip-access-parent lrn_cls_vct_web' || qtipObj.qtipClass == 'admin-qtip-access-parent lrn_cls_vct_oth' || qtipObj.qtipClass == 'admin-qtip-presenter-access-parent') {
								var repstr = qtipObj.qtipClass;
								var replacedstr = repstr.replace(' ','-');
								$("#"+popupId).append("<div id="+replacedstr+" class='bottom-qtip-tip active-qtip-div set-wbubble-left' ></div>");
							} 
							else {
								$("#"+popupId).append("<div class='bottom-qtip-tip active-qtip-div set-wbubble-left'></div>");
							}
							
							if(qtipObj.qtipClass == 'display-message-positioning' || qtipObj.qtipClass == 'pwdstrength-admin-popup' || qtipObj.qtipClass == 'admin-qtip-presenter-access-parent' || qtipObj.qtipClass == 'admin-qtip-access-parent lrn_cls_vct_exp' || qtipObj.qtipClass == 'admin-qtip-access-parent lrn_cls_vct_web' || qtipObj.qtipClass == 'admin-qtip-access-parent lrn_cls_vct_oth') {
								$("#"+popupId).append("<div id='"+contentDiv+"' class='active-qtip-div "+qtipObj.qtipClass+"'></div>");
							}else{
								$("#"+popupId).append("<div id='"+contentDiv+"' class='active-qtip-div'></div>");
							}
							$("#"+contentDiv).html(paintHtml);
							if(catalogVisibleId.indexOf('addroletousers')>=0){
								$("#paintContent"+popupId).css('min-height','225px');
							}
							$(".bottom-qtip-tip").css('bottom','0px').css('position','absolute').css('z-index','101');
							//$('#upload_browse_and_url').find('#search-list-title-keyword').css('z-index','0');
							//$('#upload_detail_container').find('.content-browse-upload').css('z-index','0');
							if (qtipObj.qtipClass == 'add-key-word-popup') {
								$("#"+contentDiv).css('position','absolute').css('left','-'+parseInt(qtipLeftPos-98)+'px').css('bottom','40px').css('z-index','100');
							}
							else if (qtipObj.qtipClass == 'pwdstrength-admin-popup') {
							  $("#"+contentDiv).css('position','absolute').css('left','-'+parseInt(qtipLeftPos-48)+'px').css('bottom','40px').css('z-index','100');
							}
							else if (catalogVisibleId.indexOf("1_qtip_visible_disp_addsession_iprange")==0) {
								$("#"+contentDiv).css('position','absolute').css('left',parseInt(qtipLeftPos)-133+'px').css('bottom','48px').css('z-index','100');
							}
							else if (catalogVisibleId.indexOf("2_qtip_visible_disp_addsession_iprange")==0) {
								$("#"+contentDiv).css('position','absolute').css('left','-6px').css('bottom','30px').css('z-index','100');
							}
							else if (catalogVisibleId.indexOf("qtip_visible_disp_iprange_1_iprange_disp")==0) {
								$("#"+contentDiv).css('position','absolute').css('left',parseInt(qtipLeftPos-75)+'px').css('bottom','40px').css('z-index','100');
							}
							else if (catalogVisibleId.indexOf("EditSessionIdqtip") < 0) {
								$("#"+contentDiv).css('position','absolute').css('left','-'+qtipLeftPos+'px').css('bottom','40px').css('z-index','100');
							}
							var p = $("#"+popupId);
							var position = p.position();
							var divHeight = $("#"+contentDiv).height();
		 					var parentTopPos = position.top + 130;
		 					//alert("top Pos: "+parentTopPos+" | divHeight: "+divHeight);
		 					if($("#"+popupId+" .tab-title").width() > 0 ){
								 var labelWidth = ($("#"+popupId+" .tab-title").width())/2;
								 var bubbleWidth = 32;
								 var setbubblePosition = labelWidth - bubbleWidth;
								 $(".set-wbubble-left").css('left', setbubblePosition +'px');
						    }
		 					if(catalogVisibleId.indexOf("MoreEditSession")>0||catalogVisibleId.indexOf("EditSessionIdqtip")>0){
		 						$('.session-add-list').hide();
		 					}

		 				   var dispop=$('.add_session_popup').css('display');
		 				   var SRecrowCount = $('#admin-data-grid').find('.admin-datagrid-session-with-addanother .ui-jqgrid-bdiv tr').length;
		 				   var sessionScrId=$('#admin-data-grid').parent('div').attr("class");

		 				   if (parentTopPos < divHeight && position.top!=0 || catalogVisibleId.indexOf("AddMoreSessionIdqtip")>0)  {
		 					  //Add another session qtip working code
		 					  if(sessionScrId=="catalog-class-session-basic-addedit-form-container" && SRecrowCount > 5){
		 							qtipTopBottomDisplay = "tipfaceUp";
									if(catalogVisibleId.indexOf("EditSessionIdqtip") < 0);
									$("#"+contentDiv).css('position','absolute').css('left','-'+qtipLeftPos+'px').css('top','42px').css('bottom','0px').css('z-index','100');

		 					  }else{
		 					   if ($.browser.msie && $.browser.version == 7) {
									qtipTopBottomDisplay = "tipfaceUp";
									$(".narrow-search-multi-action-container div").css("position"," ");
									$(".active-qtip-div").remove();
									$("body").append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
									$("body").append("<div id='"+contentDiv+"' class='active-qtip-div'></div>");
									$("#"+contentDiv).prev().css('top',+yCoordinate+'px').css('position','absolute').css('left',+(xCoordinate-30)+'px').css('z-index','101');
									$(".bottom-qtip-tip").remove();
									if(catalogVisibleId.indexOf("EditSessionIdqtip") < 0)
										$("#"+contentDiv).css('top',+(yCoordinate+29)+'px').css('position','absolute').css('left',+(xCoordinate-setBtmLeft)+'px').css('z-index','100');
		 						} else {
		 							qtipTopBottomDisplay = "tipfaceUp";
									$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
									//add script for qtip arrow come twoice in add another session
									 if(dispop=="block") {
			 						 $("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('position','absolute');
			 						 $("#"+popupId).last('div').prev('.bottom-qtip-tip-up').remove();
			 					        } else	{
										 $("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('position','absolute').css('left','-10px');//add minus 10px for tipfaceup qtip
									 }
									$(".bottom-qtip-tip").remove();
									if(catalogVisibleId.indexOf("EditSessionIdqtip") < 0);
									$("#"+contentDiv).css('position','absolute').css('left','-'+qtipLeftPos+'px').css('top','42px').css('bottom','0px').css('z-index','100');
								}
		 					}
		 				  }
		 					if( qtipObj.tipPosition == 'tipTopLeft'){
		 						qtipTopBottomDisplay = "tipfaceUp";
								//Add another session qtip working code
								if(sessionScrId=="catalog-class-session-basic-addedit-form-container" && SRecrowCount > 5){
								    $(".bottom-qtip-tip").remove();
									$("#"+popupId).append("<div class='bottom-qtip-tip active-qtip-div set-wbubble-left'></div>");
								    $("#"+popupId+" .bottom-qtip-tip").css('bottom','2px').css('left','55').css('z-index','101').css('position','absolute');
									//$("#"+popupId+" .active-qtip-div").css('bottom','30px');
							    	$("#"+popupId+" .active-qtip-div").css('left','55px'); /*viswanathan changed 76px to 55px for #74374*/
									$("#"+contentDiv).css('position','absolute').css('left','-19px').css('top','').css('bottom','40px').css('z-index','100');
								}
								else{
								    $("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
								    
								    
								    if(qtipObj.qtipClass == 'admin-qtip-access-parent another-lrn_cls_vct_web' || qtipObj.qtipClass == 'admin-qtip-access-parent another-lrn_cls_vct_oth' || qtipObj.qtipClass == 'admin-qtip-access-parent another-lrn_cls_vct_exp') {
								    var repstr = qtipObj.qtipClass;
								    var replacedstr = repstr.split(' ');
									$("#"+popupId+" .active-qtip-div").addClass(replacedstr[1]);
								    
								    	if ((navigator.userAgent.toLowerCase().indexOf('chrome') > -1) || ($.browser.msie && $.browser.version < 8)) {
								    		$("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('position','absolute').css('left',parseInt((span_width1+span_width2)-13)+'px');
								    	} else {
								    		$("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('position','absolute').css('left',parseInt((span_width1+span_width2)-9)+'px');
								    	}
								    } else {
								    	$("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('position','absolute');
								    }
								    
								    
								    $(".bottom-qtip-tip").remove();
								    $("#"+contentDiv).css('position','absolute').css('left','-'+19+'px').css('top','42px').css('bottom','0px').css('z-index','100');
								}
						    } else if(qtipObj.tipPosition == 'tipTopRight') {

						    	$("#"+popupId+" .bottom-qtip-tip-up").css('left','-20px');
						    	if(catalogVisibleId.indexOf("EditSessionIdqtip") >0){
						    		if(catalogVisibleId.indexOf("qtip_EditSessionIdqtipvisible_disp_iprange")==0)
				    				{
				    				$("#"+contentDiv).css('position','absolute').css('left','').css('right','-'+34+'px').css('top','').css('bottom','-'+12+'px').css('z-index','100');
				    				$("#"+popupId+" .bottom-qtip-tip").css('bottom','').css('left','-26px').css('z-index','101').css('position','absolute');
				    				}
						    		else
						    		{
						    		//edit and more session qtip working code
						    		if(rowVal>4){
						    			$("#"+popupId+" .bottom-qtip-tip").css('bottom','2px').css('left','-26px').css('z-index','101').css('position','absolute');
						    			$("#"+contentDiv).css('position','absolute').css('left','').css('right','-'+34+'px').css('top','').css('bottom','40px').css('z-index','100');
						    		}else{
						    			$(".bottom-qtip-tip").remove();
						    			$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
						    			$("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('left','-26px').css('position','absolute');
						    			$("#"+contentDiv).css('position','absolute').css('left','').css('right','-'+34+'px').css('top','42px').css('bottom','0px').css('z-index','100');
						    			
						    	    	}
						    		 }
						    	}else{
						    	  $("#"+contentDiv).css('position','absolute').css('left','-'+375+'px').css('top','42px').css('bottom','0px').css('z-index','100');
						    	}
						    }else if(qtipObj.tipPosition == 'tipfaceMiddleRight' && catalogVisibleId.indexOf("AddNewSessionIdqtip") >0 ) {
						       	var getSessTop=$("#"+catalogVisibleId+"_disp").offset();
						       	if(parseInt(getSessTop.top) < "30"){
						    		qtipTopBottomDisplay = "tipfaceUp";
						    		$(".bottom-qtip-tip").remove();
					    			$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
					    			$("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('left','300px').css('position','absolute');
					    			$("#"+contentDiv).css('position','absolute').css('left','0').css('right','').css('top','42px').css('bottom','0px').css('z-index','100');
					       			//Drupal.ajax.prototype.commands.CtoolsModalAdjust();
					    			
						    	}else{
						    	qtipTopBottomDisplay = "tipfaceUp";
						    	$("#"+popupId+" .active-qtip-div").css('bottom','32px');
						    	if (Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
						    		$("#"+popupId+" .active-qtip-div").css('left','319px');
						    		$("#"+contentDiv).css('position','absolute').css('left','0').css('bottom','70px').css('z-index','auto');
						    	} else{
						    		$("#"+popupId+" .active-qtip-div").css('left','210px');
						    		$("#"+contentDiv).css('position','absolute').css('left','0').css('bottom','70px').css('z-index','unset');
						    	}
						    	}
						    }
							switch(qtipTopBottomDisplay) {
							case 'tipfaceUp':
								wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
								hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
								setwBubble        = wBubble-20;
								setBtmLeft        = (setwBubble-104)/2;
								mLeft			  = (qtipObj.catalogVisibleId.indexOf('editclass')>0)?45:(qtipObj.catalogVisibleId.indexOf('AttachCrsIdqtip')>0)?58:82;
								//Set buble tool tip arrow position
								tipPosition       = (qtipObj.tipPosition) ? qtipObj.tipPosition : 'bottomMiddle';
								topElements = '<a class="qtip-close-button" onclick="$(\'#root-admin\').data(\'narrowsearch\').removeActiveQtip(\''+catalogVisibleId+'\');"></a><span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table" class="'+qtip_tbl_clsName+'"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
								bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-blt"></td><td class="bubble-br"></td></tr></tbody></table></span>';
								$("#"+popupId).parent().parent().parent().parent().removeClass("qtip-access-control");
							break;
							default:
								wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
								hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
								setwBubble        = wBubble-20;
								setBtmLeft        = (setwBubble-104)/2;
								//Set buble tool tip arrow position
								tipPosition       = (qtipObj.tipPosition) ? qtipObj.tipPosition : 'bottomMiddle';
								topElements = '<a class="qtip-close-button" onclick="$(\'#root-admin\').data(\'narrowsearch\').removeActiveQtip(\''+catalogVisibleId+'\');"></a><span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table" class="'+qtip_tbl_clsName+'"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
								bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-blt"></td><td class="bubble-br"></td></tr></tbody></table></span>';
								$("#"+popupId).parent().parent().parent().parent().removeClass("qtip-access-control");
							}
							var paintHtml = topElements +
                              "<div id='paintContent" + popupId + "'>" +
                                "<div id='show_expertus_message'></div>" +
                                data.render_content_main +
                              "</div>" +
                              bottomElements;
							var contentDiv = qtipObj.catalogVisibleId+"_disp";
              				if(data.drupal_settings) {
							   $.extend(true, Drupal.settings, data.drupal_settings);
							}
              				EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader(loaderId);
							$("#"+contentDiv).html(paintHtml);
							//EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader(qtipObj.catalogVisibleId+"_disp");

							//For getting default meeting details on VC session details page
							if(assignHtml == 'meeting-container'){
								mtobj.getMeetingType();
							}
							Drupal.attachBehaviors();
							if(catalogVisibleId.indexOf('addpermissions')>=0 || catalogVisibleId.indexOf('addlistpermissions')>=0){
								$('#admin-add-scroll').jScrollPane({});
								/*if(catalogVisibleId.indexOf('addpermissions')>=0){
									$('#permission-cancel-save-btn').css('margin-right','-4px')
								}else{
									$('#permission-cancel-save-btn').css('margin-right','18px')
								}*/
							} else if(catalogVisibleId.indexOf('qtipAttachIdqtip_visible_disp')>=0){
								var heigt = $("#scrolldiv").height();
								if(heigt > 80){
									$('#catalog-attachment-disp-container #scrolldiv').css('height','100px');
									$('#catalog-attachment-disp-container #scrolldiv').jScrollPane({});
									}
								}
							if(catalogVisibleId.indexOf('qtipAddSessionIdqtip_visible_disp')>=0){
								$(".bottom-qtip-tip").css('bottom','2px').css('left','0').css('position','absolute').css('z-index','101');
							}else if (catalogVisibleId.indexOf("1_qtip_visible_disp_addsession_iprange")==0) {
								$(".bottom-qtip-tip").css('bottom','20px').css('left','195px').css('position','absolute').css('z-index','101');
							}else if (catalogVisibleId.indexOf("2_qtip_visible_disp_addsession_iprange")==0) {
								$(".bottom-qtip-tip").css('bottom','2px').css('left','0').css('position','absolute').css('z-index','101');
							}
							} //end $.ajax success

						});
                vtip();
				},
				visiblePopup : function(qtipObj) {
					try{
						var url = resource.base_host+'?q='+qtipObj.url;
						var popupId 	= qtipObj.popupDispId;
						//var catalogVisibleId = qtipObj.catalogVisibleId;
						var entId = qtipObj.entityId;
						var qtipScrollId = qtipObj.scrollid;

						$.ajax({
			 				 type: "GET",
				   	         url: url,
				   	         data:  '',
				   	         success: function(data){
			 					//var paintHtml = bpTop+"<div id='paintContent"+popupDispId+"'><div id='show_expertus_message' style='"+messageStyle+"'></div>"+data.render_content_main+"</div>"+bpBot;
			 					$('#'+popupId+' #visible-popup-'+entId+' #bubble-face-table .bubble-c #paintContent'+popupId).html(data.render_content_main);
			 					$.extend(true, Drupal.settings, data.drupal_settings);
			 					EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("paintContent"+popupId);

			 					if(qtipScrollId != '' && qtipScrollId != undefined && qtipScrollId != null){
			 						var qtipScrollType = (qtipObj.scrolltype == 'class') ? '.' : '#';
			 						$('#'+popupId+' '+qtipScrollType+qtipScrollId).jScrollPane('destroy');
			 						$('#'+popupId+' '+qtipScrollType+qtipScrollId).jScrollPane({});
			 					}
								Drupal.attachBehaviors();
			 					vtip();
			 					
			 					if('[qtipObj.linkid^=visible-sharelink]') {
			 						if (navigator.appVersion.indexOf("Safari")!= -1 && ($(window).height() < 742))
			 							$('#program-tp-basic-addedit-form-container .survey-attach-grid-wrapper .ui-jqgrid .jqgrow #bubble-face-table td.bubble-c').css('height','33px');
			 								 						
									if(Drupal.settings.user.language !== undefined && $.inArray(Drupal.settings.user.language, ["pt-pt", "de", "fr"]) != -1) {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-30px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-15px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ru') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-33px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-15px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ja') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-30px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-15px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ko') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-26px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-3px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'zh-hans') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-54px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-32px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-42px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it') { 
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-3px');
										else
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'es') { 
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'en-us') { 
										if ((navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1) || (navigator.appVersion.indexOf("Trident/7.0")!= -1))
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-3px');
										else
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									}else {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-28px');
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									}
								}
		   	                 }
			   			});

					}catch(e){
						alert(e);
					}

				},
				timepickerLoad : function(){
				try{
					$('input.exp-timepicker').click(function(){
						var attributeId = $(this).attr('id');
						var expDropDownId = 'exp-dropdown-'+attributeId;
						var c = $(this).attr('class');
						var x= c.indexOf('callback')>=0?c.substring(c.indexOf('callback')):'';
						var y = x.substring(x.indexOf('-')+1,(x.indexOf(' ')>0?x.indexOf(' '):x.length));
						var hours, minutes;
						$('.exp-timepicker-selection').css('display', 'none');
						if($(this).val() == Drupal.t('hh:mm')){
							$(this).val('');
						}
						if($('#'+expDropDownId).length >0){
							if($('#'+expDropDownId).is(':visible') == true){
								$('#'+expDropDownId).css('display', 'none');
							} else {
					 			$('#'+expDropDownId).css('display', 'block');
							}
						} else {
							$(this).after('<div id="'+expDropDownId+'" data="'+attributeId+'" class="exp-timepicker-selection"><ul></ul></div>');
							for(var i = 0 ;i <= 1425; i += 15){
								hours = Math.floor(i / 60);
								minutes = i % 60;
								if (hours < 10){
								    hours = '0' + hours;
								}
								if (minutes < 10){
								    minutes = '0' + minutes;
								}
								$('#'+expDropDownId+' ul').append('<li>'+hours + ':' + minutes + '</li>');
						    }
							$('div.exp-timepicker-selection li').click(function(){
								var attributeId = $(this).closest('.exp-timepicker-selection').attr('data');
								var selectedTime = $(this).html();
								$('#'+attributeId).val(selectedTime);
								$(this).closest('.exp-timepicker-selection').css('display', 'none');
								if(y!=null && y!=undefined && y!='undefined' && $.trim(y) !=''){
									eval(y+'('+attributeId+')');
								}
							});
						}
					});
					
  			$('body,#admin-data-grid td').bind("click",function(event){
                  if($(event.target).hasClass('exp-timepicker') != true){
                   $('.exp-timepicker-selection').css('display', 'none');
                  }	
				  if($('#exp-dropdown-start_hours').is(':visible')) {
					var Index = $('#exp-dropdown-start_hours').parents('.active-qtip-div').siblings('.bottom-qtip-tip').zIndex();
				    $('#exp-dropdown-start_hours').parents('.active-qtip-div').siblings('.bottom-qtip-tip').zIndex(100);
				  } else {
					var Index = $('#exp-dropdown-start_hours').parents('.active-qtip-div').siblings('.bottom-qtip-tip').zIndex();
				    $('#exp-dropdown-start_hours').parents('.active-qtip-div').siblings('.bottom-qtip-tip').zIndex(101);
				  }
               });
               
           }catch(e){
			 alert(e);
			}
                  
				},
                /* added function to randomize li tags while switching between meetings during session creation */
				reverseSessionType : function(){
					if($(".session-list-ul li").length == 1 )
						{
							$(".session-list-ul li").addClass("single_meeting");
						}
					$('.session_type_list').click(function(){
						var liId = $(this).attr("id");
					    $('.session-list-ul li:eq(0)').removeClass('admin-save-button-middle-bg');
					    $('.session-list-ul li:eq(0)').text($('.session-list-ul li:eq(0)').text().replace(Drupal.t("LBL287"), ''));
						$('.session-list-ul span.session-list').append($('.session-list-ul li:eq(0)'));
						$('#'+liId).text(Drupal.t('LBL287')+' '+$('#'+liId).text());
						$('#'+liId).addClass('admin-save-button-middle-bg');
						$('.session-list-ul span.admin-save-button-left-bg').after($(this));
						$('.session-list-ul span.session-list').hide();
						if(document.getElementById('lrn_cls_vct_oth') && liId != "lrn_cls_vct_oth"){
							$('.session-list-ul span.session-list').append($('#lrn_cls_vct_oth'));
						}

						if(liId =='lrn_cls_vct_web') {
							$('.admin-empty-text-msg').text(Drupal.t('MSG756'));
							$('.expertus-meeting-button-container div.add_session_button_div div.dropdownadd-dd-list-arrow').css('left','95px');
						} else if(liId =='lrn_cls_vct_oth') {
							$('.admin-empty-text-msg').text(Drupal.t('MSG757'));
							$('.expertus-meeting-button-container div.add_session_button_div div.dropdownadd-dd-list-arrow').css('left','95px');
						} else {
							$('.admin-empty-text-msg').text(Drupal.t('MSG800'));
							$('.expertus-meeting-button-container div.add_session_button_div div.dropdownadd-dd-list-arrow').css('left','130px');
						}
			
						vtip();
						//$(".session-list-ul li:not(#"+liId+")").randomize();
					});

				},

				removeActiveQtip: function(id) {
				  // Handle cancel policy qtip popup close
				  var cancelPolicyQtip = $('.active-qtip-div').parents('.cancel-policy-popup-wrapper');
				  if (cancelPolicyQtip) {
				    if(id=='qtip_visible_disp_addpresenter_0_presenter') {
						  $('#exp_meeting_qtip_visible_disp_addpresenter_0_presenter').empty();
					 } else {
						  $('.active-qtip-div').remove();
					 }
				    if (cancelPolicyQtip.hasClass('add-another-cancel-policy-popup')) {
  				    //alert('closing cancel policy add another qtip popup');
  				    var defaultLabel = $('#add-another-cancel-policy-label').data('default-label');
  				    $('#add-another-cancel-policy-label').text(defaultLabel);
				    }
				    return;
				  }

				  $('.active-qtip-div').remove();
					$(".narrow-search-multi-action-container div").css("position","relative");
					if(id!=null){
						if(id.indexOf('AddMoreSessionId')>=0){
							$('#meeting-title').html('');
						}else if(id.indexOf('addroletousers')>=0){
							$("#narrow-search-results-holder").trigger("reloadGrid",[{page:1}]);

						}
					}
				},

				getInlineEdit : function(inlineData){
                    if(inlineData){
                    	this.actualInlineData = inlineData;
                        var inlineEditData= eval(inlineData);
                        this.inlineEditData = inlineEditData;
                        var html ="";
                        var callBlur = '$("#root-admin").data("narrowsearch")';
                        var widthAnchor = $("#tag-name-div-"+this.inlineEditData.tagEntityId+" a").width();
                        var heightAnchor = $("#tag-name-div-"+this.inlineEditData.tagEntityId+" a").height();
                        //html += '<input type ="text" class="tag-edit-txtbox" id="tag-list-name-id-'+this.inlineEditData.tagEntityId+'" style = "width:'+widthAnchor+'px;height:'+heightAnchor+'px;" value ="'+decodeURIComponent(this.inlineEditData.tagName)+'" onblur="$(\'#root-admin\').data(\'narrowsearch\').updateTagList(this);" onkeydown="$(\'#root-admin\').data(\'narrowsearch\').updateTagsOnKeyDown(event,this);" />';
                        html += '<input type ="text" maxlength="150" class="tag-edit-txtbox" id="tag-list-name-id-'+this.inlineEditData.tagEntityId+'" style = "width:'+widthAnchor+'px;height:'+heightAnchor+'px;" value ="" onblur="$(\'#root-admin\').data(\'narrowsearch\').updateTagList(this);" onkeydown="$(\'#root-admin\').data(\'narrowsearch\').updateTagsOnKeyDown(event,this);" />';
                        $("#tag-name-div-"+this.inlineEditData.tagEntityId).html(html);
    					$('#tag-list-name-id-'+this.inlineEditData.tagEntityId).focus();
    					$("#tag-list-name-id-"+this.inlineEditData.tagEntityId).val(decodeURIComponent(this.inlineEditData.tagName));
                    }
                },

                updateTagsOnKeyDown : function(evt,updateTag){

            		evt = evt || window.event;
            	    var charCode = evt.keyCode || evt.which;
            	    if (charCode == 13) {
            	    	updateTag.blur();
            	        evt.preventDefault();
            	        evt.stopPropagation();
            	        return false;
            	    }
            	},

                updateTagList : function(updatedTag){
            		var obj = this;
            		this.actualInlineData = this.actualInlineData.replace(/"/g,"&quot;");
                    var inlineData = this.inlineEditData;
                    var updatedTagName = $(updatedTag).attr("value");
                    var exist_tag_name = decodeURIComponent(inlineData.tagName);
//                    var allowedChars = /^[a-z A-Z0-9@*.,_\-\'\|]+$/;
//                    var restrictedSet = /[~`!#$%^&()+=\\}\]{\[\":;?\/><]/;	//for 34642: Handling of multi-language tags in cloud based on learner's preferred language
//                    if(restrictedSet.test(updatedTagName)) {
//                    	var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li style="width: 250px;"><span>' +Drupal.t('LBL191') + ' ' + Drupal.t('MSG639') + '</span></li></ul><div onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();" class="msg-close-btn"></div></div></div></div>';
//                    	$("#show_expertus_message").append(error_msg);
//                    	return false;
//                    }
                    if(updatedTagName != '' && $.trim(updatedTagName) != "" && updatedTagName != exist_tag_name){
                    	newupdatedTagName = updatedTagName.replace(/\//g,Drupal.settings.custom.EXP_AC_SEPARATOR);
	                    url = obj.constructUrl("administration/update-tags/"+inlineData.entityId+"/"+inlineData.emptyId+"/"+inlineData.entityType+"/"+encodeURIComponent(newupdatedTagName)+"/"+inlineData.tagEntityId);
	                    $.ajax({
	                        type: "POST",
	                        url: url,
	                        data:  '',
	                        success: function(result){	                        	
	                        	if(result == 'error'){
	                        		var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li><span>' +Drupal.t('LBL191') + ' ' + Drupal.t('LBL271') + '</span></li></ul><div onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();" class="msg-close-btn"></div></div></div></div>';
                    				$("#paintContentVisiblePopup #show_expertus_message").append(error_msg);
                    				return false;
	                        	}else{                  		
		                            obj.inlineEditData.tagName = encodeURIComponent(updatedTagName); //'data={"entityId":"'+inlineData.entityId+'","entityType":"'+inlineData.entityType+'","tagName":"'+updatedTagName+'","tagEntityId":"'+inlineData.tagEntityId+'","tagId":"'+inlineData.tagId+'"}';
		                            //obj.inlineEditData = obj.inlineEditData.replace('"',"&quot;");
		                            var html = "";
		                            obj.actualInlineData = obj.actualInlineData.replace(/&quot;/g,'"');
		                            obj.actualInlineData = eval(obj.actualInlineData);
		                            obj.actualInlineData.tagName = escape(updatedTagName);
		                            obj.actualInlineData = "data= "+EXPERTUS_SMARTPORTAL_AbstractManager.convertJsonToString(obj.actualInlineData);
		                            obj.actualInlineData = obj.actualInlineData.replace(/"/g,"&quot;");
		                            html += '<a href="javascript:void(0);" id="tag-list-name-id-'+inlineData.tagEntityId+'" name="tag-list-name-id-'+inlineData.tagEntityId+'" class = "tag-list-name"  ondblclick ="$(\'#root-admin\').data(\'narrowsearch\').getInlineEdit(\''+ obj.actualInlineData +'\');">'+updatedTagName+'</a>';
		                            $("#tag-name-div-"+inlineData.tagEntityId).html(html);
	                           }
	                        }
	                    });
                    }else{
                    	var html = "";
                    	obj.actualInlineData= addslashes(obj.actualInlineData);
                    	obj.actualInlineData.tagName = updatedTagName;
                    	obj.actualInlineData = obj.actualInlineData.replace(/"/g,"&quot;");
                        html += '<a href="javascript:void(0);" id="tag-list-name-id-'+inlineData.tagEntityId+'" name="tag-list-name-id-'+inlineData.tagEntityId+'" class = "tag-list-name"  ondblclick ="$(\'#root-admin\').data(\'narrowsearch\').getInlineEdit(\''+ obj.actualInlineData+'\');">'+exist_tag_name+'</a>';
                        $("#tag-name-div-"+inlineData.tagEntityId).html(html);
                    }
                },


                getInlineEditAttachedCourse : function(inlineData,e){
                	//e.preventDefault();
                	//console.log("double click trigger ");
                    if(inlineData){
                        var inlineEditData= eval(inlineData);
                        this.inlineEditData = inlineEditData;
                    }
                   
                   //0073705:single and double quotes
                    var fullGroupNameData = this.inlineEditData.fullGroupName;
                    fullGroupNameData = fullGroupNameData.replace(/"/g, '&quot;');


          
                    tpTabDoubleClickData = {
                    		'selector':"#attachedcourse-name-div-"+this.inlineEditData.courseId+'-'+this.inlineEditData.recertifyFlag,
                    		'data':$("#attachedcourse-name-div-"+this.inlineEditData.courseId+'-'+this.inlineEditData.recertifyFlag).html(),
                    		'oldName': $("#attachedcourse-name-div-"+this.inlineEditData.courseId+'-'+this.inlineEditData.recertifyFlag + " a span").text()
                    } 
                    var html ="";
                    var callBlur = '$("#root-admin").data("narrowsearch")';
                    var inline_txt = $("#attachedcourse-name-div-"+this.inlineEditData.courseId+"-"+this.inlineEditData.recertifyFlag+" #expires_duration").html();
                    html += '<input maxlength="250" type ="text" class="attachedcourse-edit-txtbox" id="attachedcourse-list-name-id-'+this.inlineEditData.courseId+'-'+this.inlineEditData.recertifyFlag+'" value ="'+fullGroupNameData+'" onkeydown="$(\'#root-admin\').data(\'narrowsearch\').updateAttachedCourseGroupKeyDown(event, this);" onblur="$(\'#root-admin\').data(\'narrowsearch\').updateAttachedCourseGroup(this);">';
                    html += '<span id ="expires_duration">'+inline_txt+'</span>';
                    $("#attachedcourse-name-div-"+this.inlineEditData.courseId+'-'+this.inlineEditData.recertifyFlag).html(html);
                    $('#attachedcourse-list-name-id-'+this.inlineEditData.courseId+'-'+this.inlineEditData.recertifyFlag).focus();
                },

                updateAttachedCourseGroupKeyDown : function(evt, updatedGroup){
            		evt = evt || window.event;
            	    var charCode = evt.keyCode || evt.which;
            	    if (charCode == 13) {
            	    	$("#root-admin").data("narrowsearch").updateAttachedCourseGroup(updatedGroup);
            	        evt.preventDefault();
            	        return false;
            	    }

                },

                updateAttachedCourseGroup : function(updatedGroup){
                    var inlineData = this.inlineEditData;
                    var updatedGroupName = $.trim($(updatedGroup).attr("value"));
                    var oldGroupName = this.inlineEditData.groupName;
                    this.inlineEditData.groupName = updatedGroupName;
                    var obj = this;
                    var fieldname = Drupal.t("LBL1003")+' '+Drupal.t("LBL3060");
                    var characterlist = "a-z A-Z 0-9 @ * ' . _ -";
                    var grpNameExists = 0;
                       /**Change by: ayyappans
                     * #37078: UI Issue in TP when adding any special characters in the group name
                     * Root Cause: If the group name contains special characters " and ' the object string gets broken and causes javascript error.
                     * Fix: validation has been added for Group name to contain alphanumerics, spaces and speacial characters ._-@*
                     * 		maxlength characters property added to textbox attachedcourse-list-name-id-?
                     * */
                   if(Drupal.settings.user.language == 'en-us')
                	   {
                	   var allowedChars = /^[a-z A-Z0-9@*\'\"._-]+$/;
                	   if ( updatedGroupName == '' || !allowedChars.test(updatedGroupName)) {
                           var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li style="width: 200px;"><span>' +  Drupal.t('MSG811', {'@fieldname' : fieldname , '@character list' : characterlist }) + "</span></li></ul><div onclick=\"$('body').data('learningcore').closeErrorMessage();\" class=\"msg-close-btn\"></div></div></div></div>";
                           $("#show_expertus_message").html(error_msg);
                           return false;
                         }
                	   }
                   else {
                	   var allowedChars = /^[a-z A-Z0-9@*\'\"._-]+$/;;
                	   if (updatedGroupName == '' || !allowedChars.test(updatedGroupName)) {
                           var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li style="width: 200px;"><span>' +  Drupal.t('MSG811', {'@fieldname' : fieldname , '@character list' : characterlist }) + "</span></li></ul><div onclick=\"$('body').data('learningcore').closeErrorMessage();\" class=\"msg-close-btn\"></div></div></div></div>";
                           $("#show_expertus_message").html(error_msg);
                           return false;
                       }
                   }
                   if(updatedGroupName.length > 49){
                   		var current_length = updatedGroupName.length;
                   		 var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li style="width: 200px;"><span>' +  Drupal.t('!name cannot be longer than %max characters but is currently %length characters long.', {'!name' : fieldname , '%max' : 50 ,'%length' : current_length}) + "</span></li></ul><div onclick=\"$('body').data('learningcore').closeErrorMessage();\" class=\"msg-close-btn\"></div></div></div></div>";
                          $("#show_expertus_message").html(error_msg);
                          return false;
                   }
                   //Group name validation
                   $('#program_attach_tabs ul li a').each(function(i) {
                   		var validationmsg = Drupal.t("LBL1003")+' '+Drupal.t("LBL107")+Drupal.t("LBL271");
				    	if (this.text == updatedGroupName) {
				    	 	grpNameExists = 1;
				    	}
					});
					if(grpNameExists == 1){
						var validationmsg = Drupal.t("LBL1003")+' '+Drupal.t("LBL107")+Drupal.t("LBL271");
						var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li style="width: 200px;"><span>' +  validationmsg + "</span></li></ul><div onclick=\"$('body').data('learningcore').closeErrorMessage();\" class=\"msg-close-btn\"></div></div></div></div>";
                        $("#show_expertus_message").html(error_msg);
                        return false;
				 	}
					
                   var groupName = decodeURIComponent(updatedGroupName);
                    //37078 : End of code change
                    if (updatedGroupName == "No Group Name" || updatedGroupName == "") {
                        updatedGroupName = oldGroupName;
                        this.inlineEditData.groupName = updatedGroupName;
                        full_grp_name = addslashes(inlineData.fullGroupName);
                        group_name = addslashes(inlineData.groupName);
                        var html = "";
                        var datanew = 'data={"entityId":"'+inlineData.entityId+'","entityType":"'+inlineData.entityType+'","groupName":"'+group_name+'","fullGroupName":"'+full_grp_name+'","courseId":"'+inlineData.courseId+'","recertifyFlag":"'+inlineData.recertifyFlag+'"}';
                        datanew = unescape(datanew).replace(/"/g, '&quot;');
                        html += '<a href="javascript:void(0);" id="attachedcourse-list-name-id-'+inlineData.courseId+'-'+inlineData.recertifyFlag+'" name="attachedcourse-list-name-id-'+inlineData.courseId+'-'+inlineData.recertifyFlag+'" class = "attachedcourse-list-name"  ondblclick ="$(\'#root-admin\').data(\'narrowsearch\').getInlineEditAttachedCourse(\''+datanew+'\',this);">'+updatedGroupName+'</a>';
                        $("#attachedcourse-name-div-"+inlineData.courseId+'-'+inlineData.recertifyFlag).html(html);
                    } else {
                    	obj.createLoader('attach_course_dt');
                        url = obj.constructUrl("administration/update-attachedcourse-group/"+inlineData.entityId+"/"+inlineData.entityType+"/"+updatedGroupName+"/"+inlineData.courseId+"/"+inlineData.recertifyFlag);
                        $.ajax({
                            type: "POST",
                            url: url,
                            data:  '',
                            success: function(result) {
	                        	tpTabDoubleClick = false;
	                        	if(tpTabDoubleClickData != null){
	                        		var oldData = tpTabDoubleClickData.data;
	                        		oldGroupName = tpTabDoubleClickData.oldName;

						formatOldGroupName=oldGroupName;
						formatOldGroupName = formatOldGroupName.replace(/"/g,"&quot;");
						oldData=oldData.replace(/\\/g, '');
						formatUpdatedGroupName=updatedGroupName;
						formatUpdatedGroupName = formatUpdatedGroupName.replace(/"/g,"\\&quot;");
						oldData = oldData.replace( new RegExp(formatOldGroupName, "gi"), formatUpdatedGroupName);

	                        		var clk = $(oldData + ' a').attr('onclick');
	                        		var dblclk = $(oldData + ' a').attr('ondblclick');
		                        	$(tpTabDoubleClickData.selector).html(oldData);
						//var acText = $(tpTabDoubleClickData.selector +" span:first").text();
						$(tpTabDoubleClickData.selector +" span:first").text(updatedGroupName);
		                        	$("#attachedcourse-name-div-"+inlineData.courseId+'-'+inlineData.recertifyFlag).attr('title',updatedGroupName);
		                        	$( "#page-container-tabs-prg" ).tabs('destroy');
		            				$( "#page-container-tabs-prg" ).tabs({});
		                        	tpTabDoubleClickData = null;
	                        	}
	                        	obj.destroyLoader('attach_course_dt');
                            }
                        });
                    }
                },
                moreSurAssSearchHideShow : function () {
                    $('#select-list-surass-dropdown-list').slideToggle();
                    $('#select-list-surass-dropdown-list li:last').css('border-bottom','0px none');
                	},
                	/* Ticket: 46992 */
                	moreSurAssSearchHideShowTag : function () {
                        $('#select-list-surass-dropdown-list-tag').slideToggle();
                        $('#select-list-surass-dropdown-list-tag li:last').css('border-bottom','0px none');
                    	},
                    	/* Ticket: 46992 */
               moreSurAssSearchTypeText : function(dCode,dText) {
                    $('#select-list-surass-dropdown-list').hide();
                    $('#select-list-surass-dropdown').text(dCode);
                    $('#search_all_surass_type-hidden').val(dText);
                    var displayText;
                    if(dText=='surassqus'){
                 	   displayText = Drupal.t('LBL324');
                    }else{
                 	   displayText =Drupal.t('LBL036') + ' ' + dCode;
                    }
                    //var displayText = Drupal.t('LBL036') + ' ' + dCode;
            		$('#surassattchedquestions-autocomplete').val(displayText);
            		$('#surassattchedquestions-autocomplete_hidden').val(displayText);
            		$('#surassattchedquestions-autocomplete').addClass('input-field-grey');

             },
             /* Ticket: 46992 */
             moreSurAssSearchTypeTextTag : function(dCode,dText) {

                 $('#select-list-surass-dropdown-list-tag').hide();
                 $('#select-list-surass-dropdown-tag').text(dCode);
                 $('.admin_add_multi_search_container #tagsearch_all_surass_type-hidden').val(dText);

                 var displayText;
                 if(dText=='surassqus'){
              	   displayText = Drupal.t('LBL324');
                 }else{
              	   displayText =Drupal.t('LBL193');
                 }
                 //var displayText = Drupal.t('LBL036') + ' ' + dCode;

         		$('#surassattachquestion-autocomplete-tag').val(displayText);
         		$('#surassattachquestion-autocomplete-tag_hidden').val(displayText);
         		$('#surassattchedquestions-autocomplete-tag').addClass('input-field-grey');

          },
          /* Ticket: 46992 */
               moreCourseSearchTypeText : function(dCode,dText) {
                    //console.log('moreCourseSearchTypeText() called');
                    $('#select-list-course-dropdown').text(dCode);
                    $('#search_all_course_type-hidden').val(dText);
                    var displayText;
                    if(dText=='crstit'){
                 	   displayText = Drupal.t('LBL088')+ ' '+ Drupal.t('LBL083');
                    }else{
                 	   displayText =Drupal.t('LBL036') + ' ' + dCode;
                    }
                    //var displayText = Drupal.t('LBL036') + ' ' + dCode;
            		$('#tpattchedcoursename-autocomplete').val(displayText);
            		$('#tpattchedcoursename-autocomplete_hidden').val(displayText);
            		$('#tpattchedcoursename-autocomplete').addClass('input-field-grey');

             },
             	moreAdduserSearchTypeText : function(dCode,dText) {
                 //console.log('moreCourseSearchTypeText() called');
            	 $('#search-list-class-title-keyword #select-list-class-dropdown-list').hide();
                 $('#select-list-class-dropdown').text(dCode);
                 $('#search_all_user_type-hidden').val(dText);
                 var displayText;
                 if(dText=='usrtit'){
              	   displayText = Drupal.t('LBL181');
                 }else if(dText=='fultit'){
              	   displayText =Drupal.t('LBL036') + ' ' + Drupal.t('LBL691');
                 }
                 else{
              	   displayText =Drupal.t('LBL036') + ' ' + dCode;
                 }
                 //var displayText = Drupal.t('LBL036') + ' ' + dCode;
         		$('#username-search-autocomplete').val(displayText);
         		$('#username-search-autocomplete_hidden').val(displayText);
         		$('#username-search-autocomplete').addClass('input-field-grey');

          	},
          	 moreUserSearchTypeText : function(dCode,dText) {
          		$('#search-list-class-title-keyword #select-list-class-dropdown-list').hide();
                $('#select-list-class-dropdown-adduser').text(dCode);
                $('#search_all_user_type-hidden').val(dText);
                var displayText;
                if(dText=='usrtitle'){
             	   displayText = Drupal.t('LBL181');
                }else if(dText=='fultitle'){
             	   displayText =Drupal.t('LBL036') + ' ' + Drupal.t('LBL691');
                }else if(dText=='grptitle'){
             	   displayText =Drupal.t('LBL1270');
                }
                else{
             	   displayText =Drupal.t('LBL036') + ' ' + dCode;
                }
                //var displayText = Drupal.t('LBL036') + ' ' + dCode;
        		$('#addusername-search-autocomplete').val(displayText);
        		$('#addusername-search-autocomplete_hidden').val(displayText);
        		$('#addusername-search-autocomplete').addClass('input-field-grey');

          },
                getInlineEditAttachedQuestion : function(inlineData){
                    if(inlineData){
                        var inlineEditData= eval(inlineData);
                        this.inlineEditData = inlineEditData;
                    }
                    var html ="";
                    var callBlur = '$("#root-admin").data("narrowsearch")';
                    html += '<input size="16" type ="text" class="attachedquestion-edit-txtbox" id="attachedquestion-list-name-id-'+this.inlineEditData.questionId+'" value ="'+this.inlineEditData.fullGroupName+'" onkeydown="return $(\'#root-admin\').data(\'narrowsearch\').updateAttachedQuestionGroupKeyDown(event, this);" onblur="return $(\'#root-admin\').data(\'narrowsearch\').updateAttachedQuestionGroup(this);">';
                    $("#attachedquestion-name-div-"+this.inlineEditData.questionId).html(html);
                    $('#attachedquestion-list-name-id-'+this.inlineEditData.questionId).focus();

                },

                updateAttachedQuestionGroupKeyDown : function(evt, updatedGroup){
            		evt = evt || window.event;
            	    var charCode = evt.keyCode || evt.which;
            	    if (charCode == 13) {
            	    	$("#root-admin").data("narrowsearch").updateAttachedQuestionGroup(updatedGroup);
            	        evt.preventDefault();
            	        return false;
            	    }

                },
                searchClassNameFilter : function(courseId,classId){
                	var obj = this;
                	var className    = encodeURIComponent($('#classname-autocomplete').val());
                	var searchType   = $('#search_all_classs_type-hidden').val();
					var pagerId	     = '#admin-course-class-list-'+courseId+'-pagination_toppager';
					var objStr       = '$("#root_admin").data("narrowsearch")';

					obj.createLoader("paint-class-search-results-datagrid");
					$("#admin-course-class-list-"+courseId+"-pagination").setGridParam({url: this.constructUrl("administration/class-pagination/"+courseId+"/"+classId+"/0&class_name="+className+"&search_Type="+searchType)});
					$("#admin-course-class-list-"+courseId+"-pagination").trigger("reloadGrid",[{page:1}]);

					//$('#paintContentResults').setGridParam({url: this.constructUrl('learning/catalog-search/search/all/'+searchStr)});
				    //$("#paintContentResults").trigger("reloadGrid",[{page:1}]);
               },
               moreClassSearchHideShow : function () {
                   $('#select-list-class-dropdown-list').slideToggle();
                   $('#select-list-class-dropdown-list li:last').css('border-bottom','0px none');
               	},

              moreClassSearchTypeText : function(dCode,dText) {
                   $('#select-list-class-dropdown-list').hide();
                   $('#select-list-class-dropdown').text(dCode);
                   $('#search_all_classs_type-hidden').val(dText);
                   $('#search_all_user_type-hidden').val(dText);
                   var displayText;
                   if(dText=='clstit'){
                	   displayText = Drupal.t('LBL766');
                   }else if (dText=='clsstatus') {
                	   displayText =Drupal.t('LBL036') + ' ' + Drupal.t('Class')+ ' ' + dCode;
                   }else if(dText=='usrtit' || dText=='fultit' ){
		                $('#username-search-autocomplete').val(displayText);
		                $('#username-search-autocomplete_hidden').val(displayText);
		              	$('#username-search-autocomplete').addClass('input-field-grey');
		              	$('#username-search-autocomplete').focus();
                   }
                   else{
                	   displayText =Drupal.t('LBL036') + ' ' + dCode;
                   }
                   //var displayText = Drupal.t('LBL036') + ' ' + dCode;

           		$('#classname-autocomplete').val(displayText);
           		$('#classname-autocomplete_hidden').val(displayText);
           		$('#classname-autocomplete').addClass('input-field-grey');

            },

            templateDisplayPagination : function(notificationId,templateId,argType){
				var obj = this;
				var pagerId	= '#admin-notification-template-list-'+notificationId+'-pagination_toppager';
				var objStr = '$("#root_admin").data("narrowsearch")';
				if(argType == 'notification_template'){
				  urlType = "administration/manage/notification_template/temp-pagination/"+notificationId+"/"+templateId;
				}else{
				  urlType = "administration/manage/certificate/temp-pagination/"+notificationId+"/"+templateId;
				}
				//CREATE LOADER ICON
				obj.createLoader("paint-template-search-results-datagrid");
				$("#admin-notification-template-list-"+notificationId+"-pagination").jqGrid({
					url: this.constructUrl(urlType),
					datatype: "json",
					mtype: 'GET',
					//colNames:['Detail'],
					colModel:[ {name:'Detail',index:'detail', title: false, width:620, 'widgetObj':objStr, sortable: false, formatter: obj.displayClassListValues  }],
					rowNum:5,
					rowList:[5,10,15],
					pager: pagerId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					toppager: false,
					height: "auto",
					width: 620,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					loadui:false,
					loadComplete:obj.callbackTemplateListDataGrid
					}).navGrid('#pager-datagrid-'+notificationId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				$("#admin-notification-template-list-"+notificationId+'-pagination_toppager').hide();
				//}
				},
			callbackTemplateListDataGrid : function(response, postdata, formid) {
				var listRows 	= 	5;
				//var curObj	 	=	this;
				var curObjStr 	= 	$("#root-admin").data("narrowsearch");
				$('#admin-notification-template-list-'+response.notificationId+'-pagination').show();
				//$('#gview_admin-notification-template-list-'+response.notificationId+'-pagination').css();
				var element = document.getElementById('modal-content');
				if(element.clientHeight == element.scrollHeight){
					$("#gbox_admin-notification-template-list-"+response.notificationId+"-pagination").css('width','620px');
					$("#gbox_admin-notification-template-list-"+response.notificationId+"-pagination").css('overflow','hidden');
				}
				// Hide the pagination, if the record count in the view learning table is equal to or less than
				var recordCount = $('#admin-notification-template-list-'+response.notificationId+"-pagination").jqGrid('getGridParam', 'records');

		        if (recordCount == 0) {
		        	$("#admin-notification-template-list-"+response.notificationId+"-pagination"). css('display','block');
		            var html = Drupal.t('MSG381')+'.';
		            $("#admin-notification-template-list-"+response.notificationId+"-pagination").html('<tr><td class="border-style-none" width="620px"><div id="add-edit-class-norecords" class="no-records-msg">'+html+'</div></td></tr>');
		            $('#admin-notification-template-list-'+response.notificationId+"-pagination").css('text-align','center');
		            $('.border-style-none').css('border','0');
		        } else if(recordCount > listRows){
			        $("#admin-notification-template-list-"+response.notificationId+"-pagination_toppager").show();
			        if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
			         $("#admin-notification-template-list-"+response.notificationId+"-pagination_toppager_center").css('width','229px');
			        }else{
			         $("#admin-notification-template-list-"+response.notificationId+"-pagination_toppager_center").css('width','250px');
			        }

			        $('#add-edit-class-norecords').css('display','none');
				} else {
					$('#add-edit-class-norecords').css('display','none');
				}

				curObjStr.initGridTemplatePagination(response.notificationId);
				curObjStr.destroyLoader('paint-template-search-results-datagrid');

				$('#admin-notification-template-list-pagination-'+response.notificationId+' tr').click(function(event){
					event.stopPropagation();
				});

				$('.edit-class-list-container').last().css('border','0px none');
				var backdrop_height = $(document).height();
				$('#modalBackdrop').css('height',backdrop_height+'px');
				//When Template is added,the save and publish button is enabled
				$('.pub-unpub-only-save-btn').attr('disabled',false);
				$('.save-pub-unpub-sub-menu input').removeClass('save-and-enable-disabled');
				$('#admin-notification-template-list-'+response.notificationId+'-pagination tr td').find('div.edit-class-list').last().css('border-bottom','0px none');

				//Vtip-Display toolt tip in mouse over
				 vtip();
			},
			initGridTemplatePagination : function(notificationId) {
				var curObjStr 	= 	$("#root-admin").data("narrowsearch");

				//NEXT PREVIOUS LOADER SETTING
				$('#first_admin-notification-template-list-'+notificationId+'-pagination_toppager').click(function(e) {
									if (!$('#first_admin-notification-template-list-'+notificationId+'-pagination_toppager').hasClass('ui-state-disabled')) {
										$('#admin-notification-template-list-'+notificationId+'-pagination').hide();
										$('#gview_admin-notification-template-list-'+notificationId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-template-search-results-datagrid');
									}
								});
				$('#prev_admin-notification-template-list-'+notificationId+'-pagination_toppager').click(function(e) {
									if (!$('#prev_admin-notification-template-list-'+notificationId+'-pagination_toppager').hasClass('ui-state-disabled')) {
										$('#admin-notification-template-list-'+notificationId+'-pagination').hide();
										$('#gview_admin-notification-template-list-'+notificationId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-template-search-results-datagrid');
									}
								});
				$('#next_admin-notification-template-list-'+notificationId+'-pagination_toppager').click(function(e) {
									if (!$('#next_admin-notification-template-list-'+notificationId+'-pagination_toppager').hasClass('ui-state-disabled')) {
										$('#admin-notification-template-list-'+notificationId+'-pagination').hide();
										$('#gview_admin-notification-template-list-'+notificationId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-template-search-results-datagrid');
									}
								});
				$('#last_admin-notification-template-list-'+notificationId+'-pagination_toppager').click(function(e) {
									if (!$('#last_admin-notification-template-list-'+notificationId+'-pagination_toppager').hasClass('ui-state-disabled')) {
										$('#admin-notification-template-list-'+notificationId+'-pagination').hide();
										$('#gview_admin-notification-template-list-'+notificationId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-template-search-results-datagrid');
									}
								});

				$(".ui-pg-input").keyup(function(event) {
									if (event.keyCode == 13) {
										$('#admin-notification-template-list-'+notificationId+'-pagination').hide();
										$('#gview_admin-notification-template-list-'+notificationId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-template-search-results-datagrid');
									}
								});

			},
		    userDisplayPagination : function(roleId,userId){
				var obj = this;
				var pagerId	= '#admin-course-class-list-'+roleId+'-pagination_toppager';
				var objStr = '$("#root_admin").data("narrowsearch")';

				//CREATE LOADER ICON
				obj.createLoader("paint-user-search-results-datagrid");

				$("#admin-course-class-list-"+roleId+"-pagination").jqGrid({
					url: this.constructUrl("administration/people/group/user-pagination/"+roleId+"/"+userId),
					datatype: "json",
					mtype: 'GET',
					colNames:[  Drupal.t('LBL054'),Drupal.t('LBL691'),Drupal.t('LBL102'), multiselectCheckbox],
					colModel:[ { name:'User Name',index:'UserName', title: false, width:120, 'widgetObj':objStr, formatter: obj.userDisplayGridValues },
					           { name:'Full Name',index:'FullName', title: false, width:200, 'widgetObj':objStr, formatter: obj.userDisplayGridValues },
					           { name:'Status',index:'Status', title: false, width:80, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, hidden: hideStatusOption },
					           { name:'MultiselectCheck',index:'MultiselectCheck', title: false, width:19, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: false}
					         ],
					rowNum:5,
					rowList:[5,10,15],
					pager: pagerId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					toppager: false,
					height: "auto",
					width: 570,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					loadui:false,
					loadComplete:obj.callbackUserListDataGrid
					}).navGrid('#pager-datagrid-'+roleId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				$("#admin-course-class-list-"+roleId+'-pagination_toppager').hide();
				setQtipPosition();
			},
			callbackUserListDataGrid : function(response, postdata, formid) {
				var listRows 	= 	5;
				//var curObj	 	=	this;
				var curObjStr 	= 	$("#root-admin").data("narrowsearch");
				$('#admin-course-class-list-'+response.roleId+'-pagination').show();
				var element = document.getElementById('modal-content');
				if(element.clientHeight == element.scrollHeight){
					$("#gbox_admin-course-class-list-"+response.roleId+"-pagination").css('width','570px');
					$("#gbox_admin-course-class-list-"+response.roleId+"-pagination").css('overflow','hidden');
				}
				// Hide the pagination, if the record count in the view learning table is equal to or less than
				var recordCount = $('#admin-course-class-list-'+response.roleId+"-pagination").jqGrid('getGridParam', 'records');

		        if (recordCount == 0) {
		        	$("#admin-course-class-list-"+response.roleId+"-pagination"). css('display','block');
		            var html = Drupal.t('MSG381')+'.';
		            $("#admin-course-class-list-"+response.roleId+"-pagination").html('<tr><td class="border-style-none" width="570px"><div id="add-edit-class-norecords" class="no-records-msg">'+html+'</div></td></tr>');
		            $('#admin-course-class-list-'+response.roleId+"-pagination").css('text-align','center');
		            $('.border-style-none').css('border','0');
		        } else if(recordCount > listRows){
			        $("#admin-course-class-list-"+response.roleId+"-pagination_toppager").show();
			        $('#add-edit-class-norecords').css('display','none');
				} else {
					$('#add-edit-class-norecords').css('display','none');
				}

				curObjStr.initGridUserPagination(response.roleId);
				curObjStr.destroyLoader('paint-user-search-results-datagrid');

				$('#admin-course-class-list-pagination-'+response.roleId+' tr').click(function(event){
					event.stopPropagation();
				});

				$('.edit-class-list-container').last().css('border','0px none');
				var backdrop_height = $(document).height();
				$('#modalBackdrop').css('height',backdrop_height+'px');
				//Vtip-Display toolt tip in mouse over
				 vtip();
			},
			initGridUserPagination : function(roleId) {
				var curObjStr 	= 	$("#root-admin").data("narrowsearch");

				//NEXT PREVIOUS LOADER SETTING
				$('#prev_admin-course-class-list-'+roleId+'-pagination_toppager').click(function(e) {
									if (!$('#prev_admin-course-class-list-'+roleId+'-pagination_toppager').hasClass('ui-state-disabled')) {
										$('#admin-course-class-list-'+roleId+'-pagination').hide();
										$('#gview_admin-course-class-list-'+roleId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-class-search-results-datagrid');
									}
								});
				$('#next_admin-course-class-list-'+roleId+'-pagination_toppager').click(function(e) {
									if (!$('#next_admin-course-class-list-'+roleId+'-pagination_toppager').hasClass('ui-state-disabled')) {
										$('#admin-course-class-list-'+roleId+'-pagination').hide();
										$('#gview_admin-course-class-list-'+roleId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-class-search-results-datagrid');
									}
								});

				$(".ui-pg-input").keyup(function(event) {
									if (event.keyCode == 13) {
										$('#admin-course-class-list-'+roleId+'-pagination').hide();
										$('#gview_admin-course-class-list-'+roleId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-class-search-results-datagrid');
									}
								});

			},



				classDisplayPagination : function(courseId,classId,oldClassId){
					var obj = this;
					var pagerId	= '#admin-course-class-list-'+courseId+'-pagination_toppager';
					var objStr = '$("#root_admin").data("narrowsearch")';

					//CREATE LOADER ICON
					obj.createLoader("paint-class-search-results-datagrid");
					if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
						var Wgrid = '835';
					} else {
						var Wgrid = '827';
					}

					$("#admin-course-class-list-"+courseId+"-pagination").jqGrid({
						url: this.constructUrl("administration/class-pagination/"+courseId+"/"+classId+"/"+oldClassId),
						datatype: "json",
						mtype: 'GET',
						colNames:['Detail'],
						colModel:[ {name:'Detail',index:'detail', title: false, width:860, 'widgetObj':objStr, sortable: false, formatter: obj.displayClassListValues  }],
						rowNum:5,
						rowList:[5,10,15],
						pager: pagerId,
						viewrecords:  true,
						multiselect: false,
						emptyrecords: "",
						toppager: false,
						height: "auto",
						width: Wgrid,
						loadtext: "",
						recordtext: "",
						pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
						loadui:false,
						loadComplete:obj.callbackClassListDataGrid
						}).navGrid('#pager-datagrid-'+courseId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
					$("#admin-course-class-list-"+courseId+'-pagination_toppager').hide();
					setQtipPosition();
				},
				callbackClassListDataGrid : function(response, postdata, formid) {
				Drupal.ajax.prototype.commands.CtoolsModalAdjust(null, null, null);
					if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
						var Wgrid = '835';
					} else {
						var Wgrid = '827';
					}
					var listRows 	= 	5;
					//var curObj	 	=	this;
					var curObjStr 	= 	$("#root-admin").data("narrowsearch");
					$('#admin-course-class-list-'+response.courseId+'-pagination').show();
					var element = document.getElementById('modal-content');
					if(element.clientHeight == element.scrollHeight){
						$("#gbox_admin-course-class-list-"+response.courseId+"-pagination").css('width',Wgrid+'px');
						$("#gbox_admin-course-class-list-"+response.courseId+"-pagination").css('overflow','hidden');
					}
					// Hide the pagination, if the record count in the view learning table is equal to or less than
					var recordCount = $('#admin-course-class-list-'+response.courseId+"-pagination").jqGrid('getGridParam', 'records');

			        if (recordCount == 0) {
			        	$("#admin-course-class-list-"+response.courseId+"-pagination"). css('display','block');
			            var html = Drupal.t('MSG381')+'.';
			            $("#admin-course-class-list-"+response.courseId+"-pagination").html('<tr><td class="border-style-none" width="'+Wgrid+'px"><div id="add-edit-class-norecords" class="no-records-msg">'+html+'</div></td></tr>');
			            $('#admin-course-class-list-'+response.courseId+"-pagination").css('text-align','center');
			            $('.border-style-none').css('border','0');
			            $("#admin-course-class-list-"+response.courseId+"-pagination_toppager").hide();
			        } else if(recordCount > listRows){
				        $("#admin-course-class-list-"+response.courseId+"-pagination_toppager").show();
				        if ( $.browser.msie && parseInt($.browser.version, 10)=='9'){
				        $("#admin-course-class-list-"+response.courseId+"-pagination_toppager").find('.ui-pg-table').css('width', 'auto');
				        }
				        $('#add-edit-class-norecords').css('display','none');
					} else {
						$('#add-edit-class-norecords').css('display','none');
						$("#admin-course-class-list-"+response.courseId+"-pagination_toppager").hide();
					}

					curObjStr.initGridClassPagination(response.courseId);
					curObjStr.destroyLoader('paint-class-search-results-datagrid');

					$('#admin-course-class-list-pagination-'+response.courseId+' tr').click(function(event){
						event.stopPropagation();
					});

					$('.edit-class-list-container').last().css('border','0px none');
					var backdrop_height = $(document).height();
					$('#modalBackdrop').css('height',backdrop_height+'px');
					//Vtip-Display toolt tip in mouse over
					 vtip();
					 $("#admin-course-class-list-"+response.courseId+"-pagination > tbody > tr:last > td > .edit-class-list"). css('border','0px none');
				},


				initGridClassPagination : function(courseId) {

					var curObjStr 	= 	$("#root-admin").data("narrowsearch");

					//NEXT PREVIOUS LOADER SETTING
					$('#first_admin-course-class-list-'+courseId+'-pagination_toppager').click(function(e) {
										if (!$('#first_admin-course-class-list-'+courseId+'-pagination_toppager').hasClass('ui-state-disabled')) {
											$('#admin-course-class-list-'+courseId+'-pagination').hide();
											$('#gview_admin-course-class-list-'+courseId+'-pagination').css('min-height','50px');
											curObjStr.createLoader('paint-class-search-results-datagrid');
										}
									});
					$('#prev_admin-course-class-list-'+courseId+'-pagination_toppager').click(function(e) {
										if (!$('#prev_admin-course-class-list-'+courseId+'-pagination_toppager').hasClass('ui-state-disabled')) {
											$('#admin-course-class-list-'+courseId+'-pagination').hide();
											$('#gview_admin-course-class-list-'+courseId+'-pagination').css('min-height','50px');
											curObjStr.createLoader('paint-class-search-results-datagrid');
										}
									});
					$('#next_admin-course-class-list-'+courseId+'-pagination_toppager').click(function(e) {
										if (!$('#next_admin-course-class-list-'+courseId+'-pagination_toppager').hasClass('ui-state-disabled')) {
											$('#admin-course-class-list-'+courseId+'-pagination').hide();
											$('#gview_admin-course-class-list-'+courseId+'-pagination').css('min-height','50px');
											curObjStr.createLoader('paint-class-search-results-datagrid');
										}
									});
					$('#last_admin-course-class-list-'+courseId+'-pagination_toppager').click(function(e) {
										if (!$('#last_admin-course-class-list-'+courseId+'-pagination_toppager').hasClass('ui-state-disabled')) {
											$('#admin-course-class-list-'+courseId+'-pagination').hide();
											$('#gview_admin-course-class-list-'+courseId+'-pagination').css('min-height','50px');
											curObjStr.createLoader('paint-class-search-results-datagrid');
										}
									});

					$(".ui-pg-input").keyup(function(event) {
										if (event.keyCode == 13) {
											$('#admin-course-class-list-'+courseId+'-pagination').hide();
											$('#gview_admin-course-class-list-'+courseId+'-pagination').css('min-height','50px');
											curObjStr.createLoader('paint-class-search-results-datagrid');
										}
									});

				},
				displayClassListValues : function(cellvalue, options, rowObject) {
					return rowObject['detail'];
				},

				userDisplayGridValues : function(cellvalue, options, rowObject){
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
						return '<input type="'+inputType+'" id="multiselect-singlecheck-'+uniqueId+'_'+id+'" value="'+id+'" class="multiselect-singlecheck" />';
					}else if(index == 'UserName'){
							var titleAllowed = type == 'RoleAddusers' ?  13 : 20;
							var userNameRestricted = $('body').data('mulitselectdatagrid').titleRestricted(rowObject[index], titleAllowed);
							return '<span class="vtip" title="'+htmlEntities(rowObject[index])+'">'+userNameRestricted+'</span>';

				   }else if(index == 'FullName'){

						var titleAllowed = type == 'RoleAddusers' ?  15 : 35;
						var fullNameRestricted = $('body').data('mulitselectdatagrid').titleRestricted(rowObject[index], titleAllowed);
						return '<span class="vtip" title="'+htmlEntities(rowObject[index])+'">'+fullNameRestricted+'</span>';

					}


				},

                updateAttachedQuestionGroup : function(updatedGroup){

                    var inlineData = this.inlineEditData;
                    var updatedGroupName = escape($(updatedGroup).attr("value"));
                    var oldGroupName	= this.inlineEditData.groupName;
                    this.inlineEditData.groupName = updatedGroupName;
                    var obj = this;
                    /*-- #36135: Isuse in Survey/Assessment group name tool --*/
                    var regGroupName = /^[A-Za-z0-9.,@_\s]{0,100}$/;  // /[^a-zA-Z0-9,.@\'\_ |\/]/
                    var grpString = decodeURIComponent(updatedGroupName);
                    // 36135 : validatin code added
                    if(!regGroupName.test(grpString)) {
                    	var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li style="width: 200px;"><span>' +Drupal.t('MSG644') + ' ' + Drupal.t('MSG639') + '</span></li></ul><div onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();" class="msg-close-btn"></div></div></div></div>';
                    	$("#show_expertus_message").append(error_msg);
                    	return false;
                    }
                    if(updatedGroupName == "No Group Name" || updatedGroupName == "") {
                    	updatedGroupName = oldGroupName;
                    	this.inlineEditData.groupName = updatedGroupName;
                    	var html = "";
                        html += '<a href="javascript:void(0);" id="attachedquestion-list-name-id-'+inlineData.questionId+'" name="attachedquestion-list-name-id-'+inlineData.questionId+'" class = "attachedcourse-list-name"  ondblclick ="$(\'#root-admin\').data(\'narrowsearch\').getInlineEditAttachedQuestion();">'+updatedGroupName+'</a>';
                        $("#attachedquestion-name-div-"+inlineData.questionId).html(html);
                    }else{
                    	obj.createLoader('survey_attach_questions_dt');
                        url = obj.constructUrl("administration/update-attachedquestion-group/"+inlineData.entityId+"/"+updatedGroupName+"/"+inlineData.questionId);
                        $.ajax({
                            type: "POST",
                            url: url,
                            data:  '',
                            success: function(result){
                        	    obj.destroyLoader('survey_attach_questions_dt');
                        	    var result = result.split('|');
                        	    var shortGroupName = result[0];
                        	    var longGroupName = result[1];
                                var html = "";
                                var dataQuesGroup = 'data={"entityId":"'+inlineData.entityId+'","entityType":"'+inlineData.entityType+'","groupName":"'+$.trim(shortGroupName)+'","fullGroupName":"'+$.trim(longGroupName)+'","questionId":"'+inlineData.questionId+'"}';
                                dataQuesGroup = unescape(dataQuesGroup).replace(/"/g, '&quot;');
                                html += '<a href="javascript:void(0);" id="attachedquestion-list-name-id-'+inlineData.questionId+'" name="attachedquestion-list-name-id-'+inlineData.questionId+'" class = "attachedquestion-list-name"  ondblclick ="$(\'#root-admin\').data(\'narrowsearch\').getInlineEditAttachedQuestion(\''+dataQuesGroup+'\');">'+titleRestrictionFadeoutImage(shortGroupName,'survey-grp-name')+'</a>';
                                $("#attachedquestion-name-div-"+inlineData.questionId).html(html);
                                $("#attachedquestion-name-div-"+inlineData.questionId).attr('title', longGroupName);
                            }
                        });
                    }

                },

                /*

                updateAttachedQuestionScore : function(inlineData){
                	if(inlineData){
                          var inlineEditData= eval(inlineData);
                          this.inlineEditData = inlineEditData;
                    }
                	var scoreValue = $('#edit-edit-attachquestion-list-score-'+this.inlineEditData.loopId).val();
	                 if(scoreValue != ''){
	                    var obj = this;
	                    url = obj.constructUrl("administration/update-attachedquestion-score/"+this.inlineEditData.entityId+"/"+this.inlineEditData.entityType+"/"+this.inlineEditData.questionId+"/"+scoreValue);
	                    $.ajax({
	                        type: "POST",
	                        url: url,
	                        data:  '',
	                        success: function(result){

	                    	}
	                    });
	                 }

                },

                updateAttachedQuestionScoreKeyDown : function(evt, inlineData){
            		evt = evt || window.event;
            	    var charCode = evt.keyCode || evt.which;
            	    if (charCode == 13) {
            	    	$("#root-admin").data("narrowsearch").updateAttachedQuestionScore(inlineData);
            	        evt.preventDefault();
            	        return false;
            	    }

                }*/

		updatePreviewContent :function(){
        	/*
				function to redirect callback of content launch
				--> searches for the callback function on close of the content and removes the corresponding record from
				slt_aicc_interaction -- only for aicc contents

			*/

            var url='';
            var post_data='';
            if(scoobj._type == "AICC" || scoobj._type == "AICC Course Structure") {
            	var sid = $('#root-admin').data('narrowsearch').launchAICCId;
            	url = resource.base_url+"/sites/all/commonlib/AICC_Handler.php";
            	post_data={"session_id":sid,"command":"UPDATEAICCPREVIEW"};

        		var obj = this;
        		$.ajax({
        			type: "POST",
        			url: url,
        			data:  post_data,
        			success: function(result) {

        			}
        	    });
            }
        },

        showExpAdminDropdownMenu : function (dropdownListId) {
          var dropdownListSelector = '#' + dropdownListId;
            if ($(dropdownListSelector).is(":visible")) {
              //console.log('showExpAdminDropdownMenu() : is already displayed ' + dropdownListId);
              // hideExpAdminDropDownMenu() will hide the dropdown menu when it is already displayed.
            }
            else {
              //console.log('showExpAdminDropdownMenu() : showing ' + dropdownListId);
              $(dropdownListSelector).slideDown(); // show menu
              $(dropdownListSelector + ' li:last').css('border-bottom','0px none');

              $('html').bind('mousedown.' + dropdownListId, {dropdownListId : dropdownListId},
                                           $("#root-admin").data("narrowsearch").hideOnClickExpAdminDropDownMenu);
            }
         },

         hideOnClickExpAdminDropDownMenu : function (e) {
           var dropdownListId = e.data.dropdownListId;
           var dropdownListSelector = '#' + dropdownListId;
           var el = e.target;
           var dropdown = $('#' + dropdownListId + ':visible')[0];
           if(typeof dropdown == 'undefined') {
             //console.log('hideOnClickExpAdminDropDownMenu() : no dropdown menu open. Returning true');
             $('html').unbind('mousedown.' + dropdownListId, $("#root-admin").data("narrowsearch").hideOnClickExpAdminDropDownMenu);
             return true;
           }

           // Hide the dropdown menu
           //console.log('hideOnClickExpAdminDropDownMenu() : hiding ' + dropdownListId);
           $(dropdownListSelector).slideUp(); // hide menu
           $('html').unbind('mousedown.' + dropdownListId, $("#root-admin").data("narrowsearch").hideOnClickExpAdminDropDownMenu);
           return false; // no need to propagate the event further
        },

        meetingUrlCreate : function (meetingName,classId) {
        	if(meetingName == 'lrn_cls_vct_oth'){ // Others
        		$('#session_presenter_password').val('');
        		$('#session_attende_password').val('');
        		$('#session_meeting_id').val('');
        		$('#attendee_url').css('display','block');
        		$('#presenter_url').css('display','block');
        		$('#attendee_password').css('display','none');
        		$('#presenter_password').css('display','none');
        		$('#meeting_id_dt').css('display','none');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class');
        		$('.admin-addedit-session-details-meeting tr:visible').addClass('session-meeting-vc-class-others');

	        }else if(meetingName == 'lrn_cls_vct_exp'){ // Expertus Meeting
        		$('#session_presenter_password').val('');
        		$('#session_attende_password').val('');
        		$('#session_meeting_id').val('');
        		$('#session_presenter_password').attr('disabled',true).addClass('admin-grey-out-fields');
        		$('#session_attende_password').attr('disabled',true).addClass('admin-grey-out-fields');
        		$('#session_meeting_id').attr('disabled',true).addClass('admin-grey-out-fields');
        		$('.session-pass-mandatory').css('display','none');
        		$('#attendee_url').css('display','none');
        		$('#presenter_url').css('display','none');
/*        		$('#attendee_password').css('display','block');
        		$('#presenter_password').css('display','block');
        		$('#meeting_id_dt').css('display','block'); */

        		$('#attendee_password').css('display','none');
        		$('#presenter_password').css('display','none');
        		$('#meeting_id_dt').css('display','none');
        		$('.admin-addedit-session-details-meeting tr:visible').addClass('session-meeting-vc-class');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class-others');

	        }else{
	        	$('#session_presenter_password').attr('disabled',false).removeClass('admin-grey-out-fields');
    			$('#session_attende_password').attr('disabled',false).removeClass('admin-grey-out-fields');
    			$('#session_meeting_id').attr('disabled',false).removeClass('admin-grey-out-fields');
        		$('.session-pass-mandatory').css('display','inline-block');
        		$('#attendee_url').css('display','none');
        		$('#presenter_url').css('display','none');
        		$('#attendee_password').css('display','block');
        		$('#presenter_password').css('display','block');
        		$('#meeting_id_dt').css('display','none');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class-others');
	        }
        },

		getMeetingType : function(){
        	var meetingtypeid = $('#set-meeting-type').val();
            var class_id = $('#class_id').val();
            if(meetingtypeid == 'lrn_cls_vct_oth'){
            	$('#session_presenter_password').val('');
        		$('#session_attende_password').val('');
        		$('#session_meeting_id').val('');
            	$('#attendee_url').css('display','block');
        		$('#presenter_url').css('display','block');
        		$('#attendee_password').css('display','none');
        		$('#presenter_password').css('display','none');
        		$('#meeting_id_dt').css('display','none');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class');
        		$('.admin-addedit-session-details-meeting tr:visible').addClass('session-meeting-vc-class-others');
            }else if(meetingtypeid == 'lrn_cls_vct_exp'){ // Expertus Meeting
        		$('#session_presenter_password').val('');
        		$('#session_attende_password').val('');
        		$('#session_meeting_id').val('');
        		$('#session_presenter_password').attr('disabled',true).addClass('admin-grey-out-fields');
        		$('#session_attende_password').attr('disabled',true).addClass('admin-grey-out-fields');
        		$('#session_meeting_id').attr('disabled',true).addClass('admin-grey-out-fields');
        		$('.session-pass-mandatory').css('display','none');
        		$('#attendee_url').css('display','none');
        		$('#presenter_url').css('display','none');
        		$('#attendee_password').css('display','none');
        		$('#presenter_password').css('display','none');
        		$('#meeting_id_dt').css('display','none');
        		$('.admin-addedit-session-details-meeting tr:visible').addClass('session-meeting-vc-class');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class-others');
	        }else{
	        	var getDisAttr = $('#session_meeting_id');
	        	if(!getDisAttr.attr('disabled')){
	        		$('#session_presenter_password').attr('disabled',false);
	        		$('#session_attende_password').attr('disabled',false);
	        		$('#session_meeting_id').attr('disabled',false);
	        	}
    			$('.session-pass-mandatory').css('display','inline-block');
    			$('#session_presenter_password').removeClass('admin-grey-out-fields');
        		$('#session_attende_password').removeClass('admin-grey-out-fields');
        		$('#session_meeting_id').removeClass('admin-grey-out-fields');
        		$('#attendee_url').css('display','none');
        		$('#presenter_url').css('display','none');
        		$('#attendee_password').css('display','block');
        		$('#presenter_password').css('display','block');
        		$('#meeting_id_dt').css('display','');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class-others');
	        }

        },


         /*
    	function to display popup if content has multiple lessons
    	--> rendered on clicking "preview" for each version
         */
        getPreviewContent : function(contentId,VersionId,lessonlist){

    		var closeButt={};
    	    $('#lesson-wizard').remove();
    	  	html="";
    		html+='<div id="lesson-wizard" class="lesson-wizard-content" style="display:none; padding: 0px;">';
    	    html+= this.getContentVersionLessons(contentId,VersionId,lessonlist);
    	    html+='</div>';
    	    $("body").append(html);
    	    Drupal.attachBehaviors();


    	    $("#lesson-wizard").dialog({
    	        position:[(getWindowWidth()-500)/2,100],
    	        bgiframe: true,
    	        width: 450,
    	        zIndex: 10000,
    	        resizable:false,
    	        modal: true,
    	        title: Drupal.t('LBL889'),
    	        buttons:closeButt,
    	        closeOnEscape: false,
    	        draggable:false,
    	        overlay:
    	         {
    	           opacity: 0.9,
    	           background: "black"
    	         }
    	    });

    	    $("#lesson-wizard").show();

    		$('.ui-dialog-titlebar-close').click(function(){
    	        $("#lesson-wizard").remove();
    	    });
			if(this.currTheme == 'expertusoneV2'){
		    	changeDialogPopUI();
			}
    	    //SCrollbar for content dialogbox
    	    this.scrollBarContentDialog();

        },

        /*
    	function to display the list of lessons inside the dialog
    	--> rendered on clicking "preview" for each version
        */
        getContentVersionLessons : function(contentId,VersionId,data){


    	var lesCnt = 0;
    	var ostr = '';
    	ostr += '<td colspan="2"><div class="enroll-accordian-div">';
    	ostr += '<table class="launch-lesson-view" cellpadding="0" cellspacing="0" width="100%" border="0">';


    	$(data).each(function(){
    		$(this).each(function(){
    			  lesCnt++;
    			  var params = "";
    			  var lessontitle = $(this).attr('title');
    			  var launchurl =  $(this).attr('launchurl');
    			  var contentType = $(this).attr('contentype');
    			  var contentTypeCode = $(this).attr('contentypecode');
    			  var contenthostedtype = $(this).attr('contenthostedtype');
    			  var aicc_sid = $(this).attr('AICC_SID');
    			  var callLaunchUrlFn;
    			  var lessonId =  $(this).attr('lessonid');
    			  if(contentTypeCode != "lrn_cnt_typ_vod"){
    				    params = "{'Id':'"+lessonId+"','url1':'"+launchurl+
    			            "','ErrMsg':'"+"error"+"','contentType':'"+contentType+"','AICC_SID':'"+aicc_sid+"'}";
    				    callLaunchUrlFn = "onclick=\"$('#root-admin').data('narrowsearch').launchLesson("+params+")\"";
    			  }else{
    				    lessontitle = encodeURIComponent(lessontitle.replace(/\//g, '()|()'));
    				    callLaunchUrlFn = "href=\"/?q=learning/nojs/play_video/";
    				    callLaunchUrlFn += lessontitle + "/";
    				    callLaunchUrlFn += escape(contenthostedtype) + "/";
    				    callLaunchUrlFn += escape(launchurl.replace(/\//g, '()|()')) + '/';
    				    callLaunchUrlFn += "ME" + "/";
    				    callLaunchUrlFn += "/";
    				    callLaunchUrlFn += "/";
    				    callLaunchUrlFn += lessonId + "/";
    				    callLaunchUrlFn += "/";
    				    callLaunchUrlFn += "/";
    				    callLaunchUrlFn += "\"";

    			  }

    		      var pipe = '<span class="enroll-pipeline">|</span>';

    			  ostr += '<tr class="enroll-lesson-section"><td class="enroll-launch-column1">';
    			  ostr += '<div class="enroll-course-title-lesson">';
    			  ostr += '<span id="titleAccEn_'+lessonId+'" class="item-title" ><span class="enroll-course-title vtip" title="'+Drupal.t('LBL854')+' '+lesCnt+' : '+unescape(lessontitle).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
    			  ostr += 'Lesson '+lesCnt+': ' +titleRestricted(decodeURIComponent(lessontitle),35);
    			  ostr += '</span></span>';
    			  ostr += '</div>';
    			  ostr += '<div class="enroll-class-title-info">';
    			  ostr += '<div class="item-title-code">';

    			  ostr += ' </div>';
    			  ostr += '</div>';
    			  ostr += '</td>';
    			  ostr += '<td class="enroll-launch-column2">';
    			  ostr +=   '<div class="enroll-main-list">';
    			  ostr +=     '<label id="' + lessonId + '_launch_button_lbl"' +
    			                  ' class="'+"enroll-launch-full" + ' launch_button_lbl">';

    			  if(contentTypeCode != "lrn_cnt_typ_vod"){
    				  ostr += '<input type="button" id="' + lessonId + '_launch_button"' +
    			                   ' class="' + 'actionLink' + ' enroll-launch"' +

    			                       ' value="' + Drupal.t('LBL199') + '" '+ callLaunchUrlFn  + '>';
    			  }else{
    				  ostr += "<a class='actionLink enroll-launch use-ajax ctools-modal-ctools-video-style' " +
                      "title='" + Drupal.t("LBL199") + "' " +
                        "id='launch" + lessonId + "' " +
                          callLaunchUrlFn +
                            " >" +
                              Drupal.t("LBL199") + "</a>";
    			  }
    			  ostr +=     '</label>';
    			  ostr +=   '</div>';
    			  ostr += '</td></tr>';

    		});
    	});
    	ostr += '</table></div"></td>';
    	return ostr;

        },

        /*
    	function to initialixe scorm wrapper
    	--> called on clicking "preview" for each version
         */
        launchLesson : function(data){
        	var pmType 		= data.contentType;
	        var contentVersion;
	        var contentType;
		    var stdid		= this.getLearnerId();
		    var stdname 	= this.getUserName();
		    $('#root-admin').data('narrowsearch').launchUserId = stdid;
		    $('#root-admin').data('narrowsearch').launchLessonId = data.Id;
		    $('#root-admin').data('narrowsearch').launchAICCId = data.AICC_SID;
		    if(stdid == "0" || stdid == "")
			{
			    self.location='?q=learning/enrollment-search';
		        return;
			}
	        if(pmType.toLowerCase() == 'scorm 1.2' || pmType.toLowerCase() == 'scorm 2004') {
	        	pmType				= pmType;
	        	var pmTypeVer 		= pmType.split(' ');
	            contentType  	= pmTypeVer[0];
	            contentVersion 	= pmTypeVer[1];
	        } else  {
	            contentVersion 	= '';
	            contentType 	= pmType;
	        }

	        var x	= {version: contentVersion,type:contentType};
	        scoobj 	= new SCORM_API_WRAPPER(x);

	        var cdnstatus = data.cdnmodulestatus;
	        if(cdnstatus == 1){
	        	$.ajax({
	        		url: '?q=ajax/getcdnurl/'+data.Id,
					data:  '',
					success: function(result){
						var output = result.split('~~');
						var url1      = output[0];
	        var data1 = {
				    	        url				 : url1,
				    	        callback		 : $('#root-admin').data('narrowsearch').updatePreviewContent,
				    	        learnerId		 : stdid,
				    	        learnerName		 : stdname,
				    	        completionStatus : 'not attempted',
				    	        scoreMax		 : '0',
				    	        scoreMin		 : '0',
				    	        score			 : '0',
				    	        location		 : '',
				    	        courseid 		 : '0',
				    	        classid 		 : '0',
				    	        lessonid 		 : data.Id,
				    	        versionid		 : '0',
				    	        enrollid 		 : '0',
				    	        launch_data		 : '',
				    	        suspend_data     : '',
				    	        exit			 : '',
				    	        aicc_sid		 : data.AICC_SID
				    	    };

				        scoobj.Initialize(data1);
					}
				  });
				} else {

	        var data1 = {
	    	        url				 : decodeURIComponent(data.url1),
	    	        callback		 : $('#root-admin').data('narrowsearch').updatePreviewContent,
	    	        learnerId		 : stdid,
	    	        learnerName		 : stdname,
	    	        completionStatus : 'not attempted',
	    	        scoreMax		 : '0',
	    	        scoreMin		 : '0',
	    	        score			 : '0',
	    	        location		 : '',
	    	        courseid 		 : '0',
	    	        classid 		 : '0',
	    	        lessonid 		 : data.Id,
	    	        versionid		 : '0',
	    	        enrollid 		 : '0',
	    	        launch_data		 : '',
	    	        suspend_data     : '',
	    	        exit			 : '',
	    	        aicc_sid		 : data.AICC_SID
	    	    };

	        scoobj.Initialize(data1);
				}
        },

        scrollBarContentDialog : function(){
	    	var contentDialogHeight = $("#lesson-content-container").height();
	    	if(contentDialogHeight > 550) {
	    		$(".lesson-wizard-content").css('height',550);
	    	} else if(contentDialogHeight <= 550) {
	    		$(".lesson-wizard-content").css('height','auto').css('min-height','70px');
	    	}

        },

        underlineTagAndTriggerSearch : function(tagName, filterCode, currElem) {
      	  try {
      		$('.tagscloud-tag').css('text-decoration', '');
      		$(currElem).css('text-decoration', 'underline');
      		$('#' + filterCode + '-addltext-filter').val(tagName);
									    $('#' + filterCode + '-tagscloud-clr').css('display', 'block');
      	    $('#root-admin').data('narrowsearch').refreshGrid();
      	  }
      	  catch (e) {
      	    // To Do
      	  }
      	},

      underlineTagCloudAndTriggerSearch : function(tagName, filterCode, currElem,tagWeight) {
        	  try {
        		$('.tagscloud-tag').css('text-decoration', '');
        		$(currElem).css('text-decoration', 'underline');
        		$('#' + filterCode + '-addltext-filter').val(tagName);
									    $('#' + filterCode + '-tagscloud-clr').css('display', 'block');
        	    $('#root-admin').data('narrowsearch').refreshGrid();
        	    $tagText = $(currElem).html();
        	    if($('#tagentityType').val() == 'tagtip'){
        	    	 var tagpos = $.inArray($tagText,existtagArrAdmin,0);
             	    if(tagpos == -1){
         				existtagArrAdmin.unshift($tagText);
         				existtagArrAdmin.pop();
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').find('span:last-child').remove();
         				$(currElem).html(descController('FADE OUT',$tagText,weightArrAdmin[tagWeight-1]));
         				var selectedTag = $(currElem).parent().html();
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').prepend('<span class="tagscloud-term">'+selectedTag+'</span>');
         				$(currElem).html($tagText);
         			}else if(tagpos >= 0){
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').find('span:nth-child('+(tagpos+1)+')').remove();
         				if (tagpos > -1) {
         					existtagArrAdmin.splice(tagpos, 1);
         				}
         				existtagArrAdmin.unshift($tagText);
         				$(currElem).html(descController('FADE OUT',$tagText,weightArrAdmin[tagWeight-1]));
         				var selectedTag = $(currElem).parent().html();
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').prepend('<span class="tagscloud-term">'+selectedTag+'</span>');
         				$(currElem).html($tagText);
         			}
        	    }else{
        	    	var tagpos = $.inArray($tagText,existtagClsArrAdmin,0);
             	    if(tagpos == -1){
         				existtagClsArrAdmin.unshift($tagText);
         				existtagClsArrAdmin.pop();
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').find('span:last-child').remove();
         				$(currElem).html(descController('FADE OUT',$tagText,weightArrAdmin[tagWeight-1]));
         				var selectedTag = $(currElem).parent().html();
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').prepend('<span class="tagscloud-term">'+selectedTag+'</span>');
         				$(currElem).html($tagText);
         			}else if(tagpos >= 0){
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').find('span:nth-child('+(tagpos+1)+')').remove();
         				existtagClsArrAdmin.unshift($tagText);
         				if (tagpos > -1) {
         					existtagClsArrAdmin.splice(tagpos, 1);
         				}
         				$(currElem).html(descController('FADE OUT',$tagText,weightArrAdmin[tagWeight-1]));
         				var selectedTag = $(currElem).parent().html();
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').prepend('<span class="tagscloud-term">'+selectedTag+'</span>');
         				$(currElem).html($tagText);
         			}
        	    }


        	  }
        	  catch (e) {
        	    // To Do
        	  }
        	},
        	showConfirmPopup: function(submitHandlerId, action, usersList,Type,isPricedClass) {
				try {
					var learners = $('input[name='+usersList+']').val().split(',');
					var getStatus = new Array();
					for (var i=0; i<learners.length; i++) {
						getStatus[i] = $('input[name=enrolled_status_'+learners[i]+']').val();
					}
					var unique = getStatus.filter(function(itm,i,getStatus){
						return i==getStatus.indexOf(itm);
					});
					var getdefaultStatus = new Array();
					for (var i=0; i<learners.length; i++) {
						getdefaultStatus[i] = $('input[name=enrolled_default_status_'+learners[i]+']').val();
					}
					var defltstat = getdefaultStatus.filter(function(itm,i,getdefaultStatus){
						return i==getdefaultStatus.indexOf(itm);
					});				
					 if(defltstat ==  Drupal.t('Completed') && submitHandlerId == 'commonsave_save-button'){
                     $('#'+submitHandlerId).click();
                     return;
                     }   
					var enrollmentStatus;	
					var getStsLength = unique.length;
                     if((isPricedClass == 1 ) && unique == Drupal.t('Canceled')){
                     	$('#show_expertus_message').html(expertus_error_message([Drupal.t('LBL1246')]));
                     	return;
                     }
                  if(getStsLength == 1 && (unique == Drupal.t('Waived')  || (unique == Drupal.t('Enrolled')  && submitHandlerId == 'commonsave_save-button') || (unique == Drupal.t('In progress')  && submitHandlerId == 'commonsave_save-button'))){
                     $('#'+submitHandlerId).click();
                     return;
                     }
					if($('input[name='+usersList+']').val() == "") {
						$('#show_expertus_message').html(expertus_error_message([Drupal.t('ERR106')]));
						return;
					} else if(learners.length == 1) {
						learners = $('#enroll_user_'+$('input[name='+usersList+']').val()).val();
					} else {
						learners = learners.length +" "+ Drupal.t('LBL3080');
					}			
					if(Type=='Class'){
					 var cancelTitleLT = Drupal.t('Class');
					}else{
					 var cancelTitleLT = Drupal.t('Training Plan');
					}
					switch(action) {
						case 'completedandsave':
							enrollmentStatus = Drupal.t('Completed');
						break;
						case 'incompletedandsave':
							enrollmentStatus = Drupal.t('Incomplete');
						break;
						case 'noshowandsave':
							enrollmentStatus = Drupal.t('No Show');
						break;
						case 'cancelledandsave':
							enrollmentStatus = Drupal.t('Canceled');
						break;
						default: enrollmentStatus = "Common";
						break;
					}
					//show jquery dialog with yes or cancel buttons with its action callbacks
					this.currTheme = Drupal.settings.ajaxPageState.theme;
					$('#complete-confirmation-wizard').remove();
					var html = '';
				    html += '<div id="complete-confirmation-wizard" style="display:none; padding: 13px;">';
				    html += '<table width="99%" class="complete-confirmation-table" cellpadding="0" cellspacing="0" border="0" valign="center">';
				    html += '<tr><td height="30"></td></tr>';
				    html += '<tr>';				     
					if(action == "commonsave"){
						if(getStsLength == 1 && learners.length >= 1){
							if(unique == "Completed"){
							html += '<td align="center" id="complete-confirmation-content"><span>'+ Drupal.t('MSG822', {'@learner_name': learners, '@status':Drupal.t('Completed'), '@objtype': cancelTitleLT}) +'</span></td>';
						}else if(unique == "Incomplete"){
							html += '<td align="center" id="complete-confirmation-content"><span>'+ Drupal.t('MSG822', {'@learner_name': learners, '@status':Drupal.t('Incomplete'), '@objtype': cancelTitleLT}) +'</span></td>';
						}else if(unique == "Canceled"){
							html += '<td align="center" id="complete-confirmation-content"><span>'+ Drupal.t('MSG822', {'@learner_name': learners, '@status':Drupal.t('Canceled'), '@objtype': cancelTitleLT}) +'</span></td>';
						}else if(unique == "No Show"){
							html += '<td align="center" id="complete-confirmation-content"><span>'+ Drupal.t('MSG822', {'@learner_name': learners, '@status':Drupal.t('No Show'), '@objtype': cancelTitleLT}) +'</span></td>';
						}
						}else{
							html += '<td align="center" id="complete-confirmation-content"><span>'+ Drupal.t('MSG821', {'@learner_name': learners, '@objtype': cancelTitleLT}) +'</span></td>';
						}	  
					}else{						
						html += '<td align="center" id="complete-confirmation-content"><span>'+ Drupal.t('MSG822', {'@learner_name': learners, '@status': enrollmentStatus, '@objtype': cancelTitleLT}) +'</span></td>';
					}
					html += '</tr>';
		    	   html +='</table>';
		    	   html +='</div>';
		    	   $("body").append(html);
					var confButton = {};
					confButton[Drupal.t('LBL109')] = function() {
						$(this).dialog('destroy');
						$(this).dialog('close');
						$('#complete-confirmation-wizard').remove();
					};
					confButton[Drupal.t('Yes')] = function(){
						$(this).dialog('destroy');
						$(this).dialog('close');
						$('#complete-confirmation-wizard').remove();
						$('#'+submitHandlerId).click();
					}
					var messageLine;
					if(enrollmentStatus == "Common") {
						if(getStsLength == 1 && learners.length >= 1){
							if(unique == "Completed"){
							var messageLine = Drupal.t('LBL3081') + " " +Drupal.t('Completed');
							}
							else if(unique == "Incomplete"){
								var messageLine = Drupal.t('LBL3081') + " " +Drupal.t('Incomplete');
							}
							else if(unique == "Canceled"){
								var messageLine = Drupal.t('LBL3081') + " " +Drupal.t('Canceled');
							}
							else if(unique == "No Show"){
								var messageLine = Drupal.t('LBL3081') + " " +Drupal.t('No Show');
							}
						}else {
							var messageLine = Drupal.t('LBL3082');
						}
					}else{
						var messageLine = Drupal.t('LBL3081') + " " +enrollmentStatus;
					}					
					$("#complete-confirmation-wizard").dialog({
						position:[(getWindowWidth()-500)/2,100],
						autoOpen: false,
						bgiframe: true,
						width: 500,
						resizable: false,
						modal: true,
						title: messageLine,
						buttons: confButton,
						closeOnEscape: false,
						draggable: false,
						zIndex : 10005,
						overlay: {
						   opacity: 0.9,
						   background: "black"
						}
					});
					
					$('.ui-dialog').wrap("<div id='complete-confirmation-wizard-dialog'></div>");
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
					
					changeDialogPopUI();
					$("#complete-confirmation-wizard").show();
					$("#complete-confirmation-wizard").dialog('open')
				} catch(e) {
				}
			},
			refreshLastAccessedRow: function (gridRow) {
				try {
					//console.log('refreshLastAccessedRow called');
					var rowFound = false;
					// console.log('refreshLastAccessedCatalogRow', console.trace());
					var grid = $("#narrow-search-results-holder");
					if (gridRow === undefined) {
						var gridRow = grid.jqGrid('getGridParam', 'lastAccessedRow');
					}
					//console.log(gridRow);
					if (gridRow !== undefined && gridRow != null && typeof gridRow.id != 'undefined') {
						//console.log(gridRow, gridRow.id);
						var rowData = ((typeof gridRow.id != 'undefined' && gridRow.id) ? gridRow.id.split('-') : null);
						var options = {
							data: {
								jqgrid_row_id: gridRow.id,
								page: 1,
								rows: 1
							}
						};
						grid.jqGrid('updateRowByRowId', options);
						rowFound = true; // return true to stop the grid reload
						grid.jqGrid('setGridParam', {lastAccessedRow: null});
					} else {
						// grid.trigger("reloadGrid",[{page:1}]);
						rowFound = false; // return false so that reload of grid happens as no last accessed grid row found
					}
				} catch (e) {
					//console.log(e, e.stack);
				}
				//console.log('rowFound ', rowFound, 'gridRow', gridRow);
				return rowFound;
			}
		});
	 $.extend($.ui.narrowsearch.prototype, EXPERTUS_SMARTPORTAL_AbstractManager,EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
		 window.paginationEnter = false;
			$('#add_lst_pg_txtfld').live('click onfocus mousedown',function(){
				$(this).keypress(function(event){
					 if (event.keyCode == 13) {
						 window.paginationEnter = true;
		              }
				});
			});
		$('.enable-edit-icon').live('click mousedown',function(){
			window.paginationEnter = false;
		});
		$('form').live('keyup keypress',function (e) {
		    var charCode = e.charCode || e.keyCode || e.which;
		    if (charCode  == 13 && e.target.nodeName != 'TEXTAREA') {
		        return false;
		    }
		});
		vtip();
	}catch(e){
			// To Do
		}
})(jQuery);
$(function() {
	try{
	$("#root-admin").narrowsearch();
	}catch(e){
			// To Do
		}
});
function editClassDetails(courseId, classId){
	try{
	if($(".qtip-active").length > 0)
		$(".qtip-active").remove();
	$('.course-class-edit-qtip').remove();
	var classVisibility = $('#catalog-class-basic-addedit-form-'+classId).is(':visible');
	$('#catalog-class-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').css('background-color','#FFFFFF');
	$('.edit-class-list').find('.admin-add-button-container').css('display','block');
	if(classVisibility == true){
		$('#catalog-class-basic-addedit-form-'+classId).hide();
	} else {
		$('#catalog-class-basic-addedit-form-'+classId).show();
		if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
			$('#edit-class-list-'+classId).css('background-color','');
		} else {
			$('#edit-class-list-'+classId).css('background-color','#d7ecf9');
		}
		//$('#edit-class-list-'+classId).find('.admin-add-button-container').css('display','none');
	}
	if($('#edit-catalog-course-basic-cancel')){
		$('#edit-catalog-course-basic-cancel').click();
		$('.qtip').find('.qtip-button > div').each(function(){
		    var x = $(this).attr('id');
			if(x.indexOf('cre_sys_obt_crs')>0 || x.indexOf('Course_close')>0 || x.indexOf('PrintCerId')>0){
			 $(this).closest('.qtip').remove();
			};
		});
	}
	}catch(e){
			// To Do
		}
}

function editClassQTipOpen(clickId,i){
	try{
	//Added by Vincent for issue #0015360: Class screen Qtip alignment is incorrect
	var y = $('#catalog-course-basic-addedit-form').height();
	var z = Math.round(y-433+(80*i));
	$('#modal-content').scrollTop(z);

	if($('#'+clickId).hasClass('loaded-edit-class')){
		$('#'+clickId).click();
	}
	$('#'+clickId).addClass('loaded-edit-class');
	}catch(e){
			// To Do
		}
}

function editCourseDetailsView(){
	try{
	$('#catalog-class-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').find('.admin-add-button-container').css('display','block');
    if($('#edit-newtheme-cancel-link')){
		$('#edit-newtheme-cancel-link').click();
	}
	}catch(e){
			// To Do
		}
}

/* added by ganesh #custom_attribute_0078975*/
function editCustomAttributeDetailsView(){
	try{
	$('#customattribute-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').find('.admin-add-button-container').css('display','block');
	if($('#edit-newtheme-cancel-link')){
		$('#edit-newtheme-cancel-link').click();
	}
	}catch(e){
			// To Do
		}
}

function editLocationDetailsView(){
	try{
	$('#resource-facility-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').find('.admin-add-button-container').css('display','block');
	if($('#edit-newtheme-cancel-link')){
		$('#edit-newtheme-cancel-link').click();
	}
	}catch(e){
			// To Do
		}
}

function editAttributesDetailsView(){
	try{
	$('#catalog-class-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').find('.admin-add-button-container').css('display','block');
	$('.admin-course-edit-button-container').hide();
	if($('#edit-newtheme-cancel-link')){
		$('#edit-newtheme-cancel-link').click();
	}
	}catch(e){
			// To Do
		}
}
function addClassDetails(courseId){
	try{
	var classVisibility = $('#catalog-class-basic-addedit-form-').is(':visible');
	$('.admin-course-edit-button-container').show();
	$('#catalog-class-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').css('background-color','#FFFFFF');
	if(classVisibility == true){
		$('#catalog-class-basic-addedit-form-').hide();
		$('#add-edit-class-norecords').show();
	} else {
		$('.edit-class-list').find('.admin-add-button-container').css('display','block');
		$('#catalog-class-basic-addedit-form-').show();
		$('#add-edit-class-norecords').hide();
	}
	if($('#edit-catalog-course-basic-cancel')){
		$('#edit-catalog-course-basic-cancel').click();
	}
	}catch(e){
			// To Do
		}
}


function addFacilityDetails(locationId){
	try{
	var facilityVisibility = $('#resource-facility-basic-addedit-form-').is(':visible');
	$('.admin-course-edit-button-container').show();
	$('#resource-facility-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').css('background-color','#FFFFFF');
	if(facilityVisibility == true){
		$('#resource-facility-basic-addedit-form-').hide();
		$('#add-edit-facility-norecords').show();
	} else {
		$('.edit-class-list').find('.admin-add-button-container').css('display','block');
		$('#resource-facility-basic-addedit-form-').show();
		$('#add-edit-facility-norecords').hide();
	}
	if($('#edit-resource-location-basic-cancel')){
		$('#edit-resource-location-basic-cancel').click();
	}
	}catch(e){
			// To Do
		}
}

function editFacilityDetails(locationId, facilityId){
	try{
	var facilityVisibility = $('#resource-facility-basic-addedit-form-'+facilityId).is(':visible');
	$('#resource-facility-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').css('background-color','#FFFFFF');
	$('.edit-class-list').find('.admin-add-button-container').css('display','block');
	if(facilityVisibility == true){
		$('#resource-facility-basic-addedit-form-'+facilityId).hide();
	} else {
		$('#resource-facility-basic-addedit-form-'+facilityId).show();
		if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
			$('#edit-facility-list-'+facilityId).css('background-color','');
		} else {
			$('#edit-facility-list-'+facilityId).css('background-color','#d7ecf9');
		}
		$('#edit-facility-list-'+facilityId).find('.admin-add-button-container').css('display','none');
	}
	if($('#edit-resource-location-basic-cancel')){
		$('#edit-resource-location-basic-cancel').click();
	}
	}catch(e){
			// To Do
		}
}


function displayResourceActionList() {
	try{
	$('.resource-add-list').toggle();
	}catch(e){
			// To Do
		}
}

function displayPubActionList(butnCls) {
	try{
	butnCls = typeof butnCls !== 'undefined' ? butnCls : '';
	if(butnCls!='')
		$('.'+butnCls).toggle();
	else
		$('.catalog-pub-add-list').toggle();
	}catch(e){
			// To Do
		}
}

function displayNarrowSearchAction(id,obj){
	try{
	$('.pub-action-list-'+id).toggle();
	$(obj).closest("tr").prevAll("tr").find("td").find(".narrowsearch-pub-add-list").hide();
	$(obj).closest("tr").nextAll("tr").find("td").find(".narrowsearch-pub-add-list").hide();
	}catch(e){
			// To Do
		}
}

function displayNotificationActionList(e) {
	try{
	$('.catalog-pub-add-list').toggle();
	/* 	0027406: Issue with Notifications */
	//event.stopPropagation();
	if(!e) var e = window.event;
	e.cancelBubble = true;
	e.returnValue = false;
	if ( e.stopPropagation ) e.stopPropagation();
	if ( e.preventDefault ) e.preventDefault();
       return false;
	}catch(e){
			// To Do
		}
}
function showHide(obj,uniqueId){
	try{
	var fieldsetId = $(obj).val();

	//var fieldsetId = $(obj).val();
    var fieldsetId = $(obj).parents('li').attr('id');
    //$(obj).find('option[value="'+fieldsetId+'"]').remove();
    /*
    if($(obj).find('option').length == 1){
        $(obj).remove();
        $('.showselect').remove();
    }
    */
    displayResourceActionList();
	if(fieldsetId=='attachment_fieldset') {
		$('#attachment-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='tag_fieldset') {
		$('#tag-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='prerequisite_fieldset') {
		$('#prerequisite-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='equivalence_fieldset') {
		$('#equivalence-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='survey_fieldset') {
		$('#survey-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='assessment_fieldset') {
		$('#assessment-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='session_detail_fieldset') {
		$('#session-details-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='wbt_detail_fieldset') {
		$('#wbt-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='register_fieldset') {
		$('#register-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='class_register_fieldset') {
		$('#classregister-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='custom_fieldset') {
		$('#custom-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='classroom_fieldset') {
		$('#classroom-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='equipment_fieldset') {
		$('#equipment-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}
	}catch(e){
			// To Do
		}
}

function selecteDropdown() {
	try{
	$(document).ready(function(){
		if (!$.browser.opera) {
			// select element styling
			$('select.select').each(function(){
				var title = $('.select').attr('title');
				if( $('option:selected', this).val() != ''  ) title = $('option:selected',this).text();
				$(this)
					.css({'z-index':10,'opacity':0,'-khtml-appearance':'none'})
					.after('<span class="showselect">' + title + '</span>')
					.change(function(){
						val = $('option:selected','.select').text();
						$('.select').next().text(val);
						});
			});

		};

	});
	}catch(e){
			// To Do
		}
}
function hideMessageInfo(){
	try{
    setTimeout(function() {
    	$(".messages").hide();
    	Drupal.ajax.prototype.commands.CtoolsModalAdjust(null, null, null);
	}, 10000);
	}catch(e){
			// To Do
		}
}
function increasePopWidth() {
	try{
	/* $("#modalContent > .ctools-modal-content").css('width','925px');
	$("#modal-content").css('width','890px');
	Drupal.ajax.prototype.commands.CtoolsModalAdjust(null, null, null); */
	}catch(e){
			// To Do
		}
}
function textfieldTitleBlur(obj, defaultValue, searchTypeHiddenId,sessionInsId){
	try{
	if(typeof(searchTypeHiddenId)==='undefined') searchTypeHiddenId = '';
	if(typeof(sessionInsId)==='undefined') sessionInsId = '';
	var searchTypeVal = '';
	if(searchTypeHiddenId != ''){
	  searchTypeVal	= $('#'+searchTypeHiddenId).val();
	}
	if(obj.value == '' || obj.value == defaultValue || obj.value == searchTypeVal){
		if(searchTypeVal != ''){
		  $(obj).val(searchTypeVal);
		}else{
		  $(obj).val(defaultValue);
		  if(sessionInsId != ''){
			  $('#hid_instructor_id').val('');
		  }
		}
		$(obj).addClass('input-field-grey');
	} else {
		$(obj).removeClass('input-field-grey');
	}
	}catch(e){
			// To Do
		}
}

function textfieldTitleClick(obj, defaultValue, searchTypeHiddenId){
	try{
	if(typeof(searchTypeHiddenId)==='undefined') searchTypeHiddenId = '';
	var searchTypeVal = '';
	if(searchTypeHiddenId != ''){
	  searchTypeVal	= $('#'+searchTypeHiddenId).val();
	}
	if(obj.value == defaultValue || (searchTypeVal != '' && obj.value == searchTypeVal)){
		$(obj).val('');
		$(obj).removeClass('input-field-grey');
	}
	}catch(e){
			// To Do
		}
}

// Attachment : While do click on to edit, the textbox class will change
function toEditInline(attachId,callEvent) {
	try{
	$("#"+attachId).addClass("edit-attachment-txtbox");
	$("#"+attachId).removeClass("non-editable-txt");
	$("#catalog-attachment-disp-container").find(".attachment-txt-box").each(function(){
	    var gId = $(this).attr("id");
	    if((gId  == attachId) && (callEvent == 'click')) {
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
			// To Do
		}
}

//Attachment : While do click on text label, the textbox should be appear with dottet line
function toEditAttachment(id,act) {
	try{
	var callEvent ='blur';
	if(act == 'label') {
		$("#attachment_list_label_"+id).hide();
		$("#attachment_list_edit_"+id).show();
		$("#attachment_list"+id).focus();
		callEvent = 'click';
	} else {
		$("#attachment_list_edit_"+id).hide();
		$("#attachment_list_label_"+id).show();
	}
	var attachId = 'attachment_list'+id;
	$("#"+attachId).addClass("edit-attachment-txtbox");
	$("#"+attachId).removeClass("non-editable-txt");
	$("#catalog-attachment-disp-container").find(".attachment-txt-box").each(function(){
	    var gId = $(this).attr("id");
	    if((gId  == attachId) && (callEvent == 'click')) {
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
			// To Do
		}
}

// Attachment: In Edit attachment, to prevent default enter key submit
function toEditAttachmentKeyDown(evt) {
	try{
	evt = evt || window.event;
    var charCode = evt.keyCode || evt.which;
    if (charCode == 13) {
    	$("#attachment_done_btn").click();
    	evt.preventDefault();
        return false;
    }
	}catch(e){
			// To Do
		}
}

function disableEnrterKey(evt){
	try{
	  var eve = evt || window.event;
	  var keycode = eve.keyCode || eve.which || eve.charCode;

	  if (keycode == 13) {
	    eve.cancelBubble = true;
	    eve.returnValue = false;

	    if (eve.stopPropagation) {
	      eve.stopPropagation();
	      eve.preventDefault();
	    }

	    return false;
	  }
	}catch(e){
			// To Do
		}
}

function textfieldTitleChange(obj) {
	try{
	var selectTxt = obj.options[obj.selectedIndex].text;
	if(selectTxt == 'Select'){
		$(obj).removeClass('select-normal-text');
		$(obj).addClass('select-greyed-out-text');
	} else {
		$(obj).removeClass('select-greyed-out-text');
		$(obj).addClass('select-normal-text');
	}
	}catch(e){
			// To Do
		}
}
/* function selectDropdownOnclick(){
	var insList = $('#load_multiselect_session_instructor').val();
	var selectedId = $('#edit-session-presenter :selected').val();
	if(insList != null && insList != undefined  && insList != ''){
			var opt ='';
			var url = "/?q=administration/session/allpresenter/"+insList;
			$.ajax({
				url : url,
				async: false,
				success: function(data) {
					var is_selected = false;
					$.each(data, function(){
						var selected = "";
						if(this.id == selectedId) {
							selected = "selected";
							is_selected = true;
						}
						opt += '<option title="'+this.full_name+'" value="'+this.id+'"  ' +selected+ '>'+this.full_name+'</option>';
					});
					var selCls = (is_selected === false) ? "selected" :"";
					opt = '<option title="Select" '+selCls+'> Select </option>'+opt;
					$('#edit-session-presenter').html(opt);
					//$('#change_instructor').val(1);
				}
			});
	}else{
		$('#edit-session-presenter').html('<option title="Select"  value="selected">Select</option>');
	}
	
} */
function textfieldTitleChangeField(fieldId) {
	try{
	var selectTxt = $('#'+fieldId+' option:selected').text();
	if(selectTxt == 'Select'){
		$('#'+fieldId).removeClass('select-normal-text');
		$('#'+fieldId).addClass('select-greyed-out-text');
	} else {
		$('#'+fieldId).removeClass('select-greyed-out-text');
		$('#'+fieldId).addClass('select-normal-text');
	}
	}catch(e){
			// To Do
		}
}

function remove_messages(msgType){
	try{
	if(msgType == 'class') {
		$('#catalog-class-addedit-form-details .messages').remove();
	} else if(msgType == 'course') {
		$('#catalog-course-basic-addedit-form .messages').remove();
	} else if(msgType == 'location'){
		$('#resource-location-basic-addedit-form .messages').remove();
	} else if(msgType == 'facility'){
		$('#resource-facility-basic-addedit-form .messages').remove();
	}else if(msgType == 'profile'){
		$('#exp-sp-my-profile-form').remove();
	}else if(msgType == 'customattribute') { //#custom_attribute_0078975
		$('#customattribute-basic-addedit-form .messages').remove();
	}
	}catch(e){
			// To Do
		}
}

function opt_sel_attach(id) {
	try{
	if($("#edit-attachment-fieldset-attachment-attachment-radioname-"+id+"-url").attr("checked")) {
		$("#attach_url_control"+id).show();
		$("#attach_browse_control"+id).hide();
	} else {
		$("#attach_url_control"+id).hide();
		$("#attach_browse_control"+id).show();
	}
	}catch(e){
			// To Do
		}
}

function getStatusReason(statusSelectField, uniqueId) {
  try{
	var selectId = $(statusSelectField).attr('id');
	var statusName = $('#' + selectId + ' option:selected').text().toLowerCase();

	if (statusName == 'inactive') {
		$('#admin-status-inactive-part-' + uniqueId).show();
	} else {
		$('#admin-status-inactive-part-' + uniqueId).hide();
	}

	Drupal.ajax.prototype.commands.CtoolsModalAdjust();
  }catch(e){
		// To Do
	}
}
function changeDelTypeInAddClass(delTypeCode){
	try{
	if(delTypeCode != 'lrn_cls_dty_ilt') {
		var locationId = $('.addedit-edit-new_location').attr('id');
		$('#'+locationId).val('');
		$('.addedit-edit-new_location').attr('id');
		$('#'+locationId).val('');
		var maxSeatId = $('.addedit-edit-max_seats').attr('id');
		$('#'+maxSeatId).val('');
	}
	}catch(e){
			// To Do
		}
}

function changeDeliveryType(delTypeCode){
 try{
	//var selectId = $(deliveryType).attr('id');
	//var delTypeCode = $('#'+selectId).val();
	if(delTypeCode == 'lrn_cls_dty_vod' || delTypeCode == 'lrn_cls_dty_wbt'){
		$('#admin-class-delivery-data-part').hide();
		$('#admin-class-delivery-data-part-for-date').hide();//ui-datepicker-trigger
		//$('#admin-class-register-date-container').find('#edit-reg-end-date').attr('readonly','readonly').addClass('admin-grey-out-fields');
		//$('#admin-class-register-date-container').find('.ui-datepicker-trigger').css('visibility','hidden');
		$('.addedit-edit-min_seats').attr('readonly','readonly').addClass('admin-grey-out-fields');
		$('.addedit-edit-max_seats').attr('readonly','readonly').addClass('admin-grey-out-fields');
		$('.addedit-edit-waitlist_count').attr('readonly','readonly').addClass('admin-grey-out-fields');
		$('#admin-class-location-container').find('#edit-class-location').attr('readonly','readonly').addClass('admin-grey-out-fields');
		$('.addedit-edit-class_location').attr('disabled',true);
		$('#two-col-row-lang_code_status_status_temp .addedit-twocol-secondcol .addedit-mandatory').css('visibility','hidden');
		$('#two-col-row-regdate_deadlinedate .addedit-twocol-secondcol .addedit-mandatory').css('visibility','hidden');
		$('#two-col-row-delivery_type_price .addedit-twocol-secondcol .addedit-mandatory').css('visibility','hidden');

	}
	else{
		$('#admin-class-delivery-data-part').show();
		$('#admin-class-delivery-data-part-for-date').show();
		//$('#admin-class-register-date-container').find('#edit-reg-end-date').removeAttr('readonly').removeClass('admin-grey-out-fields');
		//$('#admin-class-register-date-container').find('.ui-datepicker-trigger').css('visibility','visible');
		$('.addedit-edit-min_seats').removeAttr('readonly').removeClass('admin-grey-out-fields');
		$('.addedit-edit-max_seats').removeAttr('readonly').removeClass('admin-grey-out-fields');
		$('.addedit-edit-waitlist_count').removeAttr('readonly').removeClass('admin-grey-out-fields');
		//$('#admin-class-location-container').find('#edit-class-location').removeAttr('readonly').removeClass('admin-grey-out-fields');
		$('#two-col-row-regdate_deadlinedate .addedit-twocol-secondcol .addedit-mandatory').css('visibility','visible');
		$('#two-col-row-delivery_type_price .addedit-twocol-secondcol .addedit-mandatory').css('visibility','visible');

		if(delTypeCode == 'lrn_cls_dty_vcl'){
			$('#admin-class-location-container').find('#edit-class-location').attr('readonly','readonly').addClass('admin-grey-out-fields');
			$('.addedit-edit-class_location').attr('disabled',true);
			//$('#two-col-row-waitlist_count_currency_type').find('.addedit-mandatory').css('visibility','hidden');
			$('#two-col-row-lang_code_status_status_temp .addedit-twocol-secondcol .addedit-mandatory').css('visibility','hidden');
		}
		else if(delTypeCode == 'lrn_cls_dty_ilt') {
		  $('#admin-class-location-container').find('#edit-class-location').removeAttr('readonly').removeClass('admin-grey-out-fields');
		  $('.addedit-edit-class_location').attr('disabled',false);
			//$('#two-col-row-waitlist_count_currency_type').find('.addedit-mandatory').css('visibility','visible');
		  $('#two-col-row-lang_code_status_status_temp .addedit-twocol-secondcol .addedit-mandatory').css('visibility','visible');
		}
		else {
			$('.addedit-edit-class_location').attr('disabled',false);

			//$('#two-col-row-waitlist_count_currency_type').find('.addedit-mandatory').css('visibility','hidden');
			$('#two-col-row-lang_code_status_status_temp .addedit-twocol-secondcol .addedit-mandatory').css('visibility','hidden');
		}
	}
 }catch(e){
		// To Do
	}
}

function displayLongDescription(desc){
	try{
	if(desc == 'short_description') {
		var shortLength = $('.admin-course-class-short-description').find('.addedit-new-field-value').height();
		if(shortLength >= 50) {
				//$('.admin-course-class-short-description').find('.addedit-new-field-value').addClass('add-scroll-button-description');
		}
	}
	else if(desc == 'long_description') {
		var longLength = $('.admin-course-class-long-description').find('.addedit-new-field-value').height();
		if(longLength >= 50) {
				//$('.admin-course-class-long-description').find('.addedit-new-field-value').addClass('add-scroll-button-description');
		}
	}
	}catch(e){
			// To Do
		}
}

function limitTextareaChars(limitField, limitNum){
	try{
	var textValue = $('.textformat-textarea-editor').val();
	var newTextValue = '';
	var limitCount = '';
	if (textValue.length > limitNum) {
		newTextValue = textValue.substring(0, limitNum);
		$('.textformat-textarea-editor').val(newTextValue);
	} else {
		//limitCount = limitNum - textValue.length;
		limitCount = textValue.length;
		$('#char_count_' + limitField).html(limitCount);
	}
	}catch(e){
			// To Do
		}
}

//Added for Custom Attribute #custom_attribute_0078975
function limitCustomAttributeTextareaChars(limitField, limitNum){ 
	try{
	 //alert('limitCustomAttributeTextareaChars');
	var textValue = $('#'+limitField+'_textarea_full_row .textformat-textarea-editor').val();
	// alert('textValue='+textValue);
	var newTextValue = '';
	var limitCount = '';
	if (textValue.length > limitNum) {
		newTextValue = textValue.substring(0, limitNum);
		//$('.textformat-textarea-editor').val(newTextValue);
		$('#'+limitField+'_textarea_full_row .textformat-textarea-editor').val(newTextValue);
		$('#char_count_' + limitField).html(limitNum);
	} else {
		//limitCount = limitNum - textValue.length;
		limitCount = textValue.length;
		$('#char_count_' + limitField).html(limitCount);
	}
	}catch(e){
			// To Do
		}
}



//For MetaTag Feature
function limitTextFieldChars(limitField, limitNum){
	try{
	var textValue = $('.addedit-edit-textfield').val();
	var newTextValue = '';
	var limitCount = '';
	if (textValue.length > limitNum) {
		newTextValue = textValue.substring(0, limitNum);
		$('.addedit-edit-textfield').val(newTextValue);
	} else {
		//limitCount = limitNum - textValue.length;
		limitCount = textValue.length;
		$('#char_count_' + limitField).html(limitCount);
	}
	}catch(e){
			// To Do
		}
}

function fillCancelReason(uniqueId){
	try{
	var cancelReason = $('#cancel_reason_'+uniqueId).val();
	$('input[name="hidden_cancel_reason"]').val(cancelReason);
	$('#qtipIdqtip_class_cancel_disp_'+uniqueId).closest('.qtip-active').remove();
	}catch(e){
			// To Do
		}
}

(function($) {
	$.fn.refreshVersionList = function(contentId) {
		try{
		/* refresh version list on save or other action performed */
		//var pageNo = $(".ui-pg-input").val();
		$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");
		$("#admin-version-list-"+contentId+"-pagination").trigger("reloadGrid",[{page:1}]);
		}catch(e){
  			// To Do
  		}
	};
	
	$.fn.putCustomLabelForContentAuthor = function(){
		putCustomLabelForContentAuthorWrapper();
	};
	
	$.fn.initializePresentationObjects = function()
	{
		initializePresentationObjectsWrapper();	 
	
	}
	$.fn.hideUploadVideoControlAndDisplayLabel = function()
	{
	
		hideUploadVideoControlAndDisplayLabelWrapper();
	};
})(jQuery);


(function($) {
	$.fn.refreshMoveUsersQtip = function(contentId,versionId) {
		try{
		$('input[name="hidden_idlist_ContentMoveUsers-'+contentId+'-'+versionId+'"]').attr('value',0);
		/* refresh users list in move users qtip on save or other action performed */
		$("#datagrid-container-ContentMoveUsers-"+contentId+"-"+versionId).trigger("reloadGrid",[{page:1}]);
		}catch(e){
  			// To Do
  		}
	};

  $.fn.closeMoveUsersQtip = function() {
	  try{
    $(".qtip-active").remove();
    isPopupOpen=0;
	  }catch(e){
			// To Do
		}
  };
})(jQuery);

(function($) {
	$.fn.attachClose = function(entityId,entityType,type) {
		try{
		if(entityType == 'cre_sys_obt_trp'|| entityType == 'cre_sys_obt_cur' || entityType == 'cre_sys_obt_crt' || entityType == 'cre_sys_obt_trn') {
			var object_id = entityId.split('-');
			entityId = object_id[0];
			$('#qtipAttachCrsIdqtip_visible_disp_'+entityId+'_'+entityType).closest('.qtip-active').remove();
		}else if(entityType == 'cre_sec'){
			$('#qtipAttachIdqtip_addusers_visible_disp_'+entityId+'_'+entityType).closest('.qtip-active').remove();
		}else if(entityType == 'admin-order'){
			$(".active-qtip-div").remove();
		}else{
			$('#qtip_visible_disp_attach_question_'+entityId+'_'+entityType).closest('.qtip-active').remove();
		}
		if(entityType == 'cre_sys_obt_trp' || entityType == 'cre_sys_obt_cur' || entityType == 'cre_sys_obt_crt' || entityType == 'cre_sys_obt_trn'){
			var modId = object_id[1];
			var pos = ''; 
			if(modId != '' && modId != undefined && modId != 'undefined' && ($("#module-list-"+modId).index()>= 0)){
				pos = $("#module-list-"+modId).index();
				
			}else{
				//pos = ($(".first-arrow").size() > 0) ? 1 : 0;
				pos = $( "#page-container-tabs-prg .visible-main-tab:last" ).index();
			}
				if($( ".first-arrow" ).size() > 0 ){
					pos = pos-1;
				}
			$( "#page-container-tabs-prg" ).tabs('destroy');
			
	    	$( "#page-container-tabs-prg" ).tabs({
	    		selected: pos
	    	});
	    	
	    	/*Diable attach courses*/
	    	var str = $('#program_attach_tabs .ui-state-active').attr('id');
			var currmod = str.split('-');
			moduleTabclick(currmod[2]);
		}
		}catch(e){
  			// To Do
  		}
	};
})(jQuery);

(function($) {
	$.fn.validateCustomFields = function(customWrapperId,customErrorFlag) {
		try{
		if(customErrorFlag==true)
			$('#'+customWrapperId).show();
		}catch(e){
  			// To Do
  		}
	};
})(jQuery);

(function($) {
	try{
	$.fn.validateCertificate = function(defaultCertifyId) {
		$('input:radio[name="attach_certificate"]').filter('[value="'+defaultCertifyId+'"]').attr('checked', true);
		$('#certificate-display-table').jScrollPane({});
	};
	}catch(e){
			// To Do
		}
})(jQuery);

(function($) {
	try{
	$.fn.scrollPerm = function(permWrapperId) {
		$('#admin-add-scroll').jScrollPane({});
	};
	}catch(e){
			// To Do
		}
})(jQuery);

(function($) {
	try{
	$.fn.scrollTag = function(tagWrapperId) {
		$('#tag-scroll-id').jScrollPane({});
	};
	}catch(e){
			// To Do
		}
})(jQuery);

(function($) {
	try{
	$.fn.scrollView = function(argsList) {
		$("#view-scroll-wrapper").jScrollPane({});
		vtip();
		if($('#viewgroup-detail-wrapper').length) {	//to check group view form is open
			$('#modalContent .ctools-modal-content #ctools-face-table .popup-block-footer-right').css('right', '45px'); //57996
		}
	};
	}catch(e){
			// To Do
		}
})(jQuery);

(function($) {
	try{
	$.fn.cancelClassDetails = function(classId, editable) {
		$('#qtipAttachIdqtip_addlocation_visible_disp_'+classId).closest('.qtip-active').remove();

		if(document.getElementById('qtipAttachIdqtip_addclass_visible_disp_'+classId)){
		  $('#qtipAttachIdqtip_addclass_visible_disp_'+classId).closest('.qtip-active').remove();
		}
		if(document.getElementById('qtipAttachIdqtip_addclass_visible_disp_')){
			  $('#qtipAttachIdqtip_addclass_visible_disp_').closest('.qtip-active').remove();
		}
		if(document.getElementById('qtip_editclass_visible_dispid_'+classId)){
		  $('#qtip_editclass_visible_dispid_'+classId).closest('.qtip-active').remove();
		}
		if (!editable && editable != null && editable != undefined) {
			$("#edit-catalog-course-basic-cancel").click();
		}
		$('#catalog-class-addedit-form-details .addedit-form-wrapper').hide();
		$('.edit-class-list').css('background-color','#FFFFFF');
		$('.edit-class-list').find('.admin-add-button-container').css('display','block');
		$('#catalog-class-basic-addedit-form-'+classId).hide();
	};

	}catch(e){
			// To Do
		}
})(jQuery);

(function($) {
	$.fn.cancelFacilityDetails = function(facilityId) {
		try{
		$('#resource-facility-addedit-form-details .addedit-form-wrapper').hide();
		$('.edit-class-list').css('background-color','#FFFFFF');
		$('.edit-class-list').find('.admin-add-button-container').css('display','block');
		$('#resource-facility-basic-addedit-form-'+facilityId).hide();
		}catch(e){
  			// To Do
  		}
	};
})(jQuery);

function clearMessages(){
	try{
	$(".messages").hide();
	}catch(e){
			// To Do
		}
}

/*function attachmentUploadFile() {
  var provideUrlVisibility = $('#provide_url_id').is(':visible');

  if(provideUrlVisibility == true){
	 $("#provide_url_id").hide();
  }
  $("#attachment_upload_file").click();
}*/

function attachmentProvideUrl() {
	try{
	var provideUrlVisibility = $('#provide_url_id').is(':visible');

	if(provideUrlVisibility == true){
		$("#provide_url_id").hide();
	} else {
		$("#provide_url_id").show();
	}
	}catch(e){
			// To Do
		}
}
function bubblePopupClose(popupId) {
	try{
	$("#"+popupId).remove();
	}catch(e){
			// To Do
		}
}

function removebubblePopup(){
	try{
	$('.qtip').remove();
	}catch(e){
			// To Do
		}
}

function attachSubmitUrl() {
	try{
	//alert("Heree calling");
	// attachment_done_btn
	$("#attachment_done_btn").click();
	}catch(e){
			// To Do
		}
}

function validateCustomFieldClass() {
	try{
	$("#userbasic-custom-fields").find('.input-field-grey').each(function(){
	    var gId= $(this).attr("id");
	    var gVal = $("#"+gId).val();
	    if((gVal == 'Enter a Label') || (gVal == 'Enter a Value')) {
	        $("#"+gId).removeClass('input-field-grey');
	    	$("#"+gId).addClass('input-field-grey');
	    } else {
	    	$("#"+gId).removeClass('input-field-grey');
	    }
	});
	}catch(e){
			// To Do
		}
}


function sequenceDragAndDrop(containerId, type){
	try{
	if(type == 'attach_class_content'){
	  $("#"+containerId+' tbody').sortable({
		handle: '.dragndrop-selectable-item',
		cursor: 'crosshair'
	  });
	}
	else{
	  $("#"+containerId).sortable({
		handle: '.dragndrop-selectable-item',
		cursor: 'crosshair'
	  });
	}
	//$("#"+containerId).bind('click.sortable mousedown.sortable',function(ev){ ev.target.focus(); });
	//$("#"+containerId).disableSelection();
	$("#"+containerId).droppable({
		drop: function( event, ui ) {
			var sequenceOrder = '';
			var sequenceOrderArray = new Array();
			var c = 0;
			if(type == 'attach_class_content'){
				var currentId = $('#'+containerId+' tr.ui-sortable-helper').attr('id');
				var prevId = $('#'+containerId+' tr.ui-sortable-placeholder').prev().attr('id');
			}
			else{
				var currentId = $('#'+containerId+' li.ui-sortable-helper').attr('id');
				var prevId = $('#'+containerId+' li.ui-sortable-placeholder').prev().attr('id');
			}
			var recordStored = 0;
			if(prevId == undefined){
				sequenceOrderArray[0] = currentId;
				recordStored = 1;
				c = 1;
			}
			if(type == 'attach_class_content'){
				$('#'+containerId+' tr.ui-widget-content').each(function(){
					if($(this).attr('id') != currentId){
						sequenceOrderArray[c] = $(this).attr('id');
						if($(this).attr('id') == prevId){
							c++;
							sequenceOrderArray[c] = currentId;
						}
					}
					c++;
			   });
			}
			else{
				$('#'+containerId+' li.draggable-list').each(function(){
				if($(this).attr('id') != currentId){
					sequenceOrderArray[c] = $(this).attr('id');
					if($(this).attr('id') == prevId){
						c++;
						sequenceOrderArray[c] = currentId;
					}
				}
				c++;
				});
			}
			sequenceOrder = sequenceOrderArray.join(",");
			var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
			obj.createLoader(containerId);
			var i=1;
			$.ajax({
				url : obj.constructUrl("administration/sequence-drag-drop/"+type+"/"+sequenceOrder),
				async: false,
				success: function(data) {
					obj.destroyLoader(containerId);
					var sequenceArray = sequenceOrderArray.filter(function(v){return v!==''});
					var list_values = ($('#dragndrop li.draggable-list').length)-1;
					var arrLen = sequenceArray.length;
					if(list_values == arrLen ) {
						var i = 0;
						for(i=0; i<arrLen; i++) {
							$("#dragndrop li[id="+ sequenceArray[i] +"]").removeClass('odd-even-row-highlighter');
							if (i%2 == 1) {
								$("#dragndrop li[id="+ sequenceArray[i] +"]").addClass('odd-even-row-highlighter');
							}
						}
					}

				}
			});

		}
	});
	}catch(e){
			// To Do
		}
}
    $('#mandatory-list .man-opt-selection').live('click', function(){
    	try{
		if($(this).next().css('display') == 'block'){
			$(this).next().css('display', 'none');
		} else {
			$('.sub-menu').hide();
			$(this).next().css('display', 'block');
		}
    	}catch(e){
  			// To Do
  		}
	});


	$('#mandatory-list .sub-menu li').live('click', function(){
		try{
		var selectedMO = $(this).html();
		$(this).parent().prev('span').prev('span').html(selectedMO);
		$(this).parent().css('display', 'none');
		var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;

		var attachValues = $(this).attr('data');
		//alert(attachValues);
		var manValue = 1;
		obj.createLoader('attach_course_dt');
		$.ajax({
			url : obj.constructUrl("administration/update-attachedcourse-mandatory/"+attachValues),
			async: false,
			success: function(data) {
				obj.destroyLoader('attach_course_dt');
				$("#root-admin").data("narrowsearch").refreshGrid();
			}
		});

		return false;
		}catch(e){
  			// To Do
  		}
	});


    $('#attach-question-mandatory-list .man-opt-selection').live('click', function(){
    	try{
		if($(this).next().css('display') == 'block'){
			$(this).next().css('display', 'none');
		} else {
			$(this).next().css('display', 'block');
		}
    	}catch(e){
  			// To Do
  		}
	});


	$('#attach-question-mandatory-list .sub-menu li').live('click', function(){
		try{
		var selectedMO = $(this).html();
		$(this).parent().prev('span').prev('span').html(selectedMO);
		$(this).parent().css('display', 'none');
		var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;

		var attachValues = $(this).attr('data');
		//alert(attachValues);
		var manValue = 1;
		obj.createLoader('survey_attach_questions_dt');
		$.ajax({
			url : obj.constructUrl("administration/update-attachedquestion-mandatory/"+attachValues),
			async: false,
			success: function(data) {
				obj.destroyLoader('survey_attach_questions_dt');
			}
		});

		return false;
		}catch(e){
  			// To Do
  		}
	});


	(function($) {
		$('.addedit-customfields-wrapper .custom-field-label').live('dblclick', function(){
			try{
			var labelId = $(this).attr('id');
			var inputId = labelId.replace('custom_label_custom_value_', 'custom_input_custom_value_');
			$('#'+labelId).hide();
			$('#'+inputId).show();
			$('#'+inputId+' input').focus();
			}catch(e){
	  			// To Do
	  		}
		});
	})(jQuery);

	function updateCustomLabelKeyDown(evt, newValue, labelDetails, id, loaderWrapper){
		try{
		evt = evt || window.event;
	    var charCode = evt.keyCode || evt.which;
	    if (charCode == 13) {
	        updateCustomLabel(newValue, labelDetails, id, loaderWrapper);
	        evt.preventDefault();
	        return false;
	    }
		}catch(e){
  			// To Do
  		}
	}

	function updateCustomLabel(newValue, labelDetails, id, loaderWrapper) {
		try{
		var labelDetailsArray = labelDetails.split('-');
		var customId = labelDetailsArray[0];
		var entityId = labelDetailsArray[1];
		var entityType = labelDetailsArray[2];

		var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
		obj.createLoader(loaderWrapper);
		$.ajax({
			type: "POST",
			async: false,
			url: obj.constructUrl("administration/update-custom-fields/"+newValue+"/"+customId),
			data: '',
			success: function(result){
				$('#custom_label_custom_value_'+id).html(result);
				$('#custom_input_custom_value_'+id).hide();
				$('#custom_label_custom_value_'+id).attr('title', newValue);
				$('#custom_label_custom_value_'+id).show();
				obj.destroyLoader(loaderWrapper);
			},
			failure : function(){
				obj.destroyLoader(wrapper);
			}
	    });
		}catch(e){
  			// To Do
  		}
		    }
	function attachCrsDelete(callId) {
		try{
		if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
			var esignObj = {'esignFor':'attachCrsDelete','objectId':callId};
			$.fn.getNewEsignPopup(esignObj);
		}else{
			$("#"+callId).click();
		    }
		}catch(e){
  			// To Do
  		}
	}

	function setQtipPosition(){
		try{
		if(document.getElementById('qtip_position')!=null){
			if($('#qtip_position').val()!=''){
				try{
					var initVal = $('#qtip_position').val().split('#');
					var bubbleha = $("#"+initVal[0]).css('height');
					var bubbleTop = initVal[1];
					var bubblehb = initVal[2];
					bubbleha = parseInt(bubbleha.substring(0,bubbleha.length-2));
					bubbleTop = bubbleTop-(bubbleha-bubblehb);
					$("#"+initVal[0]).closest(".qtip-active").css('top',bubbleTop);
				}catch(e){}
			}
		}
		}catch(e){
  			// To Do
  		}
	}

	/**
	 * closeQtyp()
	 *
	 * This function has been added by Vincent on July 19, 2012.
	 * This will be called on clicking the qtip close icon form the top right corner of the Qtip
	 *
	 * @param id
	 * @return
	 */
	function closeQtyp(id,courseId){
		try{
		var tmp ;
		//reloadFlagForPagination=true;
		if(id.indexOf('addclass')>=0){
			tmp=0;
			 //Fixed for #0038166 : added to get hidden value before ctool get closed.
	        var emptyId = $('#bubble-face-table #empty_id').val();
			var entityType = $('#bubble-face-table #entity_value').val();
			if(emptyId != null && emptyId != undefined  && emptyId != ''){
		    	var url = "?q=administration/catalogaccess/delete/"+emptyId+"/"+entityType;
		    	$.ajax({
		 		   type: "POST",
		 		   url: url,
		 		   data:  '',
		 		   success: function(respText){
		 			  res = respText;
		 	  		}
		 		 });
		  	}
		}else{
			tmp = id.substring(id.indexOf('dispid_')+7);
		}
		if(id.indexOf('addclass')>=0 || id.indexOf('editclass')>=0){
		  //Drupal.ckeditorSubmitAjaxForm();
			$("#admin-course-class-list-pagination-wrapper").empty().remove();
			$('#paint-class-search-results-datagrid').empty().remove();
			var res = document.getElementById('enrollment_upload_done_btn_view');
			var result = document.getElementById('course_class_cancel_button');
			if(document.getElementById('attachment-addedit-form-html')){
					$('#attachment-addedit-form-html').attr('enctype','application/x-www-form-urlencoded');
				if(document.getElementById('attachment_upload_file')!=undefined && document.getElementById('attachment_upload_file')!=null && document.getElementById('attachment_upload_file')!='')
					$('#attachment_upload_file').remove();
			}
			else if(res !=null && res != undefined && res != '' ){
				var formname = document.getElementsByClassName('addedit-form');
				if (formname.length > 0)
						$('.addedit-form').attr('enctype','application/x-www-form-urlencoded');
				if(document.getElementById('enrollment_upload_file_view')!=undefined && document.getElementById('enrollment_upload_file_view')!=null && document.getElementById('enrollment_upload_file_view')!='')
					$('#enrollment_upload_file_view').remove();
				if(document.getElementById('enrollment_upload_file')!=undefined && document.getElementById('enrollment_upload_file')!=null && document.getElementById('enrollment_upload_file')!='')
					$('#enrollment_upload_file').remove();
			} else if(result !=null && result != undefined && result != '' ){
				var formname = document.getElementsByClassName('addedit-form');
				if (formname.length > 0)
					$('.addedit-form').attr('enctype','application/x-www-form-urlencoded');
				if(document.getElementById('enrollment_upload_file_view')!=undefined && document.getElementById('enrollment_upload_file_view')!=null && document.getElementById('enrollment_upload_file_view')!='')
					$('#enrollment_upload_file_view').remove();
				if(document.getElementById('enrollment_upload_file')!=undefined && document.getElementById('enrollment_upload_file')!=null && document.getElementById('enrollment_upload_file')!='')				
					$('#enrollment_upload_file').remove();
				if(document.getElementById('course_class_cancel_button')!=undefined && document.getElementById('course_class_cancel_button')!=null && document.getElementById('course_class_cancel_button')!='')
		 	        $("#course_class_cancel_button").click();
			}
		  	removeQtip(tmp);
		  	if(window.location.href.indexOf("admincalendar")>0)
			{
				$(".close").click();
				adminCalCloseHandler("classaddedit");
			}
		}
		if(id.indexOf('addtemplate') >=0 || id.indexOf('edittemplate') >=0 ){
			//alert(document.getElementById('notify_template_cancel_button'));
			$("#notify_template_cancel_button").click();
			removeClassQtip();
		}
		if(id.indexOf('MoveUsersqtip') >=0 ){
			var contentId = id.split("_");
			isPopupOpen=0;
			$.fn.refreshVersionList(contentId[3]);
		}
		$('#'+id+"_disp").html('');
		$('#'+id+"_disp").closest(".qtip-active").hide();
		
		if(id == "qtipAttachIdqtip_listvalues_visible_disp_0_cre_usr_dpt" || id == "qtipAttachIdqtip_listvalues_visible_disp_0_cre_usr_etp" || id == "qtipAttachIdqtip_listvalues_visible_disp_0_cre_usr_jrl"  || id == "qtipAttachIdqtip_listvalues_visible_disp_0_cre_usr_jtl" || id == "qtipAttachIdqtip_listvalues_visible_disp_0_cre_usr_ptp"){
			$('.page-administration-people-setup #add_lst_pg_next,.page-administration-people-setup #add_lst_pg_previous,.page-administration-people-setup #add_lst_pg_last,.page-administration-people-setup #add_lst_pg_first,.page-administration-people-setup #add_lst_pg_txtfld,.page-administration-people-setup #listvalues-autocomplete,.page-administration-people-setup #search_listvalues').removeAttr('style');								
		}
		
		
		}catch(e){
  			// To Do
  		}
	}

	/**
	 * setWordCountDiv()
	 *
	 * This function has been added by Vincent on July 24, 2012 to fix ckeditor word count issue(#0014481).
	 * It is drived form /ckeditor/wordcount/plugin.js to create word count div
	 *
	 * @param id
	 * @return
	 */
	function setWordCountDiv(id){
		try{
		setTimeout(function(){
			try{
				var editorRealName;
				var editor = CKEDITOR.instances[id];
				if(editor!=null && editor!=undefined && editor!='undefined'){
					editorRealName = (editor.element.$.name).split('[');
					if(!document.getElementById('cke_wordcount_'+editor.name)) {
			    		$('#char_count_'+editorRealName[0]).html('<div id="cke_wordcount_'+editor.name+'" style="display: inline-block; float: right; text-align: right; margin-top: 0px; cursor:auto; font:10px helvetica neue,helvetica,arial; height:auto; padding:0; text-align:left; text-decoration:none; vertical-align:baseline; white-space:nowrap; width:auto;color:#666666;">'+$('#char_count_'+editorRealName[0]).html()+'</div>');
			    	}
				}
			}catch(e){
				//Do nothing;
			}
		},4000);
		}catch(e){
  			// To Do
  		}
	}

	function removeAllQtip(){
		try{
		$('.qtip').remove();
		}catch(e){
  			// To Do
  		}
	}

	function removeQtip(id){
		try{
		//reloadFlagForPagination = true;
		if(id== -1){
			$('#qtipAttachIdqtip_addlocation_visible_disp__close').closest('.qtip').remove();
		}else if(id==0){
			$("#course_class_cancel_button").click();
			$('#qtipAttachIdqtip_addclass_visible_disp__close').closest('.qtip').remove();
			$('#qtipAttachIdqtip_addlocation_visible_disp__close').closest('.qtip').remove();
		}else{
			$("#course_class_cancel_button").click();
			$('#qtipAttachIdqtip_addlocation_visible_disp_'+id+'_close').closest('.qtip').remove();
		}
		removeClassQtip();
		}catch(e){
  			// To Do
  		}
	}

	function removeClassQtip(){
		try{
		$('.qtip').find('.qtip-button > div').each(function(){
		    var x = $(this).attr('id');
			if(x.indexOf('cre_sys_obt_cls')>0 || x.indexOf('Class_close')>0){
			 $(this).closest('.qtip').remove();
			};
		});
		}catch(e){
  			// To Do
  		}
	}


(function($) {
	$.fn.addCustomGrayConversion = function(customCount) {
		try{
		//alert(customCount);
		/*for(var i=0; i<customCount;i++){
			$('.addedit-edit-custom-label-field-'+i).addClass('input-field-grey');
			$('.addedit-edit-textarea-custom_value_'+i).addClass('input-field-grey');

		}*/
		$(".addedit-customfields-wrapper").find('.input-field-grey').each(function(){
		    var gId= $(this).attr("id");
		    var gVal = $("#"+gId).val();
		    if((gVal == 'Enter a Label') || (gVal == 'Enter a Value')) {
		        $("#"+gId).removeClass('input-field-grey');
		    	$("#"+gId).addClass('input-field-grey');
		    } else {
		    	$("#"+gId).removeClass('input-field-grey');
		    }
		});
		}catch(e){
  			// To Do
  		}
	};

	// Time picker
	/*$('input.exp-timepicker').live('click',function(){
		var attributeId = $(this).attr('id');
		var expDropDownId = 'exp-dropdown-'+attributeId;
		var hours, minutes;
		$('.exp-timepicker-selection').css('display', 'none');
		if($('#'+expDropDownId).length >0){
			if($('#'+expDropDownId).is(':visible') == true){
				$('#'+expDropDownId).css('display', 'none');
			} else {
	 			$('#'+expDropDownId).css('display', 'block');
			}
		} else {
			$(this).after('<div id="'+expDropDownId+'" data="'+attributeId+'" class="exp-timepicker-selection"><ul></ul></div>');
			for(var i = 0 ;i <= 1425; i += 15){
				hours = Math.floor(i / 60);
				minutes = i % 60;
				if (hours < 10){
				    hours = '0' + hours;
				}
				if (minutes < 10){
				    minutes = '0' + minutes;
				}
				$('#'+expDropDownId+' ul').append('<li>'+hours + ':' + minutes + '</li>');
		    }
		}
	});

	$('div.exp-timepicker-selection li').live('click', function(){
		var attributeId = $(this).closest('.exp-timepicker-selection').attr('data');
		var selectedTime = $(this).html();
		$('#'+attributeId).val(selectedTime);
		$(this).closest('.exp-timepicker-selection').css('display', 'none');
	});*/
	$('body').click(function(event){
		try{
		if($(event.target).hasClass('exp-timepicker') != true){
			$('.exp-timepicker-selection').css('display', 'none');
		}
		}catch(e){
  			// To Do
  		}
	});

})(jQuery);

//Hide the drop down menu option
  $('body,#admin-add-course-training-plan .ui-widget-content').bind('click', function(event) {
	  try{
	  if(event.target.id != 'admin-dropdown-arrow'){
		 $('#select-list-course-dropdown-list').hide();
		 $('#select-list-class-dropdown-list').hide();
		 $('#select-list-surass-dropdown-list').hide();
    }
	  }catch(e){
			// To Do
		}
  });
   //$('body').bind('mousedown', function(event) {
  //if((event.target.nodeName == 'SELECT') || (event.target.nodeName == 'INPUT' && ($(event.target).attr('type') == 'text'|| $(event.target).attr('type') == 'submit'))){
  $('body').bind('mousedown', function(event) {
	  try{
	  var nsParts = window.location.href.split("/?q=");
 	   if((event.target.nodeName == 'SELECT') || (event.target.nodeName == 'INPUT')
 			  || (event.target.nodeName == 'TEXTAREA') || (event.target.nodeName == 'A' && nsParts[1] !='administration/order/create')
 		  	  || (event.target.nodeName == 'SPAN' && nsParts[1] !='administration/order/create') || (event.target.nodeName == 'LI')){
		  $('#message-container').remove();
	  }
	  }catch(e){
			// To Do
		}
  });
/*$('#location-list .loc-disp-selection').live('click', function(){
	if($(this).next().css('display') == 'block'){
		$(this).next().css('display', 'none');
		$('.add-location-cls').css('display', 'none');
	} else {
		$(this).next().css('display', 'block');
		$('.add-location-cls').css('display', 'block');
	}
});

$('#location-list .sub-menu-list li').live('click', function(){
	var selectedMO = $(this).html();
	$(this).parent().prev('span').prev('span').html(selectedMO);
	$(this).parent().css('display', 'none');
	$('.add-location-cls').css('display', 'none');

	var attachValues = $(this).attr('data');
	//$('#class_location').val(attachValues);
	$('input[name$="class_location"]').val(attachValues);

	return false;
});*/

//$('#location-list .loc-disp-selection').live('click', function(){
$('#location-list .top-select').live('click', function(){
	try{
	if($('.sub-menu-list').css('display') == 'inline-block'){
		$('.sub-menu-list').css('display', 'none');
		$('.add-location-cls').css('display', 'none');
	} else {
		var actualWidthofUL = $('ul.sub-menu-list').width();
		if ($.browser.msie  && parseInt($.browser.version, 10) === 7) {
			var widthofUL = actualWidthofUL + 37;
		} else {
			var widthofUL = actualWidthofUL + 10;
			}
		$('.sub-menu-list').css('display', 'inline-block');
		$('.add-location-cls').css('width', +widthofUL+'px');
		$('.add-location-cls').css('display', 'block');
	}
	}catch(e){
			// To Do
		}
});

$('#location-list .sub-menu-list li').live('click', function(){
	try{
	var selectedMO = $(this).html();
	$('#location-input-txt').val(selectedMO);
	$(this).parent().css('display', 'none');
	$('.add-location-cls').css('display', 'none');

	var attachValues = $(this).attr('data');
	//$('#class_location').val(attachValues);
	$('input[name$="class_location"]').val(attachValues);

	return false;
	}catch(e){
			// To Do
		}
});

(function($) {
	try{
	$.fn.classCreateLocation = function(locId,locName,classId) {
		$('#newlocationpaint').append("<li data="+locId+" title='"+locName+"'>"+locName+"</li>");
		$('#location-list .sub-menu-list li ').click();
		sortUnorderedList("newlocationpaint");
/*		if(classId){
			$('#qtipAttachIdqtip_addlocation_visible_disp_'+classId).closest('.qtip-active').remove();
		}*/
	};
	}catch(e){
			// To Do
		}
})(jQuery);
function sortUnorderedList(ul, sortDescending) {
	try{
	  if(typeof ul == "string")
	    ul = document.getElementById(ul);
	  // Get the list items and setup an array for sorting
	  var lis = ul.getElementsByTagName("LI");
	  var vals = [];
	  // Populate the array
	  for(var i = 0, l = lis.length; i < l; i++)
	    vals.push(lis[i].innerHTML);
	  // Sort it
	  vals.sort();
	  if(sortDescending)
	    vals.reverse();
	  // Change the list on the page
	  for(var i = 0, l = lis.length; i < l; i++)
	    lis[i].innerHTML = vals[i];
	}catch(e){
			// To Do
		}
}
if(!document.getElementById('location-list')){
	try{
	$('body').click(function(event) {
		if($(event.target).hasClass('add-location-cls') != true){
			$('.sub-menu-list').css('display', 'none');
			$('.add-location-cls').css('display', 'none');
		}
	});
	}catch(e){
			// To Do
		}
}
function handlePostLoadDropdown(toBeHidden,toBeShown){
	try{
	$('#'+toBeHidden).hide();
	$('#'+toBeShown).show();
	}catch(e){
			// To Do
		}
}

function searchClassNameFilter(courseId,classId,ClassName){
	try{
	var obj = this;
	var pagerId	= '#admin-course-class-list-'+courseId+'-pagination_toppager';
	var objStr = '$("#root_admin").data("narrowsearch")';
	}catch(e){
			// To Do
		}
}

function onlyNumbers(evt)
{
	try{
     var charCode = (evt.which) ? evt.which : evt.keyCode;
     if (charCode == 8 || charCode == 9 || charCode == 46 || (charCode >= 37 && charCode <= 40) || (charCode >= 48 && charCode <= 57) || (charCode >= 96 && charCode <= 105))
    	 return true;
     else
     	return false;
     /*else if (charCode > 31 && (charCode < 48 || charCode > 57))
        return false;

     return true;*/
	}catch(e){
			// To Do
		}
}
/* Added by Gayathri for #0073578 */
function onlyNumbersforPrice(evt)
{
	try{
     var charCode = (evt.which) ? evt.which : evt.keyCode;
     if (charCode == 8 || charCode == 9 || charCode == 46 || charCode == 110 || (charCode >= 37 && charCode <= 40) || (charCode >= 48 && charCode <= 57) || (charCode >= 96 && charCode <= 105))
    	 return true;
     else
     	return false;    
	}catch(e){
			// To Do
	}
}
/* Set the focus on location field */
function changeLocationFocus(focusType){
	try{
  if(focusType=='deltype') {
		$('.addedit-edit-delivery_type').focusout();
	}else{
	     $('.addedit-edit-title').focusout();
	}
	}catch(e){
			// To Do
		}
}
/* Set the focus on first field */
function autoFocusFirstField() {
	try{
  $('form').find('select,textarea,input[type=text]').filter(':visible:first').focusout();
	}catch(e){
			// To Do
		}
}

/* Get Meeting details from popup tooltip and assign into form */
function getMeetingDetails(pos) {
	try{
	 /*var timezone = $('#timezone').val();
	 var session_attende_password = $('#session_attende_password').val();
	 var session_presenter_password = $('#session_presenter_password').val();
	 var session_attendee_url = $('#session_attendee_url').val();
	 var session_presenter_url = $('#session_presenter_url').val();
	 var instructorid = $('#hid_instructor_id').val();
	 var session_meeting_id = $('#session_meeting_id').val();

	 $('#time_zone_'+pos).val(timezone);
	 $('#session_attendeepass_'+pos).val(session_attende_password);
	 $('#session_presenterpass_'+pos).val(session_presenter_password);
	 $('#session_attendeeurl_'+pos).val(session_attendee_url);
	 $('#session_presenterurl_'+pos).val(session_presenter_url);
	 $('#hid_session_details_id_'+pos).val(instructorid);
	 $('#session_meetingid_'+pos).val(session_meeting_id);*/

	 $('.active-qtip-div').remove();
	}catch(e){
			// To Do
		}
}

function checkedAll(multiSel,uniqueId){
	try{
		if($(multiSel).attr('checked') == true){
			$('.attach-permission-cls').each(function(){
					$(this).attr('checked', true);
					$(this).parent('td').removeClass('checkbox-unselected').addClass('checkbox-selected');
			});
		} else {
			$('.attach-permission-cls').each(function(){
				$(this).attr('checked', false);
				$(this).parent('td').removeClass('checkbox-selected').addClass('checkbox-unselected');
			});

		}
	}catch(e){
			// To Do
		}
}
function uncheckAllPermission(){
	try{
	// If all checkbox selected - multiselect-all should be checked
	var multiselectVar = true;
	var removeClass = "checkbox-unselected";
	var addClass = "checkbox-selected";
	$('.attach-permission-cls').each(function(){
		if($(this).attr('checked') == false){
			multiselectVar = false;
			removeClass = "checkbox-selected";
			addClass = "checkbox-unselected";
		}
	});
	$('#select-all-checkbox').attr('checked', multiselectVar);
	$('#select-all-checkbox').parent().removeClass(removeClass);
	$('#select-all-checkbox').parent().addClass(addClass);
	}catch(e){
			// To Do
		}
}

function scorllMainTabPrev(){
	try{
	var liCount = $('#sort-bar-V2 .AdminsublinktabNavigation li').size();
	var activeLiLast = $('#sort-bar-V2 .visible-main-tab:first').index();
	if(activeLiLast==1){
		return '';
	}
	//if(liCount>activeLiLast){
		$('#sort-bar-V2 ul li:eq('+(activeLiLast-1)+')').removeClass('hidden-main-tab');
		$('#sort-bar-V2 ul li:eq('+(activeLiLast-1)+')').addClass('visible-main-tab');
		$('#sort-bar-V2 .visible-main-tab:last').removeClass('visible-main-tab').addClass('hidden-main-tab');
	//}
	}catch(e){
			// To Do
		}
}

function scorllMainTabNext(){
	try{
	var liCount = $('#sort-bar-V2 .AdminsublinktabNavigation li').size();
	var activeLiLast = $('#sort-bar-V2 .visible-main-tab:last').index();
	if(activeLiLast==(liCount-2)){
		return '';
	}
	if(liCount>activeLiLast){
		$('#sort-bar-V2 ul li:eq('+(activeLiLast+1)+')').removeClass('hidden-main-tab');
		$('#sort-bar-V2 ul li:eq('+(activeLiLast+1)+')').addClass('visible-main-tab');
		$('#sort-bar-V2 .visible-main-tab:first').removeClass('visible-main-tab').addClass('hidden-main-tab');
	}
	}catch(e){
			// To Do
		}
}

function resetMainTab(){
	try{
	var selected = $('#sort-bar-V2 .selected').index();
	var lastLi = $('#sort-bar-V2 .visible-main-tab:last').index();
	if($('#sort-bar-V2 .selected').css('display')=='none'){
		for(var i=lastLi;i<selected;i++){
			scorllMainTabNext();
		}
	}
	}catch(e){
			// To Do
		}
}

function deleteOrderLineItem(nid,enrid){
	try{
	$("input[name=lineitem_nid]:hidden").val(nid);
	$("input[name=lineitem_enrid]:hidden").val(enrid);
	$("input[name = 'pay_lineitem_canceled']").click();
	}catch(e){
			// To Do
		}
}

function checkboxSelectedUnselectedMultiParent(id){
	try{
	if($(id).is(':checked')){
		$(id).parents().eq(1).removeClass('survey-checkbox-unselected');
		$(id).parents().eq(1).addClass('survey-checkbox-selected');
	}
	else {
		$(id).parents().eq(1).removeClass('survey-checkbox-selected');
		$(id).parents().eq(1).addClass('survey-checkbox-unselected');
	}
	}catch(e){
			// To Do
		}
}

function checkboxSelectedUnselectedAddInf(id){
	try{
	if($(id).is(':checked')){
		$(id).parents().eq(1).removeClass('checkbox-unselected');
		$(id).parents().eq(1).addClass('checkbox-selected');
	}
	else {
		$(id).parents().eq(1).removeClass('checkbox-selected');
		$(id).parents().eq(1).addClass('checkbox-unselected');
	}
	}catch(e){
			// To Do
		}
}
jQuery(function($){
	try{
	var currLang = $('html').attr('xml:lang');
	$.datepicker.regional[currLang] = {
		prevText: Drupal.t('LBL692'),
		nextText: Drupal.t('LBL693')
		};
	$.datepicker.setDefaults($.datepicker.regional[currLang]);
	}catch(e){
			// To Do
		}
});

(function($) {
	try{
	$.fn.getScrollDive = function() {
		var heigt = $("#scrolldiv").height();
		if(heigt > 80){
			$('#catalog-attachment-disp-container #scrolldiv').css('height','100px');
			$('#catalog-attachment-disp-container #scrolldiv').jScrollPane({});
			}
			vtip();
	};
	}catch(e){
			// To Do
		}
})(jQuery);

(function($) {
	try{
	$.fn.cloneScrollPopup = function(classId) {
		var heigt = $('#clonescrolldiv'+classId).height();
		if(heigt > 80){
			//$('#clonescrolldiv').css('height','100px');
			$('#clonescrolldiv'+classId).jScrollPane({});
			}
	};
	}catch(e){
			// To Do
		}
})(jQuery);

//if (Drupal.settings.ajaxPageState.theme == 'expertusoneV2') {
 jQuery("#classdaterange-daterange-from-date,#classdaterange-daterange-to-date").live("click",function() {
	 try{
	jQuery("#ui-datepicker-div").parent('div').removeClass('datepicker-maincontainer');
	jQuery("#ui-datepicker-div").parent('div').removeClass('edit-datepicker-maincontainer');
	jQuery("#ui-datepicker-div").parent('div').removeClass('add-datepicker-maincontainer');
	jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
	jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
	jQuery("#ui-datepicker-div").parent('div').addClass('datepicker-maincontainer');
	 }catch(e){
			// To Do
		}
 });
 jQuery("#edit-reg-end-date,.addedit-edit-reg_end_date").live("click",function() {
	 try{
	 var rdoVal = $('.narrow-search-filterset-radio-selected').find('#radio_Class').val();
	 var rdoVal1 = $('.narrow-search-filterset-radio-selected').find('#radio_Course').val();

	 if(rdoVal =="Class")
	 {
	 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
	 jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
	 jQuery("#ui-datepicker-div").parent('div').addClass('edit-datepicker-maincontainer');
	 }
	 else if(rdoVal1 =="Course")
	 {
	 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
	 jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
	 jQuery("#ui-datepicker-div").parent('div').addClass('add-datepicker-maincontainer');
	 }
	 else
	 {
	 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
	 }
	 }catch(e){
			// To Do
		}
	});
jQuery("#start_date").live("click",function() {
	try{

	 var rdoVal2 = $('.narrow-search-filterset-radio-selected').find('#radio_Class').val();
	 var rdoVal3 = $('.narrow-search-filterset-radio-selected').find('#radio_Course').val();
	 //alert(rdoVal3);
	 if(rdoVal2 =="Class")
	 {
	 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
	 jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
	 jQuery("#ui-datepicker-div").parent('div').addClass('add-sessions-date');
	 $("#ui-datepicker-div").css('z-index','10000');
	 }
	 else if(rdoVal3 =="Course")
	 {
	 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
	 jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
	 jQuery("#ui-datepicker-div").parent('div').addClass('add-sessions-date');
	 $("#ui-datepicker-div").css('z-index','10000');
	 }
	}catch(e){
			// To Do
		}
});
function datepickerAddEdit(){
	try{

		 var rdoVal2 = $('.narrow-search-filterset-radio-selected').find('#radio_Class').val();
		 var rdoVal3 = $('.narrow-search-filterset-radio-selected').find('#radio_Course').val();
		 if(rdoVal2 =="Class"){
			 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
			 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
			 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
			 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
			 jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
			 jQuery("#ui-datepicker-div").parent('div').addClass('add-sessions-date');
			 $("#ui-datepicker-div").css('z-index','10000');
		 }
		 else if(rdoVal3 =="Course"){
			 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
			 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
			 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
			 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
			 jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
			 jQuery("#ui-datepicker-div").parent('div').addClass('add-sessions-date');
			 $("#ui-datepicker-div").css('z-index','10000');
		 }
		 $('#end_date , #start_date').bind('change',function() {
		  if($('#end_date').val()!= '' && $('#end_date').val() != undefined && $('#start_date').val() != '' && $('#start_date').val()!= undefined  && ($('#start_date').val() != 'mm-dd-yyyy' || $('#end_date').val() != 'mm-dd-yyyy')) {
			  if($('#start_date').val() == $('#end_date').val() ){
				  $(".weekday-checkbox-input").attr("checked", false);
				  $(".weekday-checkbox-input").parent().removeClass('checkbox-selected');
				  $(".weekday-checkbox-input").parent().addClass('checkbox-unselected');
				  $(".weekday-checkbox-input").attr("disabled", true);
				  $(".session_det_eachday").addClass('greyoutcheckbox');
     			  }
			  else{
				  $('.weekday-checkbox-input').removeAttr("disabled");
				  $(".session_det_eachday").removeClass('greyoutcheckbox');
			  	}
			  if($('#start_date').val()!= 'mm-dd-yyyy' && $('#end_date').val() != 'mm-dd-yyyy'){
			    var start = $('#start_date').val();
				var end = $('#end_date').val()
			    var countdays = getCountOf( start,end );
			    // console.log(countdays);
			     for (var key in countdays) {
				  if(countdays[key] ==0){
					 $("#"+key).attr("checked", false);
					  $("."+key).removeClass('checkbox-selected');
					  $("."+key).addClass('checkbox-unselected');
					  $("#"+key).attr("disabled", true);
					  $("#"+key+"-text").addClass('greyoutcheckbox');
				  	}
			     }
			}
		  }
		});	

	}catch(e){
			// To Do
	}
}


function parseDate(input) {
    var parts = input.split('-');
    return new Date(parts[2], parts[0]-1, parts[1]); 
  }
function getCountOf( date1, date2){
  var dateObj1 = parseDate(date1);
  var dateObj2 = parseDate(date2);
  var week = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"];
  var dayToSearch = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"];
  	var days_count = [];
  		for(var i = 0; i < dayToSearch.length; i++){
  				var dayIndex = week.indexOf( dayToSearch[i] );
  				var count = 0;
  					while ( dateObj1.getTime() <= dateObj2.getTime()){
  						if (dateObj1.getDay() == dayIndex )
  							{
  							   count++;
  								}       
  						dateObj1.setDate(dateObj1.getDate() + 1);
  					}
  					dateObj1 = parseDate(date1);
  					days_count[dayToSearch[i]] = count;
  		}
  return days_count;
}


//}
 $('body').live('click', function (event) {
	 try{
	 $('.add-session-popup-list').css("display","");
		if(event.target.className!='add-another-arrow-link') {
				$('.add-session-popup-list').hide();
			}
	 }catch(e){
			// To Do
		}
	});
$('.add-more-session-list li').live('click', function () {
	try{
	if(navigator.userAgent.indexOf("Chrome")>=0){
      var meetingTypeW=$('.session-add-another-popup-container').find('span.chosen-meeting-type').css("width");
   	  $(this).css('width',meetingTypeW);
	}
	}catch(e){
			// To Do
		}
 });

function updatePaginationCookie(p){
	try{
	var crPage = EXPERTUS_SMARTPORTAL_AbstractManager.getCookie('current_page');
	if(crPage!=''){
	var level = crPage.split('~');
	(p==1)?level[0] = level[0].replace('#0','#1'):level[1] = level[1].replace('#0','#1');
	var updatePage = level[0]+"~"+level[1]+"~"+level[2];
	document.cookie="current_page="+updatePage+"; path=/";
	}
	}catch(e){
			// To Do
		}
}

function manageDropdown(){
	try{
	showHideDropDown();
	var par = $('.grey-btn-bg-right');
	var position = par.position();
	$("#manage-dd-list" ).css( "left" , position.left-110);
    //$("#narrow-search-actionbar-list").find("#manage-dd-list").find("li:first").css('padding-bottom','5px');
    //$("#narrow-search-actionbar-list").find("#manage-dd-list").find("li:last").css('padding-top','5px');
	}catch(e){
			// To Do
		}
}
function createDropdown(){
	try{
	showHideDropDowncreate();
	var par = $('.pink-btn-bg-right');
	var position = par.position();
	$("#create-dd-list" ).css( "left" , position.left-75);
    //$("#narrow-search-actionbar-list").find("#manage-dd-list").find("li:first").css('padding-bottom','5px');
    //$("#narrow-search-actionbar-list").find("#manage-dd-list").find("li:last").css('padding-top','5px');
	}catch(e){
			// To Do
		}
}

/* Added for Custom Attribute - Ganesh  #custom_attribute_0078975 */
function createCustomAttributeDropdown(entity_type){ 
	try{  
		var ddlist_cls='create-dd-list-for-'+entity_type;
		var ddlist = $('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('visibility'); 
		
		if(ddlist == 'hidden'){
			$('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('visibility','visible');
			$('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('display','block');
		}else{
			$('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('visibility','hidden');
			$('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('display','none');
		}   
	}catch(e){
			// To Do
	}
}

//#custom_attribute_0078975
function clickCustomAttributeDropdown(attr_type,entity_type){
	try{   
		if(attr_type!=''){
		  var repl_text = '';
		  var cls_name='';
		  if(attr_type=='cattr_type_checkbox'){
		      repl_text = Drupal.t('LBL2006');
		      cls_name='new-customattribute-wrapper-checkbox';
		  }else if(attr_type=='cattr_type_dropdown'){
		      repl_text = Drupal.t('LBL2007');
		      cls_name='new-customattribute-wrapper-dropdown';
		  }else if(attr_type=='cattr_type_radio'){
		      repl_text = Drupal.t('LBL2008');
		      cls_name='new-customattribute-wrapper-radio';
		  }else if(attr_type=='cattr_type_txtarea'){
		      repl_text = Drupal.t('LBL2010');
		      cls_name='new-customattribute-wrapper-textarea';
		  }else if(attr_type=='cattr_type_txtbox'){
		     repl_text = Drupal.t('LBL2009');
		     cls_name='new-customattribute-wrapper-textbox'; 
		  }  
		  
		  if(repl_text!=''){
		    repl_text = Drupal.t('LBL287')+' '+repl_text;
		    // $('#new-customattribute-link').html(repl_text); 
		     $('.new-customattribute-link-for-'+entity_type).html(repl_text);
		     //$('.addedit-customattributes-wrapper').addClass(cls_name);
		     $('.addedit-customattributes-wrapper .addedit-form-cancel-container-actions').attr('id', cls_name);
		     var ddlist_cls='create-dd-list-for-'+entity_type;
		     $('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('display','none');
		     $('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('visibility','hidden');
		  } 
		 
		}  
		 
	}catch(e){
			// To Do
	}
}  

// Visibility pop up close  #custom_attribute_0078975
function closeCustomAttributeQtip(refreshOpt){ 
	try{ 
		$('.qtip-popup-visible').html('').hide();  
		if(refreshOpt=='1'){
		  $('.hidden-save-addedit-entityform').click();
		}   
	}catch(e){
		// To Do
	}
}

function orderQtipVisible(qtipObj){
	
	try{
						//alert('Visible Popup');
						var url = resource.base_host+'?q='+qtipObj.url;
						var popupId 	= qtipObj.popupDispId;
						//var catalogVisibleId = qtipObj.catalogVisibleId;
						var entId = qtipObj.entityId;
						var qtipScrollId = qtipObj.scrollid;

						$.ajax({
			 				 type: "GET",
				   	         url: url,
				   	         data:  '',
				   	         success: function(data){
			 					//var paintHtml = bpTop+"<div id='paintContent"+popupDispId+"'><div id='show_expertus_message' style='"+messageStyle+"'></div>"+data.render_content_main+"</div>"+bpBot;
			 					$('#'+popupId+' #visible-popup-'+entId+' #bubble-face-table .bubble-c #paintContent'+popupId).html(data.render_content_main);
			 					$.extend(true, Drupal.settings, data.drupal_settings);
			 					EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("paintContent"+popupId);

			 					if(qtipScrollId != '' && qtipScrollId != undefined && qtipScrollId != null){
			 						var qtipScrollType = (qtipObj.scrolltype == 'class') ? '.' : '#';
			 						$('#'+popupId+' '+qtipScrollType+qtipScrollId).jScrollPane('destroy');
			 						$('#'+popupId+' '+qtipScrollType+qtipScrollId).jScrollPane({});
			 					}
								Drupal.attachBehaviors();
			 					//vtip();
			 					
			 					/*
			 					if('[qtipObj.linkid^=visible-sharelink]') {
			 						if (navigator.appVersion.indexOf("Safari")!= -1 && ($(window).height() < 742))
			 							$('#program-tp-basic-addedit-form-container .survey-attach-grid-wrapper .ui-jqgrid .jqgrow #bubble-face-table td.bubble-c').css('height','33px');
			 								 						
									if(Drupal.settings.user.language !== undefined && $.inArray(Drupal.settings.user.language, ["pt-pt", "de", "fr"]) != -1) {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-30px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-15px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ru') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-33px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-15px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ja') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-30px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-15px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ko') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-26px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-3px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'zh-hans') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-54px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-32px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-42px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it') { 
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-3px');
										else
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'es') { 
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'en-us') { 
										if ((navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1) || (navigator.appVersion.indexOf("Trident/7.0")!= -1))
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-3px');
										else
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									}else {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-28px');
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									}
								}
								*/
								
		   	                 }
			   			});

					}catch(e){
						alert(e);
					}
	
}
/* End for Custom attribute */

function showHideDropDown(){
	try{
	var ddlist = $('#manage-dd-list').css('visibility');
	if(ddlist == 'hidden'){
		$('#manage-dd-list').css('visibility','visible');
		$('#download_template').css('visibility','hidden');
                $('#download_template_enroll').css('visibility','hidden');
	}else{
		$('#manage-dd-list').css('visibility','hidden');
		$('#download_template').css('visibility','hidden');
                $('#download_template_enroll').css('visibility','hidden');
	}
	}catch(e){
			// To Do
		}
}
function showHideDropDowncreate(){
	try{
	var ddlist = $('#create-dd-list').css('visibility');
	if(ddlist == 'hidden'){
		$('#create-dd-list').css('visibility','visible');
	}else{
		$('#create-dd-list').css('visibility','hidden');
	}
	}catch(e){
			// To Do
		}
}
$('body').live('click', function (event) {
	try{
	$('#manage-dd-list').css("display","");
	if(event.target.className == 'info-user-upload vtip') {
		$('#manage-dd-list').css('visibility','visible');
		$('#manage-dd-list').css('display','block');
		$('#manage-dd-list').show();
	}else if(event.target.className == 'info-roster-upload vtip'){
                $('#manage-dd-list').css('visibility','visible');
		$('#manage-dd-list').css('display','block');
		$('#manage-dd-list').show();
	}else if(event.target.className!='grey-btn-bg-right') {
		$('#manage-dd-list').css('visibility','hidden');
		$('#manage-dd-list').hide();
	}
	$('#create-dd-list').css("display","");
	if(event.target.className!='pink-btn-bg-right' && event.target.className!='customattr-btn-bg-right'){ //Added for custom attribute by ganesh - #custom_attribute_0078975
		$("#create-dd-list" ).hide();
	}
	}catch(e){
			// To Do
		}
});


$('body').live('click', function (event) {
	try{
		classList = event.target.className.split(" ");
		if(event.target.className !='form-checkbox' && event.target.className !='tab-title' && event.target.className !='manage-clone-list' && event.target.className !='clone-label-list' && event.target.className != 'clone-list-class' && event.target.className != 'select-all-wrapper'
			&& classList[0] != ('jspDrag') && event.target.className != 'jspTrack') {
			$('.manage-clone-list').hide();
			$('.manage-clone-list').css('visibility','hidden');
		}
	}catch(e){
			// To Do
		}
});
$(".searchtext, #narrow-search-text-filter, #search_leaderboard-users_txt, #search_users_txt, #classname-autocomplete, .narrow-search-ac-text-overlap, .searchtag, .searchlocation, #tpattchedcoursename-autocomplete, #username-search-autocomplete, #surassattchedquestions-autocomplete, #countrylist-autocomplete, #enrolluser-autocomplete, #enrolltpuser-autocomplete,#prerequisite-autocomplete,#equivalence-autocomplete,#survey-autocomplete,#assessment-autocomplete,#srch_criteria_catkeyword,#report_table_autocomplete").live("click",function(){
   try{
	if(Drupal.settings.ajaxPageState.theme == 'expertusone' && ($.browser.msie && $.browser.version > 7) )
	 {
       var loaderEl = this;
       var widgetId=$(this).attr("id");
       var filterSearchClsname=$(this).attr("class");
       var filterSearchCls = filterSearchClsname.split(" ");
       $(loaderEl).val("");
       $(loaderEl).parent("div").css("position","relative");
       //reports
       $(loaderEl).closest("div.report-table-search-container").css("position","relative");
       // Top search;
	   $(loaderEl).next(".loader-hidder").remove();
	   $(".loader-hidder").remove();
	   //widget Search
	   $(loaderEl).next(".widget-loader-hidder").remove();
	   $(".widget-loader-hidder").remove();
	   //narrow filter search
	   $(loaderEl).next(".narrowfilter-loader-hidder").remove();
	   $(".narrowfilter-loader-hidder").remove();
	   //course/class search
	   $(loaderEl).next(".crecls-loader-hidder").remove();
	   $(".crecls-loader-hidder").remove();
	   //Country setting search
	   $(loaderEl).next(".countrysett-loader-hidder").remove();
	   $(".countrysett-loader-hidder").remove();
	   //select class search
	   $(loaderEl).next(".selectCls-loader-hidder").remove();
	   $(".selectCls-loader-hidder").remove();
	   //report design wizard left table search
	   $(loaderEl).next(".reportsLTbl-loader-hidder").remove();
	   $(".reportsLTbl-loader-hidder").remove();

	   $(loaderEl).after( "<div class='loader-hidder'>&nbsp;</div>" );

	   loaderEl.onkeydown = function(evt) {
           evt = evt || window.event;
          // alert("keydown: " + evt.keyCode);
          $(".loader-hidder,.widget-loader-hidder, .crecls-loader-hidder, .narrowfilter-loader-hidder, .selectCls-loader-hidder").css({"background-position":"100% -17px","cursor":"none"});
           if(widgetId=="countrylist-autocomplete" || widgetId=="enrolluser-autocomplete" || widgetId=="enrolltpuser-autocomplete" || widgetId=="prerequisite-autocomplete" || widgetId=="equivalence-autocomplete" || widgetId=="survey-autocomplete" || widgetId=="assessment-autocomplete"){
           $(".countrysett-loader-hidder").css({"background-position":"99% -16px","cursor":"none"});
           }
           if(filterSearchCls[1]=="narrow-search-ac-text-overlap" || filterSearchCls[1]=="searchtag" || filterSearchCls[1]=="searchlocation" ){
           	$(".narrowfilter-loader-hidder").css({"background-position":"100% -17px","cursor":"none"});
           }
           };
       loaderEl.onkeyup = function(evt) {
           evt = evt || window.event;
           // alert("keyup: " + evt.keyCode);
           $(".loader-hidder, .widget-loader-hidder, .crecls-loader-hidder, .narrowfilter-loader-hidder, .selectCls-loader-hidder").css("background-position","100% 3px");
           if(widgetId=="countrylist-autocomplete" || widgetId=="enrolluser-autocomplete" || widgetId=="enrolltpuser-autocomplete" || widgetId=="prerequisite-autocomplete" || widgetId=="equivalence-autocomplete" || widgetId=="survey-autocomplete" || widgetId=="assessment-autocomplete"){
             $(".countrysett-loader-hidder").css({"background-position":"99% 5px","cursor":"none"});
           }
           if(filterSearchCls[1]=="narrow-search-ac-text-overlap" || filterSearchCls[1]=="searchtag" || filterSearchCls[1]=="searchlocation" ){
        	 $(".narrowfilter-loader-hidder").css({"background-position":"100% 3px","cursor":"none"});
           }
          };

          if(widgetId=="countrylist-autocomplete" || widgetId=="enrolluser-autocomplete" || widgetId=="enrolltpuser-autocomplete" || widgetId=="prerequisite-autocomplete" || widgetId=="equivalence-autocomplete" || widgetId=="survey-autocomplete" || widgetId=="assessment-autocomplete"){
        	//$(".loader-hidder").css({"right":"22px","top":"1px","height":"22px"});
        	$(".loader-hidder").addClass("countrysett-loader-hidder");
      	    $(".countrysett-loader-hidder").removeClass("loader-hidder");
      	    if(widgetId=="enrolluser-autocomplete" || widgetId=="enrolltpuser-autocomplete"){
      	      $(".countrysett-loader-hidder").css({"right":"1px"}) ;
      	    }
      	    if(widgetId=="prerequisite-autocomplete" || widgetId=="equivalence-autocomplete" || widgetId=="survey-autocomplete" || widgetId=="assessment-autocomplete"){
      	      $(".countrysett-loader-hidder").css({"right":"1px"}) ;
      	    }
          }
          if(widgetId=="search_leaderboard-users_txt" || widgetId=="search_users_txt"){
        	  $(".loader-hidder").addClass("widget-loader-hidder");
        	  $(".widget-loader-hidder").removeClass("loader-hidder");
          }

          if(widgetId=="classname-autocomplete" || widgetId=="tpattchedcoursename-autocomplete" || widgetId=="username-search-autocomplete" || widgetId=="surassattchedquestions-autocomplete" ){
        	  $(".loader-hidder").addClass("crecls-loader-hidder");
        	  $(".crecls-loader-hidder").removeClass("loader-hidder");
        	  if(widgetId=="tpattchedcoursename-autocomplete"){
        	  $(".crecls-loader-hidder").css({"right":"1px","top":"-10px"});
        	  }
        	  if(widgetId=="surassattchedquestions-autocomplete"){
              $(".crecls-loader-hidder").css({"right":"56px","top":"-10px"});
              }
        	 /* if(widgetId=="username-search-autocomplete") {
        	  $(".crecls-loader-hidder").css("right","108px");
        	  }*/
        	  if(widgetId=="classname-autocomplete") {
              $(".crecls-loader-hidder").css("right","23px");
              }
          }
         if(widgetId=="srch_criteria_catkeyword"){
          $(".loader-hidder").addClass("selectCls-loader-hidder");
       	  $(".selectCls-loader-hidder").removeClass("loader-hidder");
         }
        if(widgetId=="report_table_autocomplete"){
             $(".loader-hidder").addClass("reportsLTbl-loader-hidder");
          	 $(".reportsLTbl-loader-hidder").removeClass("loader-hidder");
            }

          if(filterSearchCls[1]=="narrow-search-ac-text-overlap" || filterSearchCls[1]=="searchtag" || filterSearchCls[1]=="searchlocation" ){
        	 $(".loader-hidder").addClass("narrowfilter-loader-hidder");
        	 $(".narrowfilter-loader-hidder").removeClass("loader-hidder");
        	 if(filterSearchCls[1]=="searchtag" || filterSearchCls[1]=="searchlocation"){
        	 $(".narrowfilter-loader-hidder").css({"right":"21px","height":"19px"});
        	 }
          }
        }
   }catch(e){
			// To Do
		}
});
Drupal.ajax.prototype.commands.saveandclosePopup = function(ajax, response, status) {
	   try {
		   $(".active-qtip-div").remove();
		   $("#popup_container_qtip_buisness_disp_"+response.entities).closest(".qtip-active").remove();
		   $("#qtipAccessqtip_visible_disp_"+response.entities).closest(".qtip-active").remove();
	   }
	   catch(e) {
		   //nothing to do
	   }
};
Drupal.ajax.prototype.commands.displaymessagewizard = function(ajax, response, status) {
    try {
    	this.currTheme = Drupal.settings.ajaxPageState.theme;
    	var dupId = response.grpId;
	    $('#delete-msg-wizard').remove();
	    var html = '';
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';

    		var title = Drupal.t('MSG711');

	    if(this.currTheme == 'expertusoneV2'){
	   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
	    } else {
	     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+'</td></tr>';
	    }
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);

	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){  $('input[id="show_catag"]').attr('value',0); $("input[name = 'saveandshow']").click();$('#delete-msg-wizard').remove();};

         if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
	    	var esignObj = {'popupDiv':'catalog-publish-unpublis-dialog',
	    			'esignFor':'displaymessagewizard','catalogId':dupId,'catalogType': 'Class','actionStatus': 'lrn_cls_sts_atv', 'rowObj': this};
	    	closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj);$(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};
         }else{
        	 
        	 closeButt[Drupal.t('Yes')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();
	        	 setTimeout(function() {
	     			$('#visible-class-'+dupId).click()
	     		}, 300);
        	 };
         }

	    $("#delete-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:300,
	        resizable:false,
	        modal: true,
	        title:Drupal.t('LBL749'),
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
	    $(".removebutton").text(Drupal.t("No"));
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

	    if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			 /*$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');*/
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
	    }
	    $("#delete-msg-wizard").show();

		$('.ui-dialog-titlebar-close').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		if(this.currTheme == 'expertusoneV2'){
	    	changeDialogPopUI();
	    }
    }
    catch(e){
      //Nothing to do
    }
  };


function orderEdit(){
	 try {
	  setTimeout(function() {
		var orderRow=$(".payment-billing-details-info .order-review-table tr,.payment-user-details-info .order-review-table tr").length;
		for(var i=0; i < orderRow; i++){
			var billH = $(".payment-billing-details-info .order-review-table tr:eq("+i+")").height();
			var userH = $(".payment-user-details-info .order-review-table tr:eq("+i+")").height();
			var maxH = (billH < userH) ? userH : billH;
			$(".payment-billing-details-info .order-review-table tr:eq("+i+"), .payment-user-details-info .order-review-table tr:eq("+i+")").height(maxH);
		}
	  }, 1000);
	  vtip();
	}catch(e) {
		   //nothing to do
	   }
}

function checkPaginationWidth(){
	try {
	if($("#admin-data-grid .ui-pg-table .ui-pg-table").is(":visible")){
	  var pageConWidth=$("#admin-data-grid .ui-pg-table .ui-pg-table").width();
	   $("#exportcontainer").css("right",pageConWidth+15);
	   if(document.getElementById('enrollment-upload-container') !== undefined) {
		if(Drupal.settings.ajaxPageState.theme != "expertusoneV2" && pageConWidth > 0) {
			$('.admin_ac_input_mainform.addedit-edit-enrolluser-autocomplete').width(195);
	 }
		$("#enrollment-upload-container").css("right",pageConWidth+15);
	   }
	 }
	}catch(e) {
		   //nothing to do
	   }
	};

(function($) {
	$.fn.cloneDestroyloader = function() {
		try {
			$("#root-admin").data("narrowsearch").destroyLoader('paint-narrow-search-results');
		} catch (e) {
			// To Do
		}
	};
})(jQuery);


Drupal.ajax.prototype.commands.showClonePopup = function(ajax, response, status) {

    try {
    	classId = response.class_id;
    	$('#manage-clone-list'+classId).html(response.html);

    	showHideCreateCopyDropDown(classId);
    	var par = $('.grey-btn-bg-right-create-copy');
    	var position = par.position();
    	this.currTheme = Drupal.settings.ajaxPageState.theme;
    	if(this.currTheme == "expertusoneV2"){
    	$("#ul-clone-list-"+classId ).css( "right" ,'-25px');
    	}else{
    		$("#ul-clone-list-"+classId ).css( "right" ,'-25px');
    		$("#ul-clone-list-"+classId ).css( "top" ,'30px');
    	}
		    	
    	$(".contentauthorclone").parent().parent().parent().parent().css("width","180px");
		$(".contentauthorclone .jspContainer").css("width","180px");
		$(".contentauthorclone .jspContainer .jspPane").css("width","170px;");
		$(".contentauthorclone .jspContainer .jspPane").css("height","66px;");
		$(".contentauthorclone").closest(".manage-clone-list").css("width","170px;");
		$(". contentauthorclone").css("width","200px");


		
    	Drupal.attachBehaviors();
    	vtip();
    }
    catch(e){
      //Nothing to do
    }
};

function showHideCreateCopyDropDown(entityId){
	try{
		$('.manage-clone-list').hide();
		// Qtippopup Disable
		$('#group-control').remove();
		$('.qtip-popup-visible').hide();

		var ddlist = $('#manage-clone-list' + entityId).css('visibility');
		if (ddlist == 'hidden') {
			$('#ul-clone-list-' + entityId).css('visibility', 'visible');
			$('#ul-clone-list-' + entityId).show();
		} else {
			$('#ul-clone-list-' + entityId).css('visibility', 'hidden');
		}
	}catch(e){
			// To Do
		}
}

function checkboxDisableForCloneCA(obj)
{
	$('#clone-Content').prop('checked','checked');
	$('#clone-Content').parent().removeClass('checkbox-unselected');
	$('#clone-Content').parent().addClass('checkbox-selected');

	if($(obj).attr("id") == "clone-Interactions" && $(obj).is(":checked") == true)
	{
		$('#clone-Interactions').prop('checked','checked');
		$('#clone-Interactions').parent().removeClass('checkbox-unselected');
		$('#clone-Interactions').parent().addClass('checkbox-selected');
		
		$('#clone-Content').prop('checked','checked');
		$('#clone-Content').parent().removeClass('checkbox-unselected');
		$('#clone-Content').parent().addClass('checkbox-selected');
	}else if($(obj).attr("id") == "clone-Interactions" && $(obj).is(":checked") == false)
	{
		$('#clone-Interactions').parent().removeClass('checkbox-selected');
		$('#clone-Interactions').parent().addClass('checkbox-unselected');
		$('#clone-Interactions').prop('checked','');
	}
	
	
	if($(obj).attr("id") == "clone-access" && $(obj).is(":checked") == true)
	{
		$('#clone-access').prop('checked','checked');
		$('#clone-access').parent().removeClass('checkbox-unselected');
		$('#clone-access').parent().addClass('checkbox-selected');
		
		$('#clone-Content').prop('checked','checked');
		$('#clone-Content').parent().removeClass('checkbox-unselected');
		$('#clone-Content').parent().addClass('checkbox-selected');
	}else if($(obj).attr("id") == "clone-access" && $(obj).is(":checked") == false)
	{
		$('#clone-access').parent().removeClass('checkbox-selected');
		$('#clone-access').parent().addClass('checkbox-unselected');
		$('#clone-access').prop('checked','');
	}
	
	/*if($(obj).attr("id") == "clone-Content" && $(obj).is(":checked") == true)
	{
		$('#clone-Content').prop('checked','checked');
		$('#clone-Content').parent().removeClass('checkbox-unselected');
		$('#clone-Content').parent().addClass('checkbox-selected');
	}else if($(obj).attr("id") == "clone-Content" && $(obj).is(":checked") == false)
	{
		$('#clone-Content').prop('checked','');
		$('#clone-Content').parent().removeClass('checkbox-selected');
		$('#clone-Content').parent().addClass('checkbox-unselected');
		
		$('#clone-Interactions').prop('checked','');
		$('#clone-Interactions').parent().addClass('checkbox-unselected');
		$('#clone-Interactions').parent().removeClass('checkbox-selected');
		
		
		
	}*/
	
	//{
	
	//}
/*	if($('#clone-Content').is(':checked')){
		$('#clone-Interactions').attr('disabled','');
		//$('#clone-Interactions').parent().addClass('checkbox-selected');
		//$('#clone-Interactions').parent().removeClass('checkbox-unselected');
		
	}
	else
	{
		//alert($('#clone-Interactions').size());
		
		$('#clone-Interactions').attr('checked','');
		$('#clone-Interactions').parent().addClass('checkbox-unselected');
		$('#clone-Interactions').parent().removeClass('checkbox-selected');
		$('#clone-Interactions').attr('disabled','disabled');
		
		
		
	}*/
	
	$(".jspPane").css("left","0px");
}

function checkboxDisableForClone(type,id){
	if(type == 'class'){
	if(!$('#clone-content_list').is(':checked')){
		$('#clone-enrollment').parent().removeClass('checkbox-selected');
		$('#clone-enrollment').parent().addClass('checkbox-unselected');
		$('#clone-enrollment').attr('checked','');
	}
	}else{
		if(!$('#ul-clone-list-'+id+' #clone-attachedcourses').is(':checked')){
			$('#ul-clone-list-'+id+' #clone-assessment').parent().removeClass('checkbox-selected');
			$('#ul-clone-list-'+id+' #clone-assessment').parent().addClass('checkbox-unselected');
			$('#ul-clone-list-'+id+' #clone-assessment').attr('checked','');
			
			$('#ul-clone-list-'+id+' #clone-survey').parent().removeClass('checkbox-selected');
			$('#ul-clone-list-'+id+' #clone-survey').parent().addClass('checkbox-unselected');
			$('#ul-clone-list-'+id+' #clone-survey').attr('checked','');
		}
	}
}
function checkboxAutoEchckForContentClone(type,id){
	if(type == 'class'){
	if($('#clone-enrollment').is(':checked')){
		$('#clone-content_list').attr('checked','checked');
		$('#clone-content_list').parent().removeClass('checkbox-unselected');
		$('#clone-content_list').parent().addClass('checkbox-selected');
	}
	}else{
		if($('#ul-clone-list-'+id+' #clone-assessment').is(':checked') || $('#ul-clone-list-'+id+' #clone-survey').is(':checked')){
			$('#ul-clone-list-'+id+' #clone-attachedcourses').attr('checked','checked');
			$('#ul-clone-list-'+id+' #clone-attachedcourses').parent().removeClass('checkbox-unselected');
			$('#ul-clone-list-'+id+' #clone-attachedcourses').parent().addClass('checkbox-selected');
}
	}
}

//Added by ganesh for custom attribute #custom_attribute_0078975
Drupal.ajax.prototype.commands.displayScreenWarningWizard = function(ajax, response, status) {
    try { 
        
    	this.currTheme = Drupal.settings.ajaxPageState.theme;
    	//var dupId = response.grpId; 
    	var name =  unescape(response.name).replace(/"/g, '&quot;');   
    	
		$('#delete-msg-wizard').remove();
	    var html = '';
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    	var title = name;

	    if(this.currTheme == 'expertusoneV2'){
	   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
	    } else {
	     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+'</td></tr>';
	    }
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
 
	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

         if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
              closeButt[Drupal.t('Yes')]=function(){
            	  var esignObj = {'popupDiv':'displayScreenWarningWizard','esignFor':'addScreenDetails'};
	                      $.fn.getNewEsignPopup(esignObj); $(this).dialog('destroy');$(this).dialog('close');};
         }else{
        	 closeButt[Drupal.t('Yes')]=function(){  
        	   
        	   $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();
     		   $("#delete-msg-wizard").remove();  
			   $("#hidden-screen-button").click(); 
        	};
         }

	    $("#delete-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:300,
	        resizable:false,
	        modal: true,
	        title:Drupal.t("LBL2015"),//"title",
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
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

	    if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			/* $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');*/
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
	    }
	    $("#delete-msg-wizard").show();

		$('.ui-dialog-titlebar-close').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		if(this.currTheme == 'expertusoneV2'){
	    	changeDialogPopUI();
	    } 
			    
	      
	     
	   
    }
    catch(e){
      //Nothing to do
    }
  };

Drupal.ajax.prototype.commands.displayWarningWizard = function(ajax, response, status) {
    try {
    	this.currTheme = Drupal.settings.ajaxPageState.theme;
    	var dupId = response.grpId;
    	var name =  unescape(response.name).replace(/"/g, '&quot;');

	    $('#delete-msg-wizard').remove();
	    var html = '';
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    	var title = name;

	    if(this.currTheme == 'expertusoneV2'){
	   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
	    } else {
	     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+'</td></tr>';
	    }
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);

	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

         if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
              closeButt[Drupal.t('Yes')]=function(){
            	  var esignObj = {'popupDiv':'displayWarningWizard','esignFor':'displayWarningWizard', 'dupId':dupId, 'rowObj': this};
	                      $.fn.getNewEsignPopup(esignObj); $(this).dialog('destroy');$(this).dialog('close');};
         }else{
        	 closeButt[Drupal.t('Yes')]=function(){ callInsertProcess(dupId);};
         }

	    $("#delete-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:300,
	        resizable:false,
	        modal: true,
	        title:Drupal.t("Group"),//"title",
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
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

	    if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			/* $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');*/
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
	    }
	    $("#delete-msg-wizard").show();

		$('.ui-dialog-titlebar-close').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		if(this.currTheme == 'expertusoneV2'){
	    	changeDialogPopUI();
	    }
    }
    catch(e){
      //Nothing to do
    }
  };
  function callInsertProcess(dupId,esign){
	  esign = typeof esign !== 'undefined' ? esign : '';
	  $('input[id="grp_war"]').attr('value',1);
	  $('input[id="grp_war_grpId"]').attr('value',dupId);
	  if(esign == 'esign'){
		  $("#esign-role-addedit-form").click();
	  }else{
		  $("input[name = 'grp-save']").click();
	  }
	  $('#delete-msg-wizard').remove();
	}

function callVisibility(qtipObj){
	
	if(typeof(qtipObj.dispGrpView) == 'undefined') qtipObj.dispGrpView='';
	if(typeof(qtipObj.catalogVisibleId) == 'undefined') qtipObj.catalogVisibleId='';
	
	if(document.getElementsByClassName('qtip-close-button') && qtipObj.catalogVisibleId.indexOf("AttachIdqtip_add_grp") <= 0){
	$('.qtip-close-button').click();
}	
	//console.log(paginationEnter);
	if(paginationEnter)
		return false;
	// Clone Onclick Disable
	var ddlist = $('#manage-clone-list' + qtipObj.entityId).css('visibility');
	if (ddlist == 'hidden') {
		$('#ul-clone-list-' + qtipObj.entityId).css('visibility', 'visible');
		$('#ul-clone-list-' + qtipObj.entityId).show();
	} else {
		$('#ul-clone-list-' + qtipObj.entityId).css('visibility', 'hidden');
	}
	$('.manage-clone-list').hide();
	
	if(qtipObj.dispGrpView == 'Y'){ // Added for this ticket #0042617 to show the Hidden tab

		var lId = $('#viewgroup-detail-wrapper #view-scroll-wrapper');
		var mypos = $('#' + qtipObj.popupDispId); // Declaring ID
		 // Popup Calculation
		var lheg = lId.height();
		var popoff = lId.offset();
		 // Action link Calculation
		var newlheg = mypos.height();
		var newpopoff = mypos.offset();

		var gridHeight = (lheg/2) + popoff.top;
		var linkHeight = newlheg + newpopoff.top;

		if(gridHeight < linkHeight){
			qtipObj.dispDown = '';
			if((linkHeight - gridHeight) > 100){
				lId.css("overflow","hidden");
				$('#viewgroup-detail-wrapper #view-scroll-wrapper .jspContainer').css("overflow","hidden");
			}else{
				lId.css("overflow","visible");
				$('#viewgroup-detail-wrapper #view-scroll-wrapper .jspContainer').css("overflow","visible");
			}
		}else{
			lId.css("overflow","visible");
			$('#viewgroup-detail-wrapper #view-scroll-wrapper .jspContainer').css("overflow","visible");
		}
	}else {
		$('#viewgroup-detail-wrapper #view-scroll-wrapper').css("overflow","hidden");
		$('#viewgroup-detail-wrapper #view-scroll-wrapper .jspContainer').css("overflow","hidden");
	}
	$('#group-control').remove();
	//console.log(qtipObj.catalogVisibleId);
	//console.log(qtipObj.catalogVisibleId.indexOf("AttachCrsIdqtip_visible_disp"));
	if(qtipObj.catalogVisibleId.indexOf("AttachCrsIdqtip_visible_disp") > 0 || qtipObj.catalogVisibleId.indexOf("AttachIdqtip_add_grp") > 0 ){
		var str = $('#program_attach_tabs .ui-state-active').attr('id');
		var currmod = str.split('-');
		//console.log(" string " + str);
		qtipObj.url = qtipObj.url + '/' + currmod[2] + '/' + $('#attach_course_dt form input[name="form_build_id"]').val(); 
	}
	if(qtipObj.catalogVisibleId.indexOf("AttachIdqtip_history") <= 0){
	$('.qtip-popup-visible').html('').hide();
	}else{
		$('.qtip-popup-history').html('').hide();
	}
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
	setTimeout(function(){
		EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("paintContent"+ qtipObj.popupDispId);
		if('[qtipObj.linkid^=visible-sharelink]') {
			$('#sharelink_disp_div .survey-sharelink-main-wrapper .loadercontent .loaderimg').css('margin-top','-42px');
			$('.qtip_survey_sharelink .loadercontent .loaderimg').css('margin-top','-8px');
		}
	},10);
	//#custom_attribute_0078975
	if(qtipObj.entityType=='order'){ //For custom attribute order
		orderQtipVisible(qtipObj); // For Order	
	}else{
	$("#root-admin").data("narrowsearch").visiblePopup(qtipObj);
}
	
	 
}
// Visibility pop up close
function closeQtip(popupId,entId,onClsFn){
	try{
	
	   //#custom_attribute_0078975 Refresh the association when click on close button from show-in-screen Qtip
	   if(popupId.indexOf('qtip_showinscreen_dis')>=0){ 
	     $('#root-admin').data('narrowsearch').refreshLastAccessedRow();
	   }
	   
		if(typeof(onClsFn) == 'undefined') onClsFn='';
		if(popupId == '')
			$('.qtip-popup-visible').html('').hide();
		else
			$('#'+popupId+' #visible-popup-'+entId).html('').hide();

		if(typeof(onClsFn) == 'function') {
			onClsFn();
		}
	}catch(e){

	}
}
function checkCountryDisable(){
  if($('#add_grp_Country').is(':checked')){
	 $('#add_grp_State').attr('disabled', false);
  }else {
	 $('#add_grp_State').parent().removeClass('checkbox-selected');
	 $('#add_grp_State').parent().addClass('checkbox-unselected');
	 $('#add_grp_State').attr('checked','');
	 $('#add_grp_State').attr('disabled',true);
  }
}
function stateDisabledMessage(){
 if(!($('#add_grp_Country').is(':checked'))){
	 var error = new Array();
	 error[0] = Drupal.t('select a country');
	 var message_call = expertus_error_message(error,'error');
	 $('#show_expertus_message').show();
	 $('#show_expertus_message').html(message_call);
  }
}

function addslashes(string) {
    return string.replace(/'/g, '\\\'');
}

function removeCurrencyPopup(codeval,symbol){
	this.currTheme = Drupal.settings.ajaxPageState.theme;
	var uniqueClassPopup = '';
	 $('#delete-msg-wizard').remove();
	    var html = '';
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    if(this.currTheme == 'expertusoneV2'){
	   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+Drupal.t("MSG747")+' "'+codeval +' <span class="currency-override-bold">'+ symbol+'</span>"?'+'</td></tr>';
	    } else {
	     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+Drupal.t("MSG747")+' "'+ codeval +' <span class="currency-override-bold">'+ symbol +'</span>"?'+'</td></tr>';
	    }
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);

	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

		 closeButt[Drupal.t('Yes')]=function(){
			 $("#root-admin").data("narrowsearch").createLoader("currency-detail-wrapper");
			 removeaddedcurrency(codeval);
		 };
	    $("#delete-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:300,
	        resizable:false,
	        modal: true,
	        title:Drupal.t('LBL749'),
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
	    $('.ui-dialog').wrap("<div id='catalog-publish-unpublis-dialog' class='"+uniqueClassPopup+"'></div>");
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    $(".removebutton").text(Drupal.t("No"));
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
	    $("#delete-msg-wizard").show();

		$('.ui-dialog-titlebar-close').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		$('.admin-save-button-middle-bg').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		if(this.currTheme == 'expertusoneV2'){
	    	changeDialogPopUI();
	    }
}

function removeaddedcurrency(val) {
      $.ajax({
       	type: "GET",
       	url: "/?q=administration/sitesetup/config/currency/remove/"+val,
       	success: function(data) {
       			if(data=='success'){
       				$('.row-'+val).remove();
       				$(".currency-table-heading tbody tr").removeClass( "odd-list-class" );
       				$(".currency-table-heading tbody tr").removeClass( "even-list-class" );
       				$(".currency-table-heading tbody tr.rowtbody:nth-child(odd)").addClass("even-list-class");
       			    $(".currency-table-heading tbody tr.rowtbody:nth-child(even)").addClass("odd-list-class");
       			    dynamicWidthHeight('delete');
       			    $("#root-admin").data("narrowsearch").destroyLoader("currency-detail-wrapper");
       			}
       	}
       	});
}
function checkboxSelectedUnselectedCurrency(id){
	try{
		if($(id).attr('type')!='radio'){
			if($(id).is(':checked')){
				$(id).parent().removeClass('checkbox-unselected');
				$(id).parent().addClass('checkbox-selected');
			}
			else {
				$(id).parent().removeClass('checkbox-selected');
				$(id).parent().addClass('checkbox-unselected');
			}
			
		}
		var totalcheckbox = $('#currency-td-table .multichk').length;
    	var cheklen = $('#currency-td-table .checkbox-selected').length;
    	if(totalcheckbox==cheklen){
    		$('#currency_select').attr('checked', true);
    		$('.currency-muliselect').removeClass('checkbox-unselected').addClass('checkbox-selected');
    		$('.multichk').removeClass('checkbox-unselected').addClass('checkbox-selected');
    	} else {
    		$('#currency_select').attr('checked', false);
    		$('.currency-muliselect').removeClass('checkbox-selected').addClass('checkbox-unselected');
    	}	
	}catch(e){
		// to do
	}
}
function Currencycheckedall(){
	try{
		    $("#currency_select").change(function(){
		    	$(".attach-group-list").prop('checked', $(this).prop("checked"));			    	
		    	if($('#currency_select').attr('checked')) {
		    		$(this).attr('checked', true);
		    		$('.currency-muliselect').removeClass('checkbox-unselected').addClass('checkbox-selected');
		    		$('.multichk').removeClass('checkbox-unselected').addClass('checkbox-selected');
		    	} else{
		    		$(this).attr('checked', false);
		    		$('.currency-muliselect').removeClass('checkbox-selected').addClass('checkbox-unselected');
		    		$('.multichk').removeClass('checkbox-selected').addClass('checkbox-unselected');		    		
		    	}	
		    	return false;
		    });
	}

	catch(e){

	}
}
function dynamicWidthHeight(callfrom){
	  var scrol_height = '';
	  var table_height = $('#multi_currency_setup_div .currency-table-heading').height();
	  var table_width = $('#multi_currency_setup_div .currency-table-heading').width();
	  //console.log(table_height);
	  //console.log(table_width+'here');
	  if(table_height > 310 && table_width > 800){
		  var new_width = 825;
		  var new_height = 378;
		  $('#modal-content').css({"width":new_width,'height':new_height});
		  $('#tableholder').css({'height': '307px', 'width': '790px'});
		  $('#multi_currency_setup_div').css({'height': '307px', 'width': '790px', 'overflow': 'hidden'});
		  $("#multi_currency_setup_div").jScrollPane();
	  }else if(table_height <150 && table_width <165){
		  $('#modal-content').css({'height': '150px', 'width': '295px'});
		  $('.addedit-form-cancel-and-save-actions-row').css({'width':'90%','margin-left': '40px'});
		  $('#tableholder').css({'height': '60px', 'width': '240px'});
		  $('#multi_currency_setup_div').css({'height': '60px', 'width': '240px'});
		  $('.currency-table-heading').css({'width': '245px'});
		  if($('.rowtbody').length<=0){
			  $('.currency-table-heading').remove();
			  $('#multi_currency_setup_div').append("<div width=\"130px\" height=\"100px\" class=\"currency-table-heading admin-empty-text-msg\" style=\"border:0px;\">"+Drupal.t('MSG746')+"</div>" );
		  }

	  } else{
		  if($('.rowtbody').length<=0){
			  $('.currency-table-heading').remove();
			  $('#multi_currency_setup_div').append("<div width=\"130px\" height=\"100px\" class=\"currency-table-heading admin-empty-text-msg\" style=\"border:0px;\">"+Drupal.t('MSG746')+"</div>" );
		  }
		  //console.log('here');
		  var new_width = table_width +50;
		  var new_height = table_height+72;
		  table_width = table_width +15;
		  $('#modal-content').css({"width":new_width,'height':new_height});
		  $('#tableholder').css({'height': table_height+'px', 'width': table_width+'px'});
		  $('#multi_currency_setup_div').css({'height': table_height+'px', 'width': table_width+'px'});
		  var element = $('#multi_currency_setup_div').jScrollPane({});
		  var api = element.data('jsp');
		  api.destroy();
	  }
	  $('#modalContent').removeAttr( 'style' );
	  $('#modalContent').css({"z-index":"1001"});
	  $('#modalContent').css("position","absolute");
	  $('#modalContent').css("top", Math.max(0, (($(window).height() - $('#tableholder').outerHeight()) / 2) +  $(window).scrollTop()) + "px");
	  //console.log(Math.max(0, (($(window).width() - $('#tableholder').outerWidth()) / 2) +  $(window).scrollLeft()) + "px");
	  $('#modalContent').css("left", Math.max(0, (($(window).width() - $('#tableholder').outerWidth()) / 2) +  $(window).scrollLeft()) + "px");
}

(function($) {
	$.fn.dynamicWidthHeightAdd = function(){
	try{
		dynamicWidthHeight(false);
	}catch(e){
			// To Do
		}
	};

})(jQuery);


function dynamicstylechange(callfrom){
	var rowCount = $('.sf-table-heading tr').length;
	var consumer_sec_key = $( "input[name='consumer_secret_key']" ).val();
	var consumer_key = $( "input[name='consumer_key']" ).val();
	var sf_id = $( "input[name='expertusone_sf_id']" ).val();
	if(consumer_sec_key == ''){
		$( "input[name='consumer_secret_key']" ).addClass( "sfrow-error-check" );
	} if(consumer_key == ''){
		$( "input[name='consumer_key']" ).addClass( "sfrow-error-check" );
	} if(sf_id == ''){
		$( "input[name='expertusone_sf_id']" ).addClass( "sfrow-error-check" );
	}
	for(var i=0;i<rowCount-1;i++){
		//alert('#system_user_'+i);
		var system_user = $('#system_user_'+i).val();
		var system_pasword = $('#system_user_pwd_'+i).val();
		if(system_user == ''){
			$('#system_user_'+i).addClass( "sf-table-heading check-attributes > input sfrow-error-check" );
		}
		if(system_pasword == ''){
			$('#system_user_pwd_'+i).addClass( "sf-table-heading check-attributes > input sfrow-error-check" );
		}
	}
}

(function($) {
	$.fn.dynamicstylechangeAdd = function(){
	try{
		dynamicstylechange(false);
	}catch(e){
			// To Do
		}
	};

})(jQuery);

//0053694: SF-Survey | Survey code fadeout
(function($) {
	
	$.fn.callVtip = function() {
		try{
			vtip();
		}catch(e){
  			// To Do
  		}
	};
	/*Viswanathan added for #0074786*/
	$('.fivestar-widget a').live('click', function(e) {
		e.preventDefault();
   });
   // calendar icon - right click disable
   $('#calendar-view-icon').live('contextmenu', function() {
	   return false;
   });
   $('#calendar-view-icon').live('click', function(e) {
	   e.preventDefault();
      	drawCalendar();
   });
})(jQuery);

/* //0072940: Zoho# 18142- Instructor search box is not working
(function($) {
	$('#multiautocomplete').live("focus",  function(e) {
	    $(this).select();
    });	
})(jQuery);
 */
function changeDiscountType(){
	 try{
		 if($('#two-col-row-discount_type_row').find('.addedit-edit-dis_type option:selected').val() == 3){
			 $('#currency-list-discount').addClass('discount_currency_list_visible');
			 $('#currency-list-discount').removeClass('discount_currency_list_invisible');
			 $('.addedit-edit-dis_typeval').addClass('fixed_dusccount_type_applied');
		 } else{
			 $('#currency-list-discount').removeClass('discount_currency_list_visible');
			 $('#currency-list-discount').addClass('discount_currency_list_invisible');
			 $('.addedit-edit-dis_typeval').removeClass('fixed_dusccount_type_applied');
			 $('.discount_currency_list').val('');
		 }
	 }catch(e){
		 // to do
	 }
}
function drawCalendar()
{
	document.cookie="uri="+window.location.search;
	window.location.href="?q=admincalendar";
}
//Added by vetrivel.P for #0070900
function readCookie(name) {
	var nameEQ = name + "=";
	var ca = document.cookie.split(';');
	for(var i=0;i < ca.length;i++) {
		var c = ca[i];
		while (c.charAt(0)==' ') c = c.substring(1,c.length);
		if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
	}
	return null;
}
function urldecode(url) {
	  return decodeURIComponent(url.replace(/\+/g, ' '));
}
function eraseCookie(name) {
	createCookie(name,"",-1);
}
function createCookie(name,value,days) {
	if (days) {
		var date = new Date();
		date.setTime(date.getTime()+(days*24*60*60*1000));
		var expires = "; expires="+date.toGMTString();
	}
	else var expires = "";
	document.cookie = name+"="+value+expires+"; path=/";
}

function entityvalidation(entity_type){
	var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
	obj.createLoader('tab-content-main');
    document.getElementById(entity_type).click();
}

function downloadLink(btn_type){
    if(btn_type == 'user_upload_done_btn'){
	$('#download_template').css('visibility','visible');
	$('#download_template').css('display','block');
    }else if(btn_type == 'enrollment_upload_file'){
            $('#download_template_enroll').css('visibility','visible');
            $('#download_template_enroll').css('display','block');
   }else if(btn_type == 'enrollment_upload_class'){
            $('#download_template_class').css('visibility','visible');
            $('#download_template_class').css('display','block');

        }else{
            //do nothing
        }
}

function downloadSample(btn_type){
	
	var url = resource.base_host;
        if(btn_type == 'user_upload_done_btn'){
	var file = url+"/User_feed.csv";
        }else if(btn_type == 'enrollment_upload_file'){
            var file = url+"/Enrollment_feed.csv";
        }else{
            var file = url; // nothing to do here .. for future use
        }
	window.open(file);
	$('#manage-dd-list').css('visibility','hidden');
	$('#manage-dd-list').css('display','none');
	$('#manage-dd-list').hide();
	if(btn_type == 'user_upload_done_btn'){
	$('#download_template').css('visibility','hidden');
	$('#download_template').css('display','none');
	$('#download_template').hide();
        }else if(btn_type == 'enrollment_upload_file'){
            $('#download_template_enroll').css('visibility','hidden');
            $('#download_template_enroll').css('display','none');
            $('#download_template_enroll').hide();
        }else{
            // do nothing
        }
}
//#custom_attribute_0078975
function MoveTabPrev(itemcount) {
	//console.log('MoveTabPrev: '+itemcount);
	var div_width = $('#carousel_inner').width();
	var ul_width = $('#carousel_inner ul').width();
	var increment = '';
	var item_width = '';
	var left_indent = '';
	var current_position = '';
	var isChromium = window.chrome;
	$('#carousel_inner').css('width','auto');
	$('#carousel_inner ul').css('width','auto');
	var ul_actualwidth = $('#carousel_inner ul').width();
	$('#carousel_inner').css('width',div_width);
	$('#carousel_inner ul').css('width',ul_width);
	var lastli_width = parseInt($('#carousel_container #carousel_inner ul li:last').css('width'));
	// console.log('last li width: '+lastli_width);
	
	if(itemcount === 7) {
	
	if(Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'de'){
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			increment = 22;
		else
			increment = 27;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it') {
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			increment = 93;
		else
			increment = 101;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'pt-pt') {
		if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
			increment = 164;
		else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			increment = 156;
		else
			increment = 152;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ru') {
		if(isChromium)
		{
		$('#carousel_container #carousel_inner').css('width', '410px');
		increment = 42;
		}
		else if(navigator.appVersion.indexOf("Safari")!= -1)
		{
		increment = 44;
		}
		else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			increment = 39;
		else
			increment = 32;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'es') {
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			increment = 99;
		else 
			increment = 104;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'fr') {
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			{
			increment = 23;
			$('#carousel_container #carousel_inner').css('width', '449px');
			}
		else
			increment = 30;
	}
	else
		increment = 30;
	
	if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'en-us') {
		if(navigator.userAgent.indexOf("Firefox")!= -1)
			{
			item_width = (ul_actualwidth - div_width) + 3;
		 console.log('item_width1: '+item_width+'  '+'ul_actualwidth1:'+ul_actualwidth+'  '+'div_width1:'+div_width+'  '+'ul_width1:'+ul_width);
			}
		else
			{
			//alert('hev'+navigator.userAgent.indexOf("chrome")+'navigator.userAgent'+navigator.userAgent);

			item_width = (ul_actualwidth - div_width) + 7;
		// console.log('navigator.userAgent.indexOf'+navigator.userAgent+'  '+'item_width3: '+item_width+'  '+'ul_actualwidth3:'+ul_actualwidth+'  '+'div_width3:'+div_width+'  '+'ul_width3:'+ul_width);
			}

	}
	else
		{
		item_width = (ul_width - div_width) - increment;
	
	
	 //console.log('item_width3: '+item_width+'  '+'ul_actualwidth3:'+ul_actualwidth+'  '+'div_width3:'+div_width+'  '+'ul_width3:'+ul_width+' '+'increment'+increment);
		}
	var left_indent = parseInt($('#carousel_inner ul').css('left')) + item_width;
	var current_position = parseInt($('#carousel_inner ul').css('left'));
	$('#carousel_container .first a').css('margin-right','0');
	if(current_position <= 0) {
		$('#carousel_container .first').css('display', 'none');
		$('#carousel_container').css('padding-left', '9px');
		$('#carousel_container .last').css('display', 'block');
		if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'en-us'){
			if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
				$('#carousel_container #carousel_inner').css('width', '377px');
			else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
				$('#carousel_container #carousel_inner').css('width', '383px');
			else
				$('#carousel_container #carousel_inner').css('width', '373px');
		}
		if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it'){
			if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
				$('#carousel_container #carousel_inner').css('width', '428px');
			else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
				$('#carousel_container #carousel_inner').css('width', '433px');
			else
				$('#carousel_container #carousel_inner').css('width', '428px');
		}
		if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'pt-pt'){
			if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
				$('#carousel_container #carousel_inner').css('width', '400px');
			else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
				$('#carousel_container #carousel_inner').css('width', '405px');
			else
				$('#carousel_container #carousel_inner').css('width', '410px');
		}
	}
	
	}
	
	if(itemcount === 6) {
		// console.log('Hi dude came into movetabprev 6');
		
		//item_width = (navigator.appVersion.indexOf("Trident/7.0")!= -1) ? (parseInt((ul_width - div_width) / 2) + 6) : parseInt((ul_width - div_width) / 2);
				
		var width_calc = (ul_actualwidth - lastli_width) - 3;
		$('#carousel_container #carousel_inner').css('width', width_calc);
		
		left_indent = parseInt($('#carousel_inner ul').css('left')) - parseInt($('#carousel_inner ul').css('left'));
		current_position = parseInt($('#carousel_inner ul').css('left'));
		
		if(current_position <= 0) {
			$('#carousel_container .first').css('display', 'none');
			$('#carousel_container').css('padding-left', '9px');
			$('#carousel_container .last').css('display', 'block');
		}
	}
	
	$('#carousel_inner ul').animate({'left' : left_indent},{queue:true, duration:500},function(){  
    });
}
function MoveTabNext(itemcount) {
	 //console.log('MoveTabNext: '+itemcount);
	var div_width = $('#carousel_inner').width();
	var ul_width = $('#carousel_inner ul').width();
	var increment = '';
	var item_width = '';
	var left_indent = '';
	var current_position = '';
	var isChromium = window.chrome;
	$('#carousel_inner').css('width','auto');
	$('#carousel_inner ul').css('width','auto');
	var ul_actualwidth = $('#carousel_inner ul').width();
	$('#carousel_inner').css('width',div_width);
	$('#carousel_inner ul').css('width',ul_width);
	
	if(itemcount === 7) {
	
	if(Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'de'){
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			//increment = 17;
			increment = 22;
		else
			increment = 27;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it') {
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			//increment = 123;
			increment = 128;
		else
			increment = 129;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'pt-pt') {
		if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
			increment = 228;
		else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			//increment = 223;
			increment = 219;
		else
			increment = 217;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ru'){
		//if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
		if(isChromium)
			{
			$('#carousel_container #carousel_inner').css('width', '410px');
			increment = 42;
			}
		else if(navigator.appVersion.indexOf("Safari")!= -1)
			increment = 44;
		else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			increment = 39;
		else
			increment = 32;
	} 
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'es') {
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			{
			//increment = 99;
			increment = 102;
			$('#carousel_container #carousel_inner').css('width', '449px');
			}
		else 
			increment = 104;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'fr') {
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			{
			//increment = 23;
			increment = 26;
		$('#carousel_container #carousel_inner').css('width', '449px');
			}

		else
			increment = 30;
	}
	else
		increment = 30;
	
	if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'en-us') {
		if(navigator.userAgent.indexOf("Firefox")!= -1)
			{
			item_width = (ul_actualwidth - div_width) - 28;
			 //console.log('item_width1: '+item_width+'  '+'ul_actualwidth1:'+ul_actualwidth+'  '+'div_width1:'+div_width+'  '+'ul_width1:'+ul_width);
			}
		else if(navigator.appVersion.indexOf("Trident/7.0")!= -1)
		{
		item_width = (ul_actualwidth - div_width) - 34;
	 //console.log('navigator.userAgent.indexOf'+navigator.userAgent+'  '+'item_widthnext: '+item_width+'  '+'ul_actualwidthnext:'+ul_actualwidth+'  '+'div_widthnext:'+div_width+'  '+'ul_widthnext:'+ul_width);
		}
		else
			{
			item_width = (ul_actualwidth - div_width) - 24;
			 //console.log('item_width3: '+item_width+'  '+'ul_actualwidth3:'+ul_actualwidth+'  '+'div_width3:'+div_width+'  '+'ul_width3:'+ul_width);
			}
	}
	else
		{
		item_width = (ul_width - div_width) - increment;
	
	
	 //console.log('item_widthnext: '+item_width+'  '+'ul_actualwidthnext:'+ul_actualwidth+'  '+'div_widthnext:'+div_width+'  '+'ul_widthnext:'+ul_width+'  '+'increment'+increment);
		}
	left_indent = parseInt($('#carousel_inner ul').css('left')) - item_width;
	current_position = parseInt($('#carousel_inner ul').css('left'));
	$('#carousel_container .first a').css('margin-right','5px');
	if(current_position >= -162) {
		$('#carousel_container').css('padding-left', '0');
		$('#carousel_container .last').css('display', 'none');
		$('#carousel_container .first').css('display', 'block');
		if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'en-us'){
			if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
				$('#carousel_container #carousel_inner').css('width', '408px');
			else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
				$('#carousel_container #carousel_inner').css('width', '414px');
			else
				$('#carousel_container #carousel_inner').css('width', '404px');
		}
		if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it'){
			if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
				$('#carousel_container #carousel_inner').css('width', '456px');
			else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
				$('#carousel_container #carousel_inner').css('width', '463px');
			else
				$('#carousel_container #carousel_inner').css('width', '456px');
		}
		if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'pt-pt'){
			if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
				$('#carousel_container #carousel_inner').css('width', '464px');
			else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
				$('#carousel_container #carousel_inner').css('width', '472px');
			else {
				$('#carousel_container #carousel_inner').css('width', '475px');
			}
		}
	}
		
	}
	
	if(itemcount === 6) {
		// console.log('Hi dude came into movetabnext 6');
		
		//item_width = (navigator.appVersion.indexOf("Trident/7.0")!= -1) ? (parseInt((ul_width - div_width) / 2) + 6) : parseInt((ul_width - div_width) / 2);
				
		var sort_width = parseInt($('#sort-bar-V2').width());
			// console.log('container width: '+sort_width);
		var actionbar_width = parseInt($('#narrow-search-actionbar').width());
			// console.log('actionbar width: '+actionbar_width);
		var carousel_inner_width = (sort_width - actionbar_width) - 45;
			// console.log('carousel inner width: '+carousel_inner_width);
		var firstli_width = parseInt($('#carousel_container #carousel_inner ul li:first').css('width'));
			// console.log('first li width: '+firstli_width);
		var secondli_width = parseInt($('#carousel_container #carousel_inner ul li:nth-child(2)').css('width'));
			// console.log('second li width: '+secondli_width);
		var thirdli_width = parseInt($('#carousel_container #carousel_inner ul li:nth-child(3)').css('width'));
			// console.log('third li width: '+thirdli_width);
		if(carousel_inner_width > (parseInt(ul_actualwidth) - firstli_width))
			item_width = firstli_width + 6;
		else if(carousel_inner_width > (parseInt(ul_actualwidth) - (firstli_width + secondli_width)))
			item_width = (firstli_width + secondli_width) + 11;
		else
			item_width = (firstli_width + secondli_width + thirdli_width) + 16;
			// console.log('item_width: '+item_width);
		$('#carousel_container #carousel_inner').css('width', carousel_inner_width);	
		
		left_indent = parseInt($('#carousel_inner ul').css('left')) - item_width;
		current_position = parseInt($('#carousel_inner ul').css('left'));
			
		if(current_position >= -162) {
			$('#carousel_container').css('padding-left', '0');
			$('#carousel_container .last').css('display', 'none');
			$('#carousel_container .first').css('display', 'block');
		}
	}

	$('#carousel_inner ul').animate({'left' : left_indent},{queue:true, duration:500},function(){  
    });
}
$('body').click(function(event){
	try{
	if(event.target.className != 'info-classroster-upload vtip'){
       $('#download_template_class_wrapper').hide();
        }
	}catch(e){
		// to do
	}
    });