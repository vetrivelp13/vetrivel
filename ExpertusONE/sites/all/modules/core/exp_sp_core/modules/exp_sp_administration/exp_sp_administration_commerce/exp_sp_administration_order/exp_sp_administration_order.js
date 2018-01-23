var loadObj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;

function searchAddUserOrderDataGrid(mode, type, searchKeyword, orderId, entityType, excludedId){
	try{
	var uniqueId = 'add-user-admin-order';
	$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId);
	searchKeyword = searchKeyword.replace(/\//g, "|");
	var dataGridURL = loadObj.constructUrl("administration/order/user-select/"+encodeURIComponent(searchKeyword)+"/"+excludedId);
	/* clicking on the search icon in move users qtip */
	$('#datagrid-container-'+uniqueId).setGridParam({url: dataGridURL});
    $('#datagrid-container-'+uniqueId).trigger("reloadGrid",[{page:1}]);
	}catch(e){
        //Nothing to do
     }
}

function adminUserFirstnameOrderSelection(userDet,val2){
	try{
	var usrDet = userDet.split(',');
	$("input[name=first_name]").val(usrDet[1]);
	var userId = $('#form-user-id').val();
	$("input[name=formitem_userid]:hidden").val(userId);
	//EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader('order_create_form_wrapper');
	loadObj.createLoader('order_create_form_wrapper');
	$("input[name = 'user_order_creation']").click();
	}catch(e){
        //Nothing to do
     }
}

function adminUserLastnameOrderSelection(userDet,val2){
	try{
	var usrDet = userDet.split(',');
	$("#edit-panes-productorg-last-name").val(usrDet[1]);
	var userId = $('#form-user-id').val();
	$("input[name=formitem_userid]:hidden").val(userId);
	loadObj.createLoader('order_create_form_wrapper');
	$("input[name = 'user_order_creation']").click();
	}catch(e){
        //Nothing to do
     }
}

function adminUserUsernameOrderSelection(userDet,val2){
	try{
	var usrDet = userDet.split(',');
	$("input[name=user_name]").val(usrDet[1]);
	var userId = $('#form-user-id').val();
	$("input[name=formitem_userid]:hidden").val(userId);
	loadObj.createLoader('order_create_form_wrapper');
	$("input[name = 'user_order_creation']").click();
	}catch(e){
        //Nothing to do
     }
}

