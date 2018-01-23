(function($) {
	var dWidth='';
	$.widget("ui.mulitselectdatagrid", {
		_init: function() {
			try{
			var self = this;
			this.gridObj = '$("body").data("mulitselectdatagrid")';
			this.type = '';
			this.entityId = '';
			this.entityType = '';
			this.gridUrl = '';
			this.uniqueId='';
			}catch(e){
				// to do
			}
		},

		// Mode - View / Edit
		loadDataGrid : function (mode, type, searchKeyword, entityId, entityType, excludedId, gridObj){
			try{
			var deleteOption = (mode == 'edit' || mode == 'view_only') ? true : false;
			mode = (mode == 'view_only') ? 'view' : mode;
			var gridmode = (mode == 'edit') ? true : false;
			var multiselectOption = (mode == 'edit') ? false : true;
			var objStr = '';
			var obj = this;

			this.mode = mode;
			this.type = type;
			this.entityId = entityId;
			this.entityType = entityType;
			this.uniqueId = type+'-'+entityId+'-'+entityType;
			
 

				var colNames,colModel;
//				if($('#add-session-delivery-type'+this.uniqueId).val() == "lrn_cls_dty_ilt"){
				var widtharray= new Array(400,150,50,50);
					colNames=[  Drupal.t('LBL1309'),Drupal.t('LBL1306'),'',''];
					colModel=[
					           {name:'IP Address',index:'ip_address', title: false, width:widtharray[0], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, resizable:false  },
					           {name:'IP Category',index:'ip_category', title: false, width:widtharray[1], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: true, sorttype:"string", resizable:false  },
					           {name:'Edit',index:'Edit', title: false, width:widtharray[2], 'widgetObj':objStr, formatter: obj.displayGridValues, sortable: false, resizable:false },
					           {name:'Action',index:'Remove', title: false, width:widtharray[3], 'widgetObj':objStr, formatter: obj.displayGridValues, hidden: false, sortable: false, resizable:false }];

//				}
				$("#datagrid-container-"+this.uniqueId).jqGrid({
					url: this.constructUrl("administration/sitesetup/moduleinfo/ipranges/list"),
					datatype: "json",
					mtype: 'GET',
					colNames: colNames,
					colModel: colModel,
					rowNum:10,
					rowList:[10,25,50],
					pager: '#pager-datagrid-'+obj.uniqueId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					sortname: "ip_category",
					sortorder: "asc",			
					toppager: true,
					botpager: false,
					height: "auto",
					width: 430,
					loadtext: "",
					recordtext: "",
					pgtext: "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					//pgtext:"",
					loadui:false,
					onSortCol:function (){
						 $('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+type+'-'+entityId+'-'+entityType);
					},
					loadComplete:function (){
						obj.callbackDataGrid(); 
						$('body').data('mulitselectdatagrid').destroyLoader('datagrid-div-'+obj.uniqueId);
					}
				}).navGrid('#pager-datagrid-'+this.uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				setQtipPosition();
			}catch(e){
				// to do
			}
		},
		displayGridValues : function(cellvalue, options, rowObject){
			try{
             $('#gbox_datagrid-container-'+this.uniqueId).css("margin", "0 Auto");
			var type = $('body').data('mulitselectdatagrid').type;
			var mode = $('body').data('mulitselectdatagrid').mode;
			var uniqueId = $('body').data('mulitselectdatagrid').uniqueId;
			var entityId = $('body').data('mulitselectdatagrid').entityId;
			var entityType = $('body').data('mulitselectdatagrid').entityType;
			var groupname  = '';
			var index  = options.colModel.index;
			var rowEditOptionId =  rowObject['id'] + '-' + type + '-' + entityId + '-' + entityType + '-' + index;
				if(index == 'ip_address'){
					var ipAddress = rowObject[index];
					return '<span class="vtip" title="'+htmlEntities(ipAddress)+'">'+ipAddress+'</span>';
				} else if(index == 'ip_category'){
					var ip_category = rowObject[index];
					return ip_category;
				} else if(index == 'Edit'){
					var edit = rowObject[index];
					var popupAddSession = "{\'"+entityId+"\':\'0\',\'entityType\':\'addip\',\'url\':\'administration/sitesetup/moduleinfo/ipranges/addedit/"+edit+"\',\'popupDispId\':\'qtip_visible_disp_iprange_ip"+edit+"\',\'catalogVisibleId\':\'qtip_EditSessionIdqtipvisible_disp_iprange_"+edit+"_iprange\',\'wBubble\':\'400\',\'hBubble\':\'auto\',\'tipPosition\':\'tipTopRight\',\'qtipClass\':\'admin-qtip-access-parent\',\'sessionPopupId\':\'catalog-class-basic-addedit-form-container\'}";
					return '<div id="iprange-added-edit-link" style="position:relative;"><span class="vtip" title="'+htmlEntities('Edit')+'"><a onclick="$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('+popupAddSession+'); return false;" class="iprange-added-edit-link-text"> </a></span><span class="narrow-search-results-item-detail-pipe-line session-edit-delete-icons">|</span><div id="qtip_visible_disp_iprange_ip'+edit+'" class="add_session_popup"></div></div>';
				} else if(index == 'Remove'){
						var id = rowObject[index];
					return '<span class="vtip" title="'+htmlEntities('Delete')+'"><a href="JavaScript:void(0);" class="admin-pagination-action-link" onClick="$(\'body\').data(\'mulitselectdatagrid\').deleteEntity(\''+type+'\',\''+entityId+'\',\''+entityType+'\',\''+id+'\');">&nbsp;</a></span>';
				}
			}catch(e){
				// to do
			}
	
		},
		deleteEntity : function(type, entityId, entityType, rowId){
			try{
			//var uniqueId = type+'-'+entityId+'-'+entityType;
			var obj = this;
			$('#admin-data-grid .messages').remove();
			$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+this.uniqueId);
			
			$.ajax({
				url : this.constructUrl("administration/sitesetup/moduleinfo/ipranges/delete/"+rowId),
				async: false,
				success: function(data) {
					//alert($("#datagrid-container-"+obj.uniqueId).getGridParam("records"));
				    $("#datagrid-container-"+obj.uniqueId).trigger("reloadGrid",[{page:1}]);
				}
			});
			//alert($("#datagrid-container-"+obj.uniqueId).getGridParam("records"));
			if ($("#datagrid-container-"+obj.uniqueId).getGridParam("records")-1 <= 10) {
				$("#datagrid-container-"+obj.uniqueId+"_toppager").css('visibility','hidden');
			}
			}catch(e){
				// to do
			}
		},
		callbackDataGrid : function(data){
		try{ 
		  
		  var uniqueId=this.uniqueId; 
			
			if($("#datagrid-container-"+this.uniqueId).getGridParam("records")<=0)
			{
			   $('.ctools-use-modal-processed').submit();
			}
			vtip();	
			checkPaginationWidth();
			$('#datagrid-container-'+this.uniqueId+' tr:even').css("background", "#f7f7f7");
			$('#datagrid-container-'+this.uniqueId+' tr.jqgfirstrow').css("background", "none");
//			$("#datagrid-container-"+this.uniqueId+"_toppager").css('position','absolute');
			$("#gview_datagrid-container-"+this.uniqueId).find( '.ui-jqgrid-bdiv').css('margin-top','0px');
			$("#gview_datagrid-container-"+this.uniqueId).find( '.ui-jqgrid-bdiv').css('overflow','visible');
//			alert($("#datagrid-container-"+this.uniqueId).getGridParam("records"));
			$('#datagrid-container-'+this.uniqueId+' tr td').css({'overflow':'visible'});
			$('div.active-qtip-div tr td').css({'padding-left':'0px'});
			if ($("#datagrid-container-"+this.uniqueId).getGridParam("records") > 10 ) {
				$("#datagrid-container-"+this.uniqueId+"_toppager").css('visibility','visible');
				$("#datagrid-container-"+this.uniqueId+"_toppager").css('display','block');
			}else{
				$("#datagrid-container-"+this.uniqueId+"_toppager").css('visibility','hidden');
			}  
			
			$('body').data('mulitselectdatagrid').destroyLoader('datagrid-div-'+uniqueId);

			$('#first_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId); 
					
				}
			});
			
			$('#prev_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId); 
				}
			});

			$('#next_t_datagrid-container-'+uniqueId+'_toppager').click(function(){ 
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId);
					 
				}
			});

			$('#last_t_datagrid-container-'+uniqueId+'_toppager').click(function(){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId); 
				}
			}); 
			
			$('.ui-pg-input').keypress(function(event){
			var keycode = (event.keyCode ? event.keyCode : event.which);
			if(keycode == '13'){
				if($(this).hasClass('ui-state-disabled')  == false){
					$('body').data('mulitselectdatagrid').createLoader('datagrid-div-'+uniqueId); 
				}
			 }
			 event.stopPropagation();
			}); 
			
			 
			
		}catch(e){
			// to do
		}
	},

	});

	$.extend($.ui.mulitselectdatagrid.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);

$(function(){ $("body").mulitselectdatagrid(); });

//Hide the drop down option values
$('body').bind('click', function(event) {
	try{
	if(event.target.id != 'admin-dropdown-arrow'){
		$('#select-list-dropdown-list').hide();
	}
	}catch(e){
		// to do
	}
});
//remove data grid resizer span
$('.ui-jqgrid .ui-jqgrid-resize').live('mouseover mousemove keypress', function(){
	try{
	$(this).remove();
	}catch(e){
		// to do
	}
});