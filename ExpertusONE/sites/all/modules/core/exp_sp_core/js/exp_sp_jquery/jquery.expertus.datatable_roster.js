(function($) {	
		jQuery.fn.expertusDataTableRoster = function (aDefn,aConfig){	
			
			var config = jQuery.extend({			
				"paging" : true,
				"exportTbl" : true,
				"print" : true,
				"refresh": true,
				"loadonce" :true,			
				"imagePath" : themepath+'/images/',
				"pageLength":10
			}, aConfig);   	
			return this.each(function(){
				$.fn.expertusDataTableRosterRender(this.id, config, aDefn);
			});
		};
		
		
		jQuery.fn.expertusDataTableUser = function (aDefn,aConfig){	
			
			var config = jQuery.extend({			
				"paging" : true,
				"exportTbl" : true,
				"print" : true,
				"refresh": true,
				"loadonce" :true,			
				"imagePath" : themepath+'/images/',
				"pageLength":10
			}, aConfig);   	
			return this.each(function(){
				$.fn.expertusDataTableUserRender(this.id, config, aDefn);
			});
		};

		
		
		// -------- Jq Grid Plugin call -----------------
		jQuery.fn.expertusJqGridDataTable = function (tblId,tblDefn,tblConfig){
			
		
			jQuery("#"+tblId).jqGrid({ 	
				url 		: tblConfig.url,
				gridimgpath : 'images',
				datatype	: 'xml',
				colNames	: tblDefn.colNames,
				colModel	: tblDefn.colModel,
				rowNum		: tblConfig.rowNum,
				autowidth	: true, 
				rownumbers	: true, 
				rowList		: tblConfig.rowList,
				pager		: tblConfig.pagerDivId != '' ? '#'+tblConfig.pagerDivId : '',			
				searchGrid	: {multipleSearch:true, caption : 'go'},
				sortname	: tblConfig.defaultSortBy, 
				viewrecords	: true,
				imgpath : themepath+'/images',
				sortorder: "desc", 	
				//subGrid : true,
				subGridUrl: 'subgrid.php?q=2', 
				subGridUrl: tblDefn.serviceUrl+'&addGrid=true',
				multiselect:true,						
				editurl:"/jqGridModel?model=Wine"/**/
			});	
				
			var searchOptions={ view:true,top: 50, left: "100", width: 150,autosearch:true };			 					
			jQuery("#"+tblId).filterToolbar(searchOptions);				
			jQuery("#"+tblId).jqGrid('navGrid','#gridpager',{edit:false,add:false,del:false,search:false,refresh:false}); 
			jQuery("#"+tblId).jqGrid('navButtonAdd',"#gridpager",{caption:"Search",title:"Toggle Search", onClickButton:function(){}});
			$('#BulRegRosterListTbl').attr('rules','all');
			$('#BulRegRosterListTbl').attr('frame','box');	
			$('#BulRegRosterListTbl').attr('cellpadding','2');				
			$('#BulRegRosterListTbl').css('border','1px solid #A6C9E2');	
			
			
		};
		
		jQuery.fn.expertusDataTableRosterRender = function (tblId, tblConfig, tblDefn){
			//Modified by Ilayaraja
			//Removed SMARTPORTAL.t("Payment Status"), from colNames, and commented the relavant on colModel
			//Increased the username width to 140px and reg_status width to 155px to fix sub grid alignment, to copensate the payment_status width
			//UserId and Name labels changed as User Name and Full Name, since we removed user id
			jQuery("#"+tblId).jqGrid({ 		
				url:tblDefn.serviceUrl,			
				gridimgpath:'images',
				datatype: "xml", 			
				colNames:[SMARTPORTAL.t("User Name"),SMARTPORTAL.t("Full Name"), SMARTPORTAL.t("Registration Status"), SMARTPORTAL.t("Completion Status"),SMARTPORTAL.t("Score"),SMARTPORTAL.t("Grade")], 
				colModel:[ 
						{name:'enrol_pid',index:'enrol_pid', width:70,editable:true, editoptions:{readonly:false,size:10},search:true,stype:'text',searchoptions:{attr:{title:'Some title123'}}}, 
						{name:'username',index:'user_name', width:140,editable:true, editoptions:{readonly:false,size:10},search:true,stype:'text',searchoptions:{attr:{title:'Some title123'}}}, 
						{name:'reg_status',index:'reg_status', width:155,search:true,stype:'text',searchoptions:{attr:{title:'Some title456'}}}, 
						//{name:'payment_status',index:'payment_status', width:100,searchoptions:{attr:{title:'Some title456'}}}, 					
						{name:'comp_status',index:'comp_status', width:105,searchoptions:{attr:{title:'Some title456'}}}, 
						{name:'score',index:'score', width:50,searchoptions:{attr:{title:'Some title456'}}}, 
						{name:'grade',index:'grade', width:50, sortable:false ,searchoptions:{attr:{title:'Some title456'}}} 
						], 			
				rowNum:20, 
				autowidth: true, 
				rownumbers: true, 
				rowList:[20,30,40], 
				pager: '#'+tblId+'gridpager',			
				searchGrid: {multipleSearch:true, caption : 'go'},
				sortname: 'reg.id', 
				viewrecords: true, 
				imgpath : themepath+'/images',
				sortorder: "desc", 	
				subGrid : true, 
				//subGridUrl: 'subgrid.php?q=2', 
				subGridUrl: tblDefn.serviceUrl+'&addGrid=true',
				multiselect:true,
				subGridModel: [
					{ 
						name : [SMARTPORTAL.t("Registration Date"),SMARTPORTAL.t("Registration Status Date"),SMARTPORTAL.t("Completion Date"),SMARTPORTAL.t("Valid From"),SMARTPORTAL.t("Valid To")], 
						index :['username','username','username','username','username'],
						width : [100,150,100,100,100] 
					} 
				], 				
				editurl:"/jqGridModel?model=Wine"		
			}); 
			
			var searchOptions={ view:true,top: 50, left: "100", width: 150,autosearch:true };	
			//jQuery("#"+tblId).jqGrid('navGrid','#gridpager',{add:true,edit:true,del:true}); 					
			jQuery("#"+tblId).filterToolbar(searchOptions);				
			jQuery("#"+tblId).jqGrid('navGrid','#'+tblId+'gridpager',{edit:false,add:false,del:false,search:false,refresh:false}); 
			jQuery("#"+tblId).jqGrid('navButtonAdd',"#"+tblId+"gridpager",{caption:"Search",title:"Toggle Search", onClickButton:function(){}});
			$('#BulRegRosterListTbl').attr('rules','all');
			$('#BulRegRosterListTbl').attr('frame','box');	
			$('#BulRegRosterListTbl').attr('cellpadding','2');				
			$('#BulRegRosterListTbl').css('border','1px solid #A6C9E2');						
		
		};
		

		jQuery.fn.expertusDataTableUserRender = function (tblId, tblConfig, tblDefn){
			//alert("jquery render "+tblId);
			
			/*Modified By : Ilayaraja.E
			 *Description : replaced UserID(colNames) with User Name and colModel fields user_id(name) with user_name, 
			 *				user_id(index) with user_name and role_name(index) with name 
			 */
			jQuery("#"+tblId).jqGrid({ 		
				url:tblDefn.serviceUrl,			
				gridimgpath:'images',
				datatype: "xml", 			
				colNames:[SMARTPORTAL.t("User Name"),SMARTPORTAL.t("Name"), SMARTPORTAL.t("Role")], 
				colModel:[ 
						{name:'user_name',index:'user_name', width:70,editable:true, editoptions:{readonly:false,size:10},search:true,stype:'text',searchoptions:{attr:{title:'Some title123'}}}, 
						{name:'full_name',index:'full_name', width:90,editable:true, editoptions:{readonly:false,size:10},search:true,stype:'text',searchoptions:{attr:{title:'Some title123'}}}, 
						{name:'role_name',index:'name', width:105,search:true,stype:'text',searchoptions:{attr:{title:'Some title456'}}}						 
						],			
				rowNum:20, 
				autowidth: true, 
				rownumbers: true, 
				rowList:[20,30,40], 
				pager: '#gridpager',			
				searchGrid: {multipleSearch:true, caption : 'go'},
				sortname: 'id', 
				viewrecords: true, 
				imgpath : themepath+'/images',
				sortorder: "desc",				
				multiselect:true,
								
				editurl:"/jqGridModel?model=Wine"		
			}); 
			
			var searchOptions={ view:true,top: 50, left: "100", width: 150,autosearch:true };	
			//jQuery("#"+tblId).jqGrid('navGrid','#gridpager',{add:true,edit:true,del:true}); 	
			
			jQuery("#"+tblId).filterToolbar(searchOptions);				
			jQuery("#"+tblId).jqGrid('navGrid','#gridpager',{edit:false,add:false,del:false,search:false,refresh:false}); 
			jQuery("#"+tblId).jqGrid('navButtonAdd',"#gridpager",{caption:"Search",title:"Toggle Search", onClickButton:function(){}});
			$('#UserRoleListTbl').attr('rules','all');
			$('#UserRoleListTbl').attr('frame','box');	
			$('#UserRoleListTbl').attr('cellpadding','2');				
			$('#UserRoleListTbl').css('border','1px solid #A6C9E2');					
		
		};

	})(jQuery); 