function adminUserEmailOrderSelection(userDet,val2){
	try{
	var usrDet = userDet.split(',');
	$("input[name=email]").val(usrDet[1]);
	var userId = $('#form-user-id').val();
	$("input[name=formitem_userid]:hidden").val(userId);
	loadObj.createLoader('order_create_form_wrapper');
	$("input[name = 'user_order_creation']").click();
	}catch(e){
        //Nothing to do
     }
}
function loadAddProductDataGrid(mode, type, searchKeyword,orderId, entityType, excludedId){
	try{
	var gridmode = (mode == 'edit') ? true : false;
	var multiselectOption = (mode == 'edit') ? false : true;
	var objStr = '';
	var obj = $("body").data("mulitselectdatagrid");
	obj.mode = mode;
	obj.type = type;
	obj.entityId = orderId;
	obj.entityType = entityType;
	obj.uniqueId = 'add-product-admin-order';
	uniqueId 	 = 'add-product-admin-order';
    
	var title 	  = $('#addproduct-autocomplete').val();
	if(title == Drupal.t("LBL545"))
		title='';
	var dl_type 	  = $('#addproduct-autocomplete_hidden').val();
	dl_type=dl_type.replace("u0027","'");
	if(dl_type == Drupal.t("LBL428"))
		dl_type='';
	obj.createLoader('admin_product_addorder_container');
  	var searchStr	= '&title='+encodeURIComponent(title)+'&dl_type='+encodeURIComponent(dl_type);
  	//change by ayyappans for 45261: Common Issue- In create order "Add trainings" pop up shows out of the frame.
  	//have to sync these width values with Drupal.ajax.prototype.commands.renderOrderClassPopup
	var titleWidth = 160, codeWidth = 120, DeliveryTypeWidth = 100, LanguageWidth = 90, PriceWidth = 80, StatusWidth = 90, AddProductOrderWidth = 120;
	if(Drupal.settings.user.language !== undefined && $.inArray(Drupal.settings.user.language, ["pt-pt", "de", "es", "fr", "it", "ru"]) != -1) {
//		AddProductOrderWidth = 220;
		DeliveryTypeWidth  =  140;
	}
	var orderGridWidth = titleWidth + codeWidth + DeliveryTypeWidth + LanguageWidth + PriceWidth + StatusWidth + AddProductOrderWidth;
	$("#datagrid-container-"+uniqueId).jqGrid({
		url: loadObj.constructUrl("administration/order/product-select/"+encodeURIComponent(searchKeyword)+"/"+excludedId+'/'+searchStr),
		datatype: "json",
		mtype: 'GET',		
		colNames:[ Drupal.t('LBL083'),Drupal.t('LBL096'),Drupal.t('LBL036'),Drupal.t('LBL038'),Drupal.t('LBL040'),Drupal.t('LBL102'), ''],
		colModel:[ { name:'Title',index:'cls_title', title: false, width: titleWidth, 'widgetObj':objStr, sortable: true, formatter: obj.displayGridValues },    
		           { name:'Code',index:'cls_code', title: false, width: codeWidth, 'widgetObj':objStr, sortable: true, formatter: obj.displayGridValues },
		           { name:'DeliveryType',index:'delivery_type_name', title: false, width:DeliveryTypeWidth, 'widgetObj':objStr, sortable: true, formatter: obj.displayGridValues },
		           { name:'Language',index:'language', title: false, width: LanguageWidth, 'widgetObj':objStr, sortable: true, formatter: obj.displayGridValues },
		           { name:'Price',index:'price', title: false, width: PriceWidth, 'widgetObj':objStr, sortable: true, formatter: obj.displayGridValues },
		           { name:'Status',index:'Status', title: false, width: StatusWidth, 'widgetObj':objStr, sortable: false, formatter: obj.displayGridValues },
		           { name:'Add Product',index:'AddProductOrder', title: false, width:AddProductOrderWidth, 'widgetObj':objStr, sortable: false, formatter: obj.displayGridValues }
		         ],
		rowNum: 10,
		rowList:[10,25,50],
		pager: '#pager-datagrid-'+uniqueId,
		viewrecords:  true,
		multiselect: false,
		emptyrecords: "",
		sortorder: "asc",
		toppager: true,
		botpager: false,
		height: 'auto',
		width: orderGridWidth,
		loadtext: "",
		recordtext: "",
		pgtext : "{0} of {1}",
		//pgtext : "",
		loadui:false,
		onSortCol:function (){
			obj.createLoader('admin_product_addorder_container');
		},
		loadComplete:obj.callbackDataGrid
	}).navGrid('#pager-datagrid-'+uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
	$("#jqgh_datagrid-container-add-product-admin-order_Status").removeClass('ui-jqgrid-sortable');
	$('#admin_product_addorder_container #datagrid-div-add-product-admin-order').find('#datagrid-container-add-product-admin-order_toppager').css({"position":"absolute","right":"0","top":"-34px","width":"200px"});
	chkorderpg();
	}catch(e){
        //Nothing to do
     }
}
function chkorderpg(){
	setTimeout(function() {
	var chkorderPager=$('#admin_product_addorder_container #datagrid-div-add-product-admin-order').find('#datagrid-container-add-product-admin-order_toppager').css("display");
	var recordCount = $('#datagrid-container-'+uniqueId).jqGrid('getGridParam', 'records');
	if(chkorderPager=="block" || recordCount > 10){
	    $("#admin-data-grid #admin_product_addorder_container").find(".admin_add_multi_search_container").css("padding-bottom","35px");
	}else{
		$("#admin-data-grid #admin_product_addorder_container").find(".admin_add_multi_search_container").css("padding-bottom","35px");
	}
	}, 1800);
}
function searchAddOrderProductDataGrid(mode, type, searchKeyword, orderId, entityType, userId){
	try{
	var uniqueId = 'add-product-admin-order';
	$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId);
	searchKeyword = searchKeyword.replace(/\//g, "|");
	var title 	  = searchKeyword; //$('#addproduct-autocomplete').val();
	if(title == Drupal.t("LBL545"))
		title='';
	var dl_type    = $('#addproduct-autocomplete_hidden').val();
	if(dl_type == Drupal.t("LBL428"))
		dl_type='';
	var searchStr	= '&title='+encodeURIComponent(title)+'&dl_type='+encodeURIComponent(dl_type);
	var dataGridURL = loadObj.constructUrl("administration/order/product-select/"+0+"/"+userId+'/'+searchStr);
	/* clicking on the search icon in move users qtip */
	$('#datagrid-container-'+uniqueId).setGridParam({url: dataGridURL});
    $('#datagrid-container-'+uniqueId).trigger("reloadGrid",[{page:1}]);
    chkorderpg();
	}catch(e){
        //Nothing to do
     }
}

//#custom_attribute_0078975 - To validate the custom attributes when click on Add trainings
function customAttributeOrderJsValidation(){
    try{ 
		
		var custom_attr_hidden_val = $('#hid_custom_attr_order_mandatroy').data('hval'); //getter 
		var invalid=0;
		
		if(custom_attr_hidden_val!='' && custom_attr_hidden_val!=undefined){ //if there is no custom attribute
		 
				 var sp_cust_attr_arr = custom_attr_hidden_val.split('~~');
				 
				 if(sp_cust_attr_arr.length>0){ //if there is mandatory custom fields
				 
					   $.each(sp_cust_attr_arr,function(key, value){ 
					   
					     //splitup each value
					     var cust_attr_arr=value.split('|');
					     if(cust_attr_arr.length>0){ //if there is custom fields 
					        
					            var attr_type=cust_attr_arr[0]; //Type like textbox, checkbox etc
					            var attr_code=cust_attr_arr[1]; //code-of the custom attribute
					            var attr_helptext=cust_attr_arr[2]; //Helptext of the custom attribute  
					                      
				              	  if(attr_type=='cattr_type_txtbox' || attr_type=='cattr_type_txtarea'){ //textbox
				              		
				              		  var tmp_ord_fld_code='#edit-panes-billing'+'-'+attr_code;  //edit-panes-billing-c-order-textbox 
				              		  var tmp_val=$(tmp_ord_fld_code).val();
				              		   tmp_val=tmp_val.trim();
				              		  if(tmp_val=='' || tmp_val==attr_helptext){ 
				              		    invalid++;
				              		  }
				              	  }
				              
				              	  if(attr_type=='cattr_type_dropdown' || attr_type=='cattr_type_checkbox' || attr_type=='cattr_type_radio'){ //Dropdown single or multiple 
				              		  if(attr_type=='cattr_type_dropdown'){
				              		   var sel_tmp='select[id*='+attr_code+'] option:selected';
				              		  }else{ 
				              		     var sel_tmp='input[name*=panes[billing]['+attr_code+']]:checked'; 
				              		  }
				              		  var tmp_val=$(sel_tmp).val();
				              		  if(tmp_val=='' || tmp_val==undefined || tmp_val=='0' || tmp_val==0){
				              		     invalid++;
				              		  }
				              	   }
					      }
					     
					   });
				 }  
		}
		
		return invalid;
	    
	}catch(e){
	    //Nothing to do
	 } 
}

function adminProductOrderSelection(nodeId){
	try{
	
	var ret_cust_attr_invalid=customAttributeOrderJsValidation();  //#custom_attribute_0078975 - To validate the custom attributes when click on Add trainings
	 
	if($('#edit-panes-billing-billing-first-name').val()=='' || $('#edit-panes-billing-billing-last-name').val()=='' || $('#edit-panes-billing-billing-street1').val()==''
		|| $('#edit-panes-billing-billing-city').val()=='' || $('select[id*=billing-country] option:selected').val() == 0 || $('select[id*=billing-country] option:selected').val()== undefined 
		|| $('select[id*=billing-zone] option:selected').val() == 0 
		|| $('#edit-panes-billing-billing-postal-code').val() =='' || ret_cust_attr_invalid>0){   //#custom_attribute_0078975 - To validate the custom attributes when click on Add trainings
			var error = new Array();
			error[0] = Drupal.t("ERR244");
		 var message_call = expertus_error_message(error,'error');
		 $(".page-administration-order-create #paintAdminOrderResults #show_expertus_message").html(message_call);
         $(".page-administration-order-create #paintAdminOrderResults #show_expertus_message").show();
		return;
	}
	$("input[name=form_selected_nid]:hidden").val(nodeId);
	if(nodeId > 0){
	 $("input[name = 'order_product_creation']").click();
	}
	}catch(e){
        //Nothing to do
     }
}
function adminMultiTpOrderSelection(classIds){
	try{
	$("input[name=form_selected_classid]:hidden").val(classIds);
	if(classIds.length > 0){
	  $("input[name = 'order_product_creation']").click();
	}
	}catch(e){
        //Nothing to do
     }
}
function removeAdminCartItem(nodeId){
	try{
	$("input[name=form_removed_nid]:hidden").val(nodeId);
	loadObj.createLoader('order_create_form_wrapper');
	$("input[name = 'order_remove_cart']").click();
	}catch(e){
        //Nothing to do
     }
}

function deliveryProductHideShow() {
	try{
	$('#select-list-dropdown-list').slideToggle();
	$('#select-list-dropdown-list li:last').css('border-bottom','0px none');
	}catch(e){
        //Nothing to do
     }
}
function deliveryTypeSearch (dCode,dText,userId) {
	try{
	var uniqueId = 'add-product-admin-order';
	$('#select-list-dropdown-list').hide();
	$('#select-list-dropdown').text(dText);
	$('#addproduct-autocomplete_hidden').val(dCode);
	//$("#lnr-myteam-search").data("lnrmyteamsearch").catalogSearchAction("","",userId);	
	//$("#lnr-myteam-search").data("lnrmyteamsearch").paintCatkeywordAutocomplete(userId);
	 //Highlight sort type Shobana
    if(dCode!= null) {
		$('#select-list-dropdown-list li').each(function() {
			$('#select-list-dropdown-list li').removeClass('sortype-high-lighter');
		});
		$('.'+dCode).addClass('sortype-high-lighter');
		if(dCode == Drupal.t('LBL428')){
			$('.searchany').addClass('sortype-high-lighter');
		}
    }
    else {
		$('#select-list-dropdown-list li').each(function() {
			$('#select-list-dropdown-list li').removeClass('sortype-high-lighter');
		});
    }
	
	var title 	  = $('#addproduct-autocomplete').val();
	if(title == Drupal.t("LBL545"))
		title='';
	var dl_type 	  = $('#addproduct-autocomplete_hidden').val();
	if(dl_type == Drupal.t("LBL428"))
		dl_type='';
	var searchStr	= '&title='+encodeURIComponent(title)+'&dl_type='+encodeURIComponent(dl_type);
	$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId);
	var dataGridURL = loadObj.constructUrl("administration/order/product-select/0/"+userId+'/'+searchStr);
	$('#datagrid-container-'+uniqueId).setGridParam({url: dataGridURL});
    $('#datagrid-container-'+uniqueId).trigger("reloadGrid",[{page:1}]);
    chkorderpg();
	}catch(e){
        //Nothing to do
     }
}

