(function($) {
	$.widget("ui.myaccountorders", {
		_init: function() {
			try{
			var self = this;
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			if(document.getElementById('myaccount-orders-details')){	
			this.renderOrderSearchResults();
			}
			}catch(e){
				// to do
			}
		},
		callbackOrdersLoader : function(response, postdata, formid)
		{
			try{
			$("#wizard-form-wrapper").data("myaccountorders").destroyLoader('wizard-form-wrapper');
			$('#gbox_myaccount-orders-details').show();
			var recs = parseInt($("#myaccount-orders-details").getGridParam("records"),10);
	        if (recs == 0) {
	        	  $("#account-pager_center").css('display','none');
	        	  if(document.getElementById('no-records-id') == null){
	        		  var norecordsmsg = '<div id="no-records-id" class="norecords-msg-profile-page">'+Drupal.t('MSG669')+'</div>';
	  				  $("#gview_myaccount-orders-details .ui-jqgrid-hdiv").after(norecordsmsg);
	        	  }
	            
	        } else {
	        	$('.norecords-msg-edit-page').css('display','none');
	        	$(".no-orderrecords").html("");
	        }
	        $('#next_account-pager').click( function(e) {
						if(!$('#next_account-pager').hasClass('ui-state-disabled')) {
							$('#gbox_myaccount-orders-details').show();
							$('#gview_myaccount-orders-details').css('min-height','100px');
							$("#wizard-form-wrapper").data("myaccountorders").createLoader('wizard-form-wrapper');
						}
					});
	      	$('#prev_account-pager').click( function(e) {
	  				if(!$('#prev_account-pager').hasClass('ui-state-disabled')) {
	  					$('#gbox_myaccount-orders-details').show();
	  					$('#gview_myaccount-orders-details').css('min-height','100px');
	  					$("#wizard-form-wrapper").data("myaccountorders").createLoader('wizard-form-wrapper');
	  				}
	  			});
	  			$('.ui-pg-selbox').bind('change',function() {
	  				$('#gbox_myaccount-orders-details').show();
	  				$('#gview_myaccount-orders-details').css('min-height','100px');
	  				$("#wizard-form-wrapper").data("myaccountorders").hideOrderPageControls(false);
	  				$("#wizard-form-wrapper").data("myaccountorders").createLoader('wizard-form-wrapper');
	  			});			
	  			$(".ui-pg-input").keyup(function(event){				
	  				if(event.keyCode == 13){
	  					$('#gbox_myaccount-orders-details').show();
	  					$('#gview_myaccount-orders-details').css('min-height','100px');
	  				  $("#wizard-form-wrapper").data("myaccountorders").createLoader('wizard-form-wrapper');
	  				}
	  			});
	  			
	  			$('#gbox_myaccount-orders-details tr:odd').css("background", "#f7f7f7");
				$('#gbox_myaccount-orders-details tr.ui-jqgrid-labels').css("background", "none");
				$('#gbox_myaccount-orders-details #pg_account-pager tr').css("background", "#fff");
					//$('#datagrid-container-'+uniqueId).css('width','610px');
					//$('.ui-jqgrid-htable').css('width','610px');
	  		
	  			Drupal.attachBehaviors();

	        var obj = $("#wizard-form-wrapper").data("myaccountorders");
	        // Show pagination only when search results span multiple pages
	        var reccount = parseInt($("#myaccount-orders-details").getGridParam("reccount"), 10);
	        var hideAllPageControls = true;
	        if (recs > 10) { // 10 is the least view per page option.
	          hideAllPageControls = false;
	        }	        
	        //console.log('callbackLoader() : reccount = ' + reccount);
	        if (recs <= reccount) {
	          //console.log('callbackLoader() : recs <= reccount : hide pagination controls');
	          obj.hideOrderPageControls(hideAllPageControls);
	        }
	        else {
	          //console.log('callbackLoader() : recs <= reccount : show pagination controls');
	          obj.showOrderPageControls();
	        } 
	        this.currTheme = Drupal.settings.ajaxPageState.theme;
	        if(this.currTheme == "expertusoneV2")
	        {
		   $(".ui-jqgrid-bdiv").css("overflow","visible");
		   $(".ui-jqgrid-bdiv div").css('padding-left','0px');
		   
		   $('#myaccount-orders-details').find('tr td:first-child').css("font-family", "ProximaNovaBold").css("color","#474747");
		   //$('#myaccount-orders-details').find('tr td').css('padding-left','23px');
		   //$('#myaccount-orders-details').find('tr:first td:first').css('width','83px');
		   //$('#myaccount-orders-details').find('tr:first td:eq(1)').css('width','86px');
		   //$('#myaccount-orders-details').find('tr:first td:eq(2)').css('width','146px');
		   //$('#myaccount-orders-details').find('tr:first td:eq(5)').css('width','95px');
		  // $('#myaccount-orders-details').find('tr:first td:eq(3)').css('width','144px');
		 //  $('#myaccount-orders-details').find('tr:first td:eq(3)').css('width','146px');
		   $('#myaccount-orders-details').find('tr td:last-child').css("font-family", "ProximaNovaBold").css("color","#474747").css("text-transform", "uppercase");
	        }
	        else
	        {
	        	$(".ui-jqgrid-bdiv").css("overflow","hidden");
	        }
			}catch(e){
				// to do
			}
		},
		
		displayOrderGridValues : function(cellvalue, options, rowObject){
			try{
			var index  = options.colModel.index;
			return (rowObject[index] == 'undefined' || rowObject[index] == null) ? '' : rowObject[index];
			}catch(e){
				// to do
			}
		},
		
		hideOrderPageControls : function(hideAll) {
		try{
	    var lastDataRow = $('#myaccount-orders-details tr.ui-widget-content').filter(":last");
	    
	    if (hideAll) {
	      $('#account-pager').hide();
	      
	      //remove bottom dotted border from the last row in result if records are present
	      if (lastDataRow.length != 0) {
	        //console.log('hideOrderPageControls() : hideAll - last data row found');
	        lastDataRow.children('td').css('border', '0 none');
	      }
	    }
	    else {
	      //console.log('hideOrderPageControls() : hide only next/prev page control');
	      $('#account-pager').show();
	      $('#account-pager #account-pager_center #next_account-pager').hide();
	      $('#account-pager #accoount-pager_center #prev_account-pager').hide();              
	      $('#account-pager #account-pager_center #sp_1_account-pager').parent().hide();
	    }
		}catch(e){
			// to do
		}
	  },
	  
	  showOrderPageControls : function() {
		  try{
	    //console.log('showOrderPageControls() : show all control');
	    $('#account-pager').show();
	    $('#account-pager #account-pager_center #next_account-pager').show();
	    $('#account-pager #account-pager_center #prev_account-pager').show();              
	    $('#account-pager #account-pager_center #sp_1_account-pager').parent().show();
		  }catch(e){
				// to do
			}
	  },
		renderOrderSearchResults : function(){	    
			try{
			this.createLoader('wizard-form-wrapper');
			var obj = this;
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			var taxWidth;
      if(this.currTheme == "expertusoneV2")
      {
      	 taxWidth =93;
      }else{
      	 taxWidth =81;
      }
      var col1Width = 101, col2Width = 99, col3Width = 103, col4Width = 99, col5Width = 101,col6Width = 98,col7Width = 99,col8Width = 98;
			if(Drupal.settings.user.language !== undefined && $.inArray(Drupal.settings.user.language, ["pt-pt", "de", "fr", "it", "ru"]) != -1) {
				var col1Width = 110,col2Width = 100,col3Width = 140,col8Width = 140;
			}
			//searchStr = '';
			//var urlStr = (searchStr != '') ? ('&title='+encodeURIComponent(searchStr)) : '';
			//var urlStr = (searchStr != '') ? searchStr : '';
			var objStr = '$("body").data("myaccountorders")';
			
			$("#myaccount-orders-details").jqGrid({
				url:obj.constructUrl("learning/my-account/shopping-cart/results"),
				datatype: "json",
				mtype: 'GET',
			colNames:[ Drupal.t('LBL884'), Drupal.t('LBL821'),Drupal.t('LBL822'),Drupal.t('LBL040'),Drupal.t('Discount'),Drupal.t('Tax'),Drupal.t('LBL827'),Drupal.t('LBL824') ],
			colModel:[ {name:'Orderid',classes:'myaccount-orders-details_Orderid',index:'uc_order_id', title: false, width:col1Width, 'widgetObj':objStr, formatter: obj.displayOrderGridValues, sortable: true, resizable:false},
			   			 {name:'Items',classes:'myaccount-orders-details_Count',index:'product_count', title: false, width:col2Width, 'widgetObj':objStr, formatter: obj.displayOrderGridValues, sortable: true, resizable:false  },
			   			 {name:'OrderDate',classes:'myaccount-orders-details_Date',index:'purchase_date', title: false, width:col3Width, 'widgetObj':objStr, formatter: obj.displayOrderGridValues, sortable: true, resizable:false },
			   			 {name:'Price',classes:'myaccount-orders-details_Price',index:'Price', title: false, width:col4Width, 'widgetObj':objStr, formatter: obj.displayOrderGridValues, sortable: false, resizable:false },
			   			 {name:'Discount',classes:'myaccount-orders-details_Discount',index:'Discount', title: false, width:col5Width, 'widgetObj':objStr, formatter: obj.displayOrderGridValues, sortable: false, resizable:false },
			   			 {name:'Tax',classes:'myaccount-orders-details_Tax',index:'Tax', title: false, width:col6Width, 'widgetObj':objStr, formatter: obj.displayOrderGridValues, sortable: false, resizable:false },
			   			 {name:'Total',classes:'myaccount-orders-details_Total',index:'order_total', title: false, width:col7Width, 'widgetObj':objStr, formatter: obj.displayOrderGridValues, sortable: true, resizable:false},
			   			 {name:'Invoice',classes:'myaccount-orders-details_Invoice',index:'Invoice', title: false,width:col8Width,'widgetObj':objStr, formatter: obj.displayOrderGridValues, sortable: false, resizable:false}],
			 rowNum:10,           
				rowList:[10,20,30],
				pager: '#account-pager',
				viewrecords:  true,
				emptyrecords: "",
				sortorder: "desc",
				toppager:false,
				height: 'auto',
				width: 902,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadui:false,
				hoverrows:false,
				onSortCol:function (){
					obj.createLoader('wizard-form-wrapper');
				},
				loadComplete:obj.callbackOrdersLoader
			}).navGrid('#account-pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});
			$("#jqgh_myaccount-orders-details_Discount").removeClass('ui-jqgrid-sortable');
			$("#jqgh_myaccount-orders-details_Tax").removeClass('ui-jqgrid-sortable');
			$("#jqgh_myaccount-orders-details_Price").removeClass('ui-jqgrid-sortable');
			}catch(e){
				// to do
			}
		}
	  
	});
	
	$.extend($.ui.myaccountorders.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

	})(jQuery);


$(function() {
	try{
		$("#wizard-form-wrapper").myaccountorders();
	}catch(e){
		// to do
	}
});

function remove_messages(){
	try{
	$('.messages').css('display','none');
	}catch(e){
		// to do
	}
}
function refreshAccountSetting(){
	try{
	var getUrlPage =document.location.toString();
	window.location = getUrlPage;
	}catch(e){
		// to do
	}
}

(function($) {
	try{
	$.fn.resetWelcomeText = function(name) {
		document.getElementById('my-account-settings').innerHTML = name;
		lengthyNameFadeout();
	};
	}catch(e){
		// to do
	}
})(jQuery);


function wrapper_alignment(id){
	if(id == 'account-form-edit') {
		$('fieldset').css('margin-top','39px');
		$('#wizard-form-wrapper').css('padding-bottom','0');
		$('.my-meeting-admin-edit-button-container').css('margin-top','73px');
	}
	if(id == 'meeting-form-edit') {
		$('#wizard-form-wrapper').css('min-height','287px');
	}
	if(id == 'main-form'){
		$('#wizard-form-wrapper').css('padding-bottom','0');
	}
}

function onloadimage(){
	try{
		//For country and State
		var ob = $('#count').val();
		getstatesforcountryval(ob);
	 }
	catch (e){
		
	}
}

function getstatesforcountryval(obj)
{
var url2=resource.base_host+"/?q=learning/my-account/countrycheck/"+obj;
var st = $('#stateid').val();
if (obj != ''){
	$.ajax({
			type: "GET",
		    url : url2,
		    data: { get_param: 'value' }, 
    		dataType: 'json',
    		 success: function (data) { 
    			 
    		    $('#state option').remove();
    		    $('#state').append($('<option>', { 
                    value: '',
                    text : Drupal.t('LBL674') 
                  }));
		        $.each(data, function(index, element) {
        
		        	   $('#state').append($('<option>', { 
                            value: element.sta_code,
                            text : element.sta_name 
                        }));
		        	   
		                if (st != "") {
		                	$('#state').val(st).attr("selected", "selected");
		                	}
		                });
		     	

		        
		    },
		    complete : function(xmlHttpRequest, textStatus) {
		   
			    if((parseInt(xmlHttpRequest.status) != 0) && (parseInt(xmlHttpRequest.status / 100) != 2)) {
			        return;
			    }
			   
			    var responseText = xmlHttpRequest.responseText;
			    
			   
		    }
		    
		  
	   });}
if (obj == '') { 
	$('#state option').remove();
	$('#state').html('<option value="">'+Drupal.t('LBL674')+'</option>');
}
}