/* Highlight root admin menu */
function setAdminLinkSelected(){
	try{
	if($('#block-system-main-menu a[href="/?q=administration"]').hasClass('active') == false){
	    $('#block-system-main-menu a[href="/?q=administration"]').addClass('active-trail active');
		$('#block-system-main-menu a[href="/?q=administration"]').parent().addClass('active-trail');
	}
	}catch(e){
        //Nothing to do
     }
}

/*(function($) {
	$.widget("ui.adminorderpage", {
	
	 * _init() - The init function of this widget
	
	 * To show the pop-up with the rendered classes
});

$.extend($.ui.adminorderpage.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);*/

$(function() {
	//$('#admin_order').adminorderpage();
	 Drupal.ajax.prototype.commands.renderOrderClassPopup = function (ajax,response,orderId,drupalUId) {
		 try{
		    $("#paintAdminOrderResults").remove();
			var obj	= this;
			this.currTheme = Drupal.settings.ajaxPageState.theme;
		 	/*if(this.currTheme == "expertusoneV2"){
				var chgclassqtipWidth = 750 ;	
			}else{
				var chgclassqtipWidth = 712;
		    }*/
			var titleWidth = 160, codeWidth = 120, DeliveryTypeWidth = 100, LanguageWidth = 90, PriceWidth = 80, StatusWidth = 90, AddProductOrderWidth = 120;
			if(Drupal.settings.user.language !== undefined && $.inArray(Drupal.settings.user.language, ["pt-pt", "de", "es", "fr", "it", "ru"]) != -1) {
//				AddProductOrderWidth = 220;
				DeliveryTypeWidth  =  140;
			}
			var chgclassqtipWidth = titleWidth + codeWidth + DeliveryTypeWidth + LanguageWidth + PriceWidth + StatusWidth + AddProductOrderWidth + 40;
			var html = "<div id='paintAdminOrderResults'></div>";
		    
		    $('body').append(html);
		    var nHtml = "<div id='show_expertus_message'></div><div id='paintAdminOrderResults'></div>";
		    $('#paintAdminOrderResults').html(nHtml);
		    //alert('1');
		    var closeButt={};
		    closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close'); $('#select-order-dialog').remove();};
		    $("#paintAdminOrderResults").dialog({
		    	autoResize: true,
		    	position:[(getWindowWidth()-chgclassqtipWidth)/2,100],
		        bgiframe: true,
		        width:chgclassqtipWidth,
		        resizable:false,
		        draggable:false,
		        closeOnEscape: false,
		        modal: true,
		        title:SMARTPORTAL.t('<span class="myteam-header-text">' + Drupal.t("Trainings") +'</span><div id="search-cat-keyword"></div>'),
		        buttons: closeButt,  

		        /*close: function(){
		            $("#paintAdminOrderResults").remove();
		            $("#select-order-dialog").remove();
		            //obj.viewLearning(id,rowObj);
		        },*/
		        overlay: {
			           opacity: 0.5,
		           background: "#000000"
		         },
		        open: function(){
		        	 $(".ui-widget-overlay").eq(-1).css("width",getWindowWidth()-1+"px");
		         }
		    });
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	        $('.ui-dialog').wrap("<div id='select-order-dialog'></div>");
		    $("#paintAdminOrderResults").css("position","");
		    this.currTheme = Drupal.settings.ajaxPageState.theme;
		 	if(this.currTheme == "expertusoneV2"){
		 		changeDialogPopUI();
		 		$('#select-order-dialog').next('.ui-widget-overlay').css('display','block');
		   }
		 	$('.ui-dialog-titlebar-close, .removebutton').click(function(){
				$('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
				refreshcartpane();
		        $("#paintAdminOrderResults").remove();
		    });
		 	var lastChild =$('#paintAdminOrderResults').children().last();
	    	lastChild.replaceWith(response.html);
		 	//$("#paintAdminOrderResults").html(response.html);
		 	Drupal.attachBehaviors(); 
		 	
		 	//$('#admin_order').data('adminorderpage').dropEnrollCallfff(orderId,drupalUId);
		 }catch(e){
		        //Nothing to do
		  }
		};

});
$(function() {
	try{
	 
	//#custom_attribute_0078975
		//To set char count for TextArea for custom attribute
    	var len_cus_attr=$('#orderpage_custom_attributes #custom_attr_container').length; 
    	//alert('len_cus_attr='+len_cus_attr);
    	if(len_cus_attr>0){ 
    	
    	  //Not loading Multiple Dropdown in Chrome and IE browser
    	  var no_of_mul_dd=$('#orderpage_custom_attributes #custom_attr_container .mul-opt-dd-img').length; 
    	 // alert('no_of_mul_dd='+no_of_mul_dd);
    	  if(no_of_mul_dd>0){ 
    	       $('#orderpage_custom_attributes #custom_attr_container .mul-opt-dd-img').each(function() {   
    	         $(this).trigger( "load" );
    	       }); 
    	  }
    	  
	  		//$( "#multiselect_img").trigger( "load" );
			
    	    $('#orderpage_custom_attributes #custom_attr_container span[id^="char_count_c_"]').each(function() {   
    	             // alert('I am Order Page Calling');
		    	      var span_attr_id= $(this).attr('id'); 
		    	      var und_span_attr_id= $(this).attr('id').replace('char_count_c_','edit-panes-billing-c-').replace('_','-'); 
		    	      var helptext= $('#'+und_span_attr_id).data("default-text");
		    	      var entered_text=$('#'+und_span_attr_id).html();
		    	      
		    	     if(helptext!=entered_text && helptext!=''){
		    	         var src_val_len=$('#'+und_span_attr_id).html().length;  
		    	          $('#'+span_attr_id).html(src_val_len);
		    	     } 
		     });
    	    
    	}  
    	
	
	$('#edit-panes-productorg-first-name').autocomplete(
			"/?q=administration/order/userinfo-autocomplete/first_name",{
			minChars :3,
			max :50, 
			autoFill :true,
			mustMatch :false,
			matchContains :false,
			inputClass :"ac_input",
			loadingClass :"ac_loading",
			formatItem:function(pmRec){
				var fnameDet = '';
				fnameDet += '<div class="order-user-auto"><h4 class="fname-bold">'+pmRec[1]+'</h4>';
				fnameDet += '<div class="fname-detail"><span class="username-class">'+pmRec[2];
				fnameDet += (pmRec[3]) ? '</span> | <span class="org-name">'+pmRec[3]+'</span>' : '</span></div></div>';
				return fnameDet;
				},
			extraParams :{
				id:'form-user-id',
				callback : 'adminUserFirstnameOrderSelection'
			}
	});
	$('#edit-panes-productorg-last-name').autocomplete(
			"/?q=administration/order/userinfo-autocomplete/last_name",{
			minChars :3,
			max :50, 
			autoFill :true,
			mustMatch :false,
			matchContains :false,
			inputClass :"ac_input",
			loadingClass :"ac_loading",
			formatItem:function(pmRec){
				var fnameDet = '';
				fnameDet += '<div class="order-user-auto"><h4 class="fname-bold">'+pmRec[1]+'</h4>';
				fnameDet += '<div class="fname-detail"><span class="username-class">'+pmRec[2];
				fnameDet += (pmRec[3]) ? '</span> | <span class="org-name">'+pmRec[3]+'</span>' : '</span></div></div>';
				return fnameDet;
				},
			extraParams :{
				id:'form-user-id',
				callback : 'adminUserLastnameOrderSelection'
			}
	});
	$('#edit-panes-productorg-username').autocomplete(
			"/?q=administration/order/userinfo-autocomplete/user_name",{
			minChars :3,
			max :50, 
			autoFill :true,
			mustMatch :false,
			matchContains :false,
			inputClass :"ac_input",
			loadingClass :"ac_loading",
			formatItem:function(pmRec){
				var fnameDet = '';
				fnameDet += '<div class="order-user-auto"><h4 class="fname-bold">'+pmRec[1]+'</h4>';
				fnameDet += '<div class="fname-detail"><span class="username-class">'+pmRec[2];
				fnameDet += (pmRec[3]) ? '</span> | <span class="org-name">'+pmRec[3]+'</span>' : '</span></div></div>';
				return fnameDet;
				},
			extraParams :{
				id:'form-user-id',
				callback : 'adminUserUsernameOrderSelection'
			}
	});
	$('#edit-panes-productorg-email').autocomplete(
			"/?q=administration/order/userinfo-autocomplete/email",{
			minChars :3,
			max :50, 
			autoFill :true,
			mustMatch :false,
			matchContains :false,
			inputClass :"ac_input",
			loadingClass :"ac_loading",
			formatItem:function(pmRec){
				var fnameDet = '';
				fnameDet += '<div class="order-user-auto"><h4 class="fname-bold">'+pmRec[1]+'</h4>';
				fnameDet += '<div class="fname-detail"><span class="username-class">'+pmRec[2];
				fnameDet += (pmRec[3]) ? '</span> | <span class="org-name">'+pmRec[3]+'</span>' : '</span></div></div>';
				return fnameDet;
				},
			extraParams :{
				id:'form-user-id',
				callback : 'adminUserEmailOrderSelection'
			}
	});
	}catch(e){
        //Nothing to do
	}
});

//#custom_attribute_0078975
Drupal.behaviors.removeInputGreyfromCustomAttribute =  {
    attach: function (context, settings) {
    try{  
    
       //To set char count  in TextArea
    	/*var len_cus_attr=$('#custom_attr_container').length; 
    	//alert('len_cus_attr='+len_cus_attr);
    	if(len_cus_attr>0){ 
    	    $('#custom_attr_container span[id^="char_count_c_"]').each(function() {   
		    	      var span_attr_id= $(this).attr('id'); 
		    	      var und_span_attr_id= $(this).attr('id').replace('char_count_c_','edit-panes-billing-c-').replace('_','-'); 
		    	      var helptext= $('#'+und_span_attr_id).data("default-text");
		    	      var entered_text=$('#'+und_span_attr_id).html();
		    	      
		    	     if(helptext!=entered_text && helptext!=''){
		    	         var src_val_len=$('#'+und_span_attr_id).html().length;  
		    	          $('#'+span_attr_id).html(src_val_len);
		    	     } 
		     });
    	    
    	} */
    	
    	 
    	//Remove the grey class from custom attribute container if not matched the help text - Added by ganesh for Custom Attribute
    	if($('#custom_attr_container input').hasClass('input-field-grey') || $('#custom_attr_container textarea').hasClass('input-field-grey')){
    		//alert('hi1 -  - AddEdit Behaviur'); 
	    	$('#custom_attr_container .input-field-grey').each(function() {  
			     
			     var entered_value=$(this).val().toLowerCase();
			     var help_text_value=$(this).data('default-text').toLowerCase();
			     
			    // alert('entered_value='+entered_value);
			    // alert('help_text_value='+help_text_value);
			     if(entered_value!=help_text_value){
			     	//alert('Remove Class');
			     	$(this).removeClass('input-field-grey');
			     } 
			  });
    	} 
    	
    	
    }catch(e){
		// To Do
	}
    }
  };
  

Drupal.behaviors.orderAdminApplyDiscountTextArea =  {
	      attach: function (context, settings) {
	    	  $('#uc-discounts-codes').each(function() {
	    		  try{
	    		  // Stores the default value for each textarea within each textarea
	    		  textareaId = $(this).attr('id');
	    		  $.data(this, 'default', this.value);
	    		  if(this.value == Drupal.t("MSG472")){
		    		  $('#'+textareaId).css('color','#999999');
		    		  $('#'+textareaId).css('font-size','10px');
		    		  $('#'+textareaId).css('line-height','21px');
	    		  }else{
	    			  $('#'+textareaId).css('color','#333333');
		    		  $('#'+textareaId).css('font-size','12px');
	    		  }
	    		  }catch(e){
	    		        //Nothing to do
	    			}
	    	  }).focus(function() {
	    		  try{
	    		  // If the user has NOT edited the text clear it when they gain focus
	    		  if(this.value != Drupal.t("MSG472")){
	    			  $.data(this, 'edited', this.value);
	    		  }
	    		  if (!$.data(this, 'edited')) {
	    			  this.value = "";
	    			  $('#'+textareaId).css('color','#333333');
		    		  $('#'+textareaId).css('font-size','12px');
	    		  }
	    		  }catch(e){
	    		        //Nothing to do
	    			}
	    		  
	    	  }).change(function() {
	    		  try{
	    		  // Fires on blur if the content has been changed by the user
	    		  $.data(this, 'edited', this.value != "");
	    		  }catch(e){
	    		        //Nothing to do
	    			}
	    	  }).blur(function() {
	    		  try{
	    		  // Put the default text back in the textarea if its not been edited
	    		  if(this.value == "" && !$.data(this, 'edited')){
	    			  this.value = Drupal.t("MSG472");
	    			  $.data(this, 'default', this.value);
	    			  $('#'+textareaId).css('color','#999999');
		    		  $('#'+textareaId).css('font-size','10px');
	    		  }
	    		  }catch(e){
	    		        //Nothing to do
	    			}
	    	  });
	      } // end attach cke_resizer
};

function refreshcartpane(){
	try{
		$.ajax({
			type: "GET",
			url: "?q=ajax/order/cartpane/update",
			success: function(data){
			 $('#clone_cart-pane').html(data);
			 textareaVal = $.trim($("textarea[id*=uc-discounts-codes]").val());
			 if(textareaVal!=Drupal.t("MSG472") && textareaVal != null && textareaVal != "" && textareaVal!=undefined)
				 setTimeout(function() { $('#uc-discounts-button').click(); }, 200);
			 Drupal.attachBehaviors(); 
			}
			});
	}catch(ex){
		alert(ex);
	}
